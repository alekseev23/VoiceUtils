//---------------------------------------------------------------------------

#pragma hdrstop

#include <System.Math.hpp>
#include <stdlib.h>
#include "SpecterWork.h"
#include "../../_Common_/Lib/files.h"
#include "Envelope.h"
#include "FFT.h"
#include "FrequencyData.h"
#include "common.h"
#include "TSpecters.h"
#include "TInterpolation.h"
#include "TColorSceme.h"
#include "TSpecterWorkThread.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

SpecterWork::SpecterWork() {
Loaded = false;
FreqLinePos = new char [65536];
}
//---------------------------------------------------------------------------

SpecterWork::~SpecterWork() {
if (Buf != NULL) delete[] Buf;
if (Data != NULL) delete[] Data;
if (Middle != NULL) delete[] Middle;
if (Main != NULL) delete[] Main;

if (Re != NULL) delete[] Re;
if (Im != NULL) delete[] Im;
delete[] FreqLinePos;
}
//---------------------------------------------------------------------------

// Чтение данных из WAV файла
bool SpecterWork::ReadData(UnicodeString filename) {
	return ReadDataFromWAV(filename);
}
//---------------------------------------------------------------------------

// Чтение данных из WAV файла
bool SpecterWork::ReadDataFromWAV(UnicodeString filename) {
FileName = filename;
Loaded = false;
// Грузим данные из DAT файла
if (!FileExists(filename)) {
	ShowMessage("Файл не найден: " + filename);
	return false;
}
int fsize = getFileSize(filename);
if (fsize == 0) return false;
BufSize = (fsize - 44) / 2;
if (Buf != NULL) delete[] Buf;
Buf = new short[BufSize];
FILE *fd;
if ((fd = _wfopen(filename.w_str(), L"rb")) == NULL) {
	ShowMessage("Ошибка открытия файла: " + filename);
	return false;
}
fseek(fd, 44, SEEK_SET);
fread(Buf, 2, BufSize, fd);
fclose(fd);
Loaded = true;
return true;
}
//---------------------------------------------------------------------------
// Чтение данных из DAT файла
bool SpecterWork::ReadDataFromDAT(UnicodeString filename) {
FileName = filename;
Loaded = false;
// Грузим данные из DAT файла
if (!FileExists(filename)) {
	ShowMessage("Файл не найден: " + filename);
	return false;
}
int fsize = getFileSize(filename);
if (fsize == 0) return false;
BufSize = fsize / 2;
if (Buf != NULL) delete[] Buf;
Buf = new short[BufSize];
FILE *fd;
if ((fd = _wfopen(filename.w_str(), L"rb")) == NULL) {
	ShowMessage("Ошибка открытия файла: " + filename);
	return false;
}
fread(Buf, 2, BufSize, fd);
fclose(fd);
Loaded = true;
return true;
}
//---------------------------------------------------------------------------
// Чтение данных из DAT файла
bool SpecterWork::ReadDataFromDAT(UnicodeString filename, int begin, int len) {
FileName = filename;
Loaded = false;
// Грузим данные из DAT файла
if (!FileExists(filename)) {
	ShowMessage("Файл не найден: " + filename);
	return false;
}
int fsize = getFileSize(filename);
if (fsize < (begin + len) * 2) return false;
BufSize = len;
if (Buf != NULL) delete[] Buf;
Buf = new short[BufSize];
FILE *fd;
if ((fd = _wfopen(filename.w_str(), L"rb")) == NULL) {
	ShowMessage("Ошибка открытия файла: " + filename);
	return false;
}
fseek(fd, 2 * begin, SEEK_SET);
fread(Buf, 2, BufSize, fd);
fclose(fd);
Loaded = true;
return true;
}
//---------------------------------------------------------------------------
// Нормализация данных
void SpecterWork::Normalize(void) {
if (!Loaded) return;
short max = 0, val;
// Находим максимум
for (auto i = 0; i < BufSize; i++) {
	val = abs(Buf[i]);
	if (val > max) max = val;
}
for (auto i = 0; i < BufSize; i++) {
	Buf[i] = Buf[i] * max / 32768;
}
}

//---------------------------------------------------------------------------
// Вычисление спектров
void SpecterWork::CountAllSpecters(int n) {
if (!Loaded) return;
int RADIUS_SUM = 0, pos, a, b;
N = n;
N2 = N / 2;
Count = 400 * (BufSize - N) / 48000;
FrequencyData *FrequencyData = new class FrequencyData(N);
if (Data != NULL) delete[] Data;
Data = new double [1LL * Count * N2];
MaxValue = 0.;
double val;
for (int i = 0; i < Count; i++) {
	pos = i * (BufSize - N) / Count;
	//FrequencyData->calculateeWithStat(Buf, pos, RADIUS_SUM);
    FrequencyData->calculate(Buf, pos);
	// Сохраняем данные в массив
	for (int j = 0; j < N2; j++) {
		val = FrequencyData->Data[j];
		Data[1LL * i * N2 + j] = val;
		if (val > MaxValue) MaxValue = val;
	}
}
delete FrequencyData;
}
//---------------------------------------------------------------------------
// Вычисление спектров
void SpecterWork::CountAllSpecters(int n, bool energy_specters) {
if (!Loaded) return;
int RADIUS_SUM = 0, pos, a, b;
N = n;
N2 = N / 2;
Count = 400 * (BufSize - N) / 48000;
FrequencyData *FrequencyData = new class FrequencyData(N);
if (Data != NULL) delete[] Data;
Data = new double [1LL * Count * N2];
MaxValue = 0.;
double val;
for (int i = 0; i < Count; i++) {
	pos = i * (BufSize - N) / Count;
	FrequencyData->calculate(Buf, pos);
	// Сохраняем данные в массив
	for (int j = 0; j < N2; j++) {
		val = FrequencyData->Data[j];
		if (energy_specters) {
			val = val * val * (j + 1) * (j + 1);
		}
		Data[1LL * i * N2 + j] = val;
		if (val > MaxValue) MaxValue = val;
	}
}
delete FrequencyData;
}

//---------------------------------------------------------------------------
// Вычисление спектров (n - Кол-во точек для анализа, например 512)
// specters_in_second - кол-во спектров в секунду
void SpecterWork::CountSpecters(int n, int specters_in_second) {
if (!Loaded) return;
int pos, a, b, pp;
N = n;
N2 = N / 2;
Count = specters_in_second * (BufSize - N) / 48000;
FrequencyData *FrequencyData = new class FrequencyData(N);
if (Data != NULL) delete[] Data;
Data = new double [1LL * Count * N2];
MaxValue = 0.;
double val;
for (int i = 0; i < Count; i++) {
	pos = i * (BufSize - N) / Count;
	FrequencyData->calculate(Buf, pos);
	// Сохраняем данные в массив
	for (int j = 0; j < N2; j++) {
		val = FrequencyData->Data[j];
		Data[1LL * i * N2 + j] = val;
		if (val > MaxValue) MaxValue = val;
	}
}
delete FrequencyData;


}
//---------------------------------------------------------------------------
// Вычисляем основные спектры
void SpecterWork::CountMainSpecters(int main_count) {
    int a, b;
	MainCount = main_count;
	// Основные спектры
	if (Main != NULL) delete[] Main;
	Main = new double [MainCount * N2];
	// Обнуляем основные спектры
	for (auto i = 0; i < MainCount * N2; i++) Main[i] = 0.;
	// Цикл по основным спектрам
	for (auto i = 0; i < MainCount; i++) {
		a = i * Count / MainCount;
		b = (i + 1) * Count / MainCount;
		if (b > Count) b = Count;
		// Цикл по спектрам внутри группы (находим сумму)
		for (auto j = a; j < b; j++) {
			for (auto k = 0; k < N2; k++) {
				Main[i * N2 + k] += Data[1LL * j * N2 + k];
			}
		}
		// Находим среднее значение
		auto c = b - a;
		for (auto k = 0; k < N2; k++) Main[i * N2 + k] /= c;
	}
}
//---------------------------------------------------------------------------
// Из обычных вычисляем энергетические спектры
void SpecterWork::GetEnergySpecter(double *specter1, double *specter2) {
	for (auto i = 0; i < N2; i++) {
		specter2[i] = specter1[i] * specter1[i] * (i + 1) * (i + 1);
	}
}

//---------------------------------------------------------------------------
// Вычисление спектров
void SpecterWork::CountSpecters(int n) {
if (!Loaded) return;
int RADIUS_SUM = 0, pos, a, b, pp;
N = n;
N2 = N / 2;
Count = 400 * (BufSize - N) / 48000;
Count = 2 * 4800 * (BufSize - N) / 48000;
//if (Count > 1000) Count = 1000;
FrequencyData *FrequencyData = new class FrequencyData(N);
if (Data != NULL) delete[] Data;
Data = new double [1LL * Count * N2];

if (Re != NULL) delete[] Re;
Re = new double [Count * N2 * 2];
if (Im != NULL) delete[] Im;
Im = new double [Count * N2 * 2];

MaxValue = 0.;
double val;
for (int i = 0; i < Count; i++) {
	pos = i * (BufSize - N) / Count;
	FrequencyData->calculate(Buf, pos);
	// Сохраняем данные в массив
	for (int j = 0; j < N2; j++) {
		val = FrequencyData->Data[j];
		Data[1LL * i * N2 + j] = val;
		if (val > MaxValue) MaxValue = val;
	}

	// Готовим значения
	pp = i * N;
	for (int j = 0; j < N; j++) {
		Re[pp + j] = Buf[pos + j];
		Im[pp + j] = 0.;
	}
	//FFT(&Re[pp], &Im[pp], N, 9, FT_DIRECT);
    FFT(&Re[pp], &Im[pp], N, GetLogN(N), FT_DIRECT);
}
delete FrequencyData;

// Основные спектры
if (Main != NULL) delete[] Main;
Main = new double [MainCount * N2];
// Обнуляем основные спектры
for (auto i = 0; i < MainCount * N2; i++) Main[i] = 0.;
// Цикл по основным спектрам
for (auto i = 0; i < MainCount; i++) {
	a = i * Count / 5;
	b = (i + 1) * Count / 5;
	if (b > Count) b = Count;
	// Цикл по спектрам внутри группы (находим сумму)
	for (auto j = a; j < b; j++) {
		for (auto k = 0; k < N2; k++) {
			Main[i * N2 + k] += Data[1LL * j * N2 + k];
		}

	}
	// Находим среднее значение
	auto c = b - a;
	for (auto k = 0; k < N2; k++) Main[i * N2 + k] /= c;
}
}
//---------------------------------------------------------------------------
// Вычисление спектров
void SpecterWork::CountSpecters(int n, bool energy_specters) {
if (!Loaded) return;
int RADIUS_SUM = 0, pos, a, b, pp;
N = n;
N2 = N / 2;
//Count = 400 * (BufSize - N) / 48000;
//Count = 1 * 4800 * (BufSize - N) / 48000;
Count = 5 * 4800 * (BufSize) / 48000;
//if (Count > 1000) Count = 1000;
FrequencyData *FrequencyData = new class FrequencyData(N);
if (Data != NULL) delete[] Data;
Data = new double [1LL * Count * N2];

if (Re != NULL) delete[] Re;
Re = new double [Count * N2 * 2];
if (Im != NULL) delete[] Im;
Im = new double [Count * N2 * 2];

MaxValue = 0.;
double val;
for (int i = 0; i < Count; i++) {
	//pos = i * (BufSize - N) / Count;
    pos = i * (BufSize) / Count;
	//FrequencyData->calculate(Buf, pos);
    FrequencyData->calculate(Buf, BufSize, pos, 256);
	// Сохраняем данные в массив
	for (int j = 0; j < N2; j++) {
		val = FrequencyData->Data[j];
		if (energy_specters) {
			//val = val * val * (j + 1) * (j + 1);
			val = val * 8000 * (j + 1) * N2 / (1364 * 4096);
            val = val * val;
		}
		Data[1LL * i * N2 + j] = val;
		if (val > MaxValue) MaxValue = val;
	}

	// Готовим значения
	pp = i * N;
	for (int j = 0; j < N; j++) {
		Re[pp + j] = Buf[pos + j];
		Im[pp + j] = 0.;
	}
	//FFT(&Re[pp], &Im[pp], N, 9, FT_DIRECT);
    FFT(&Re[pp], &Im[pp], N, GetLogN(N), FT_DIRECT);
}
delete FrequencyData;

// Основные спектры
if (Main != NULL) delete[] Main;
Main = new double [MainCount * N2];
// Обнуляем основные спектры
for (auto i = 0; i < MainCount * N2; i++) Main[i] = 0.;
// Цикл по основным спектрам
for (auto i = 0; i < MainCount; i++) {
	a = i * Count / 5;
	b = (i + 1) * Count / 5;
	if (b > Count) b = Count;
	// Цикл по спектрам внутри группы (находим сумму)
	for (auto j = a; j < b; j++) {
		for (auto k = 0; k < N2; k++) {
			Main[i * N2 + k] += Data[1LL * j * N2 + k];
		}

	}
	// Находим среднее значение
	auto c = b - a;
	for (auto k = 0; k < N2; k++) Main[i * N2 + k] /= c;
}
}
//---------------------------------------------------------------------------
// Вычисление спектров
void SpecterWork::CountSpectersThread(int n, bool energy_specters, int count) {
if (!Loaded) return;
int pos, a, b, pp, d;
N = n;
N2 = N / 2;
Count = count;
if (Data != NULL) delete[] Data;
Data = new double [1LL * Count * N2];
// Подготовка и старт
TSpecterWorkThread **SpecterWorkThread = new TSpecterWorkThread* [CPUCount];
d = (Count + CPUCount - 1) / CPUCount;
for (auto i = 0; i < CPUCount; i++) {
    SpecterWorkThread[i] = new TSpecterWorkThread(true);
    SpecterWorkThread[i]->A = i * d;
    SpecterWorkThread[i]->B = (i + 1) * d;
    if (SpecterWorkThread[i]->B > Count) SpecterWorkThread[i]->B = Count;
    SpecterWorkThread[i]->Buf = Buf;
    SpecterWorkThread[i]->BufSize = BufSize;
    SpecterWorkThread[i]->Data = Data;
    SpecterWorkThread[i]->EnergySpecters = energy_specters;
    SpecterWorkThread[i]->N = N;
    SpecterWorkThread[i]->Count = Count;
    SpecterWorkThread[i]->Resume();
}
// Ожидание
int sum = 0;
while (sum < CPUCount) {
    Sleep(100);
    MaxValue = 0.;
    sum = 0;
	for (auto i = 0; i < CPUCount; i++) {
    	if (SpecterWorkThread[i]->Finished) {
        	sum++;
            if (SpecterWorkThread[i]->MaxValue > MaxValue) MaxValue = SpecterWorkThread[i]->MaxValue;
        }
    }
}
// Завершение
for (auto i = 0; i < CPUCount; i++) delete SpecterWorkThread[i];
delete[] SpecterWorkThread;
// Основные спектры
if (Main != NULL) delete[] Main;
Main = new double [MainCount * N2];
// Обнуляем основные спектры
for (auto i = 0; i < MainCount * N2; i++) Main[i] = 0.;
// Цикл по основным спектрам
for (auto i = 0; i < MainCount; i++) {
	a = i * Count / 5;
	b = (i + 1) * Count / 5;
	if (b > Count) b = Count;
	// Цикл по спектрам внутри группы (находим сумму)
	for (auto j = a; j < b; j++) {
		for (auto k = 0; k < N2; k++) {
			Main[i * N2 + k] += Data[1LL * j * N2 + k];
		}

	}
	// Находим среднее значение
	auto c = b - a;
	for (auto k = 0; k < N2; k++) Main[i * N2 + k] /= c;
}
}
//---------------------------------------------------------------------------
/*
Вычисление спектров
n - Кол-во аудио точек в спектре
energy_specters - постые спектры или энергетические
count - кол-во спектров для вычисления
begin - начало аудио области
end - конец аудио области
windows_size - размер окна
shape_type - тип шейпера
*/
void SpecterWork::CountSpectersThread(int n, bool energy_specters, int specters_per_second, int begin, int end, int windows_size, int shape_type) {
if (!Loaded) return;
int pos, a, b, pp, d;
N = n;
N2 = N / 2;
SpectersPerSecond = specters_per_second;
Count = 1LL * specters_per_second * (end - begin) / 48000;
//std::cout << "specters_per_second=" << specters_per_second <<  " begin=" << begin <<  " end=" << end <<" Count=" << Count << " 1LL * Count * N2=" << (1LL * Count * N2) << std::endl;
if (Data != NULL) delete[] Data;
__int64 t = 2LL * Count * N2;
Data = new double [t];
if (Data == NULL) {
    std::cout << "Data allocation error!" << std::endl;
    return;
}
// Подготовка и старт
TSpecterWorkThread **SpecterWorkThread = new TSpecterWorkThread* [CPUCount];
d = (Count + CPUCount - 1) / CPUCount;
for (auto i = 0; i < CPUCount; i++) {
    SpecterWorkThread[i] = new TSpecterWorkThread(true);
    SpecterWorkThread[i]->A = i * d;
    SpecterWorkThread[i]->B = (i + 1) * d;
    if (SpecterWorkThread[i]->B > Count) SpecterWorkThread[i]->B = Count;
    SpecterWorkThread[i]->Buf = Buf;
    SpecterWorkThread[i]->BufSize = BufSize;
    SpecterWorkThread[i]->Data = Data;
    SpecterWorkThread[i]->Begin = begin;
    SpecterWorkThread[i]->End = end;
    SpecterWorkThread[i]->EnergySpecters = energy_specters;
    SpecterWorkThread[i]->WindowSize = windows_size;
    SpecterWorkThread[i]->ShapeType = shape_type;
    SpecterWorkThread[i]->N = N;
    SpecterWorkThread[i]->Count = Count;
    SpecterWorkThread[i]->Resume();
}
// Ожидание
int sum = 0;
while (sum < CPUCount) {
    Sleep(100);
    MaxValue = 0.;
    sum = 0;
	for (auto i = 0; i < CPUCount; i++) {
    	if (SpecterWorkThread[i]->Finished) {
        	sum++;
            if (SpecterWorkThread[i]->MaxValue > MaxValue) MaxValue = SpecterWorkThread[i]->MaxValue;
        }
    }
}
// Завершение
for (auto i = 0; i < CPUCount; i++) delete SpecterWorkThread[i];
delete[] SpecterWorkThread;
// Основные спектры
if (Main != NULL) delete[] Main;
Main = new double [MainCount * N2];
// Обнуляем основные спектры
for (auto i = 0; i < MainCount * N2; i++) Main[i] = 0.;
// Цикл по основным спектрам
for (auto i = 0; i < MainCount; i++) {
	a = i * Count / 5;
	b = (i + 1) * Count / 5;
	if (b > Count) b = Count;
	// Цикл по спектрам внутри группы (находим сумму)
	for (auto j = a; j < b; j++) {
		for (auto k = 0; k < N2; k++) {
            t = 1LL * j * N2 + k;
			Main[i * N2 + k] += Data[t];
		}

	}
	// Находим среднее значение
	auto c = b - a;
	for (auto k = 0; k < N2; k++) Main[i * N2 + k] /= c;
}
}
//---------------------------------------------------------------------------
// Вычисление спектров
void SpecterWork::CountAllSpecters4X(int n, bool energy_specters) {
if (!Loaded) return;
int pos, a, b, pp;
double val;
// Интерполируем значения массива Buf[] и получаем Buf2[] с четверо большим кол-вом точек
TInterpolation Interpolation;
int BufSize2 = BufSize * 4;
short *Buf2 = new short [BufSize2];
Interpolation.InterpolateX4(BufSize, Buf, Buf2);
N = n;
N2 = N / 2;
Count = 2 * 4800 * (BufSize - N) / 48000;
FrequencyData *FrequencyData = new class FrequencyData(N);
if (Data != NULL) delete[] Data;
Data = new double [1LL * Count * N2];
MaxValue = 0.;
for (int i = 0; i < Count; i++) {
	pos = i * (BufSize - N) / Count;
	FrequencyData->calculate(Buf, pos);
	// Сохраняем данные в массив
	for (int j = 0; j < N2; j++) {
		val = FrequencyData->Data[j];
		if (energy_specters) {
			//val = val * val * (j + 1) * (j + 1);
			//val = val * (j + 1) * (j + 1);
		}
		Data[1LL * i * N2 + j] = val;
		if (val > MaxValue) MaxValue = val;
	}
}
delete FrequencyData;
}
//---------------------------------------------------------------------------
// Вычисление спектров
void SpecterWork::CountSpecters4X(int n, bool energy_specters) {
if (!Loaded) return;
int pos, a, b, pp;
double val;
// Интерполируем значения массива Buf[] и получаем Buf2[] с четверо большим кол-вом точек
TInterpolation Interpolation;
int BufSize2 = BufSize * 4;
short *Buf2 = new short [BufSize2];
Interpolation.InterpolateX4(BufSize, Buf, Buf2);

N = n;
N2 = N / 2;
Count = 400 * (BufSize2 - N) / 48000;
FrequencyData *FrequencyData = new class FrequencyData(N);
if (Data != NULL) delete[] Data;
Data = new double [1LL * Count * N2];
MaxValue = 0.;
for (int i = 0; i < Count; i++) {
	pos = i * (BufSize - N) / Count;
	FrequencyData->calculate(Buf, pos);
	// Сохраняем данные в массив
	for (int j = 0; j < N2; j++) {
		val = FrequencyData->Data[j];
		if (energy_specters) {
			//val = val * val * (j + 1) * (j + 1);
			//val = val * (j + 1) * (j + 1);
		}
		Data[1LL * i * N2 + j] = val;
		if (val > MaxValue) MaxValue = val;
	}
}
delete FrequencyData;

// Основные спектры
if (Main != NULL) delete[] Main;
Main = new double [MainCount * N2];
// Обнуляем основные спектры
for (auto i = 0; i < MainCount * N2; i++) Main[i] = 0.;
// Цикл по основным спектрам
for (auto i = 0; i < MainCount; i++) {
	a = i * Count / 5;
	b = (i + 1) * Count / 5;
	if (b > Count) b = Count;
	// Цикл по спектрам внутри группы (находим сумму)
	for (auto j = a; j < b; j++) {
		for (auto k = 0; k < N2; k++) {
			Main[i * N2 + k] += Data[1LL * j * N2 + k];
		}

	}
	// Находим среднее значение
	auto c = b - a;
	for (auto k = 0; k < N2; k++) Main[i * N2 + k] /= c;
}
// Удаляем ненужные объекты
delete[] Buf2;
}
//---------------------------------------------------------------------------

/* Получение спектров
type = 0 - среднее значение всех спектров
type = 1 - среднее значение начальных спектров
type = 2 - среднее значение средних спектров
type = 3 - среднее значение последних спектров
*/
void SpecterWork::GetMiddle(int type, int scale) {
if (!Loaded) return;
int n = 0, a, b;
if (Middle != NULL) delete[] Middle;
Middle = new double [N2];
// Начальная зачистка
for (int i = 0; i < N2; i++) Middle[i] = 0.;
//
switch (type) {
	case 0: a = 0; b = Count; break;
	case 1: a = 0; b = Count / 3; break;
	case 2: a = Count / 3; b = a + 2; break;
	case 3: b = Count; a = b - Count / 3; break;
	default: return;
}
for (int i = a; i < b; i++) {
	//for (int j = 0; j < N2; j++) Middle[j] += Data[i * N2 + j];
	for (int j = 0; j < N2; j++) Middle[j] += GetSpecterPointValue(i, j, scale);
	n++;
}
for (int i = 0; i < N2; i++) Middle[i] /= n;
}
//---------------------------------------------------------------------------
// Получение спектров в массив Middle[N2]
void SpecterWork::GetMiddle(int scale, int start, int num) {
if (!Loaded) return;
if (Middle != NULL) delete[] Middle;
Middle = new double [N2];
// Начальная зачистка
for (int i = 0; i < N2; i++) Middle[i] = 0.;
//
for (int i = start; i < start + num; i++) {
	for (int j = 0; j < N2; j++) Middle[j] += GetSpecterPointValue(i, j, scale);
}
for (int i = 0; i < N2; i++) Middle[i] /= num;
}
//---------------------------------------------------------------------------
// Ищет номер спектра с макимальной суммой нижних частот
int SpecterWork::FindBest(void) {
if (!Loaded) return -1;
int pos = 0;
double max_sum = 0., sum;
for (auto i = 0; i < Count; i++) {
	sum = 0.;
	for (auto j = 0; j < 10; j++) sum += Data[1LL * i * N2 + j];
	if (sum > max_sum) {
		max_sum = sum;
		pos = i;
	}
}
return pos;
}
//---------------------------------------------------------------------------

/* Получение значения точки спектра
scale = 1 - линейная шкала
scale = 2 - десятичный логарифм
scale = 3 - логарифм по основанию 1.042
*/
double SpecterWork::GetSpecterPointValue(int specter, int i, int scale) {
if (!Loaded) return 0.;
__int64 t = 1LL * specter * N2 + i;
double val = Data[t];
return GetSpecterValue(val, scale);
}

//---------------------------------------------------------------------------

/* Получение значения точки спектра
scale = 1 - линейная шкала
scale = 2 - десятичный логарифм
scale = 3 - логарифм по основанию 1.042
*/
double SpecterWork::GetSpecterValue(double val, int scale) {
if (!Loaded) return 0.;
switch (scale) {
	case 1: // Линейная шкала
			return val;
			break;
	case 2: // Десятичный логарифм
			if (val <= 0.) val = 0.;
			else {
				val = log10(val);
				if (val <= 0.) val = 0.;
			}
			return val;
			break;
	case 3: // Логарифм по основанию
			if (val <= 0.) val = 0.;
			else {
				val = Math::LogN(1.042, val);
				if (val <= 0.) val = 0.;
			}
			return val;
			break;
}
return 0.;
}

//---------------------------------------------------------------------------
// Рисование топографии спектрограмы
void SpecterWork::DrawSpectrograms(TBitmap *BMP, int scale) {
if (!Loaded) return;
int sx, sy, i;
unsigned char *p, *pp, color;
double val, old_val, max;
sx = Count;
sy = N2;
TBitmap *bmp = new TBitmap;
bmp->PixelFormat = pf24bit;
bmp->SetSize(sx, 2 * sy + 1);
//
switch (scale) {
	case 1: max = MaxValue; break;
	case 2: max = log10(MaxValue); break;
	case 3: max = Math::LogN(1.042, MaxValue); break;
}
// Цветовая схема
TColor left_color = clGreen, right_color = clGreen, control_color[4];
control_color[0] = (TColor)(0);
control_color[1] = (TColor)(255);
control_color[2] = (TColor)(255 << 8);
control_color[3] = (TColor)(255 << 16);
//double values[4] = {0., max / 3, 2. * max / 3, max};
double values[4] = {0., max / 10, max / 3, max};
TColorSceme *ColorSceme = new TColorSceme(left_color, right_color, 4, control_color, values, 1000);
//
TColor control_color2[3];
control_color2[0] = (TColor)(255);
control_color2[1] = (TColor)(255 << 8);
control_color2[2] = (TColor)(255 << 16);
double values2[3] = { -max / 20., 0., max / 20.};
TColorSceme *ColorSceme2 = new TColorSceme(clBlack, clBlack, 3, control_color2, values2, 1000);
//
unsigned char r, g, b;
for (auto y = 0; y < sy; y++) {
	p = (unsigned char *)bmp->ScanLine[y];
	pp = (unsigned char *)bmp->ScanLine[sy + 1 + y];
	i = 0;
	for (auto x = 0; x < sx; x++) {
		/*val = 250. * GetSpecterPointValue(x, N2 - y - 1, scale) / max;
		color = (unsigned char) val;
		p[i++] = color;
		p[i++] = 0;
		p[i++] = 0;
		*/
		val = GetSpecterPointValue(x, N2 - y - 1, scale);
		ColorSceme->Get(val, r, g, b);
		p[i] = r;
		p[i + 1] = g;
		p[i + 2] = b;
		if (x > 0) {
			ColorSceme2->Get(val - old_val, r, g, b);
			pp[i] = r;
			pp[i + 1] = g;
			pp[i + 2] = b;
		}
		i += 3;
		old_val = val;
	}
}
BMP->Assign(bmp);
delete bmp;
}

//---------------------------------------------------------------------------
// Рисование топографии спектрограмы
void SpecterWork::DrawSpectrograms2(TBitmap *BMP, int scale) {
if (!Loaded) return;
int sx, sy, i, yy;
unsigned char *p, color;
double val, max;
bool flag;
sx = Count;
sy = N2;
TBitmap *bmp = new TBitmap;
bmp->PixelFormat = pf24bit;
bmp->SetSize(sx, sy);
//
switch (scale) {
	case 1: max = MaxValue; break;
	case 2: max = log10(MaxValue); break;
	case 3: max = Math::LogN(1.042, MaxValue); break;
}
// Цветовая схема
TColor left_color = clYellow, right_color = clYellow, control_color[4];
control_color[0] = (TColor)(0);
control_color[1] = (TColor)(255);
control_color[2] = (TColor)(255 << 8);
control_color[3] = (TColor)(255 << 16);
double values[4] = {0., max / 3, 2. * max / 3, max};
TColorSceme *ColorSceme = new TColorSceme(left_color, right_color, 4, control_color, values, 1000);
//
unsigned char r, g, b;
for (auto y = 0; y < sy; y++) {
	p = (unsigned char *)bmp->ScanLine[y];
	i = 0;
	yy = N2 - y - 1;
    flag = false;
	switch (N2) {
		case  256: if ((yy % 20) == 0) flag = true; break;
		case  512: if ((yy % 40) == 0) flag = true; break;
		case 1024: if ((yy % 80) == 0) flag = true; break;
		case 2048: if ((yy % 160) == 0) flag = true; break;
		case 4096: if ((yy % 320) == 0) flag = true; break;
		default:   flag = false;
	}
	for (auto x = 0; x < sx; x++) {
		val = GetSpecterPointValue(x, yy, scale);
		ColorSceme->Get(val, r, g, b);
		if (flag) {
			if (r < 240) r += 15; else r = 255;
			if (g < 240) g += 15; else g = 255;
			if (b < 240) b += 15; else b = 255;
		}
		p[i++] = r;
		p[i++] = g;
		p[i++] = b;
	}
}
BMP->Assign(bmp);
delete bmp;
}
//---------------------------------------------------------------------------
// Рисование топографии спектрограмы
void SpecterWork::DrawSpectrograms2(TBitmap *BMP, int scale, int freq_width, bool show_grid_lines) {
if (!Loaded) return;
int sx, sy, i, yy, SpectersPerSecond10;
unsigned char *p, color;
double val, max;
bool flag;
// Вычисляем тики частоте
GetFreqPos();
sx = Count;
sy = freq_width;
SpectersPerSecond10 = SpectersPerSecond / 10;
TBitmap *bmp = new TBitmap;
bmp->PixelFormat = pf24bit;
bmp->SetSize(sx, sy);
//
switch (scale) {
	case 1: max = MaxValue; break;
	case 2: max = log10(MaxValue); break;
	case 3: max = Math::LogN(1.042, MaxValue); break;
}
// Цветовая схема
TColor left_color = clYellow, right_color = clYellow, control_color[4];
control_color[0] = (TColor)(0);
control_color[1] = (TColor)(255);
control_color[2] = (TColor)(255 << 8);
control_color[3] = (TColor)(255 << 16);
double values[4] = {0., 1. * max / 20, 1. * max / 4, max};
//double values[5] = {0., 1. * max / 1000, 1. * max / 100, 1. * max / 10, max};
TColorSceme *ColorSceme = new TColorSceme(left_color, right_color, 4, control_color, values, 1000);
//
unsigned char r, g, b;
for (auto y = 0; y < sy; y++) {
	p = (unsigned char *)bmp->ScanLine[y];
	i = 0;
	yy = sy - 1 - y;
   	if (show_grid_lines) {
    	if (FreqLinePos[yy]) {
			for (auto x = 0; x < sx; x++) {
				p[i++] = 30;                
				p[i++] = 30;                
				p[i++] = 30;                
            }
			continue;
        }
    }
	for (auto x = 0; x < sx; x++) {
        // Отрисовка сетки
    	if (show_grid_lines) {
            if (x % SpectersPerSecond == 0) { // Линия 1 секунда
				p[i++] = 80;                
				p[i++] = 80;                
				p[i++] = 80;                
                continue;
            }
            if (x % SpectersPerSecond10 == 0) { // Линия 0.1 секунды
				p[i++] = 30;
				p[i++] = 30;                
				p[i++] = 30;                
                continue;
            }
        }    
		val = GetSpecterPointValue(x, yy, scale);
		ColorSceme->Get(val, r, g, b);
		p[i++] = r;
		p[i++] = g;
		p[i++] = b;
	}
}
BMP->Assign(bmp);
delete bmp;
}

//---------------------------------------------------------------------------
// Рисование топографии спектрограмы
void SpecterWork::DrawSpectrograms3(TBitmap *BMP, int scale) {
if (!Loaded) return;
int sx, sy, i;
unsigned char *p, color;
double val, max;
sx = Count;
sy = N2 + 2 * N + 2;
TBitmap *bmp = new TBitmap;
bmp->PixelFormat = pf24bit;
bmp->SetSize(sx, sy);
//
switch (scale) {
	case 1: max = MaxValue; break;
	case 2: max = log10(MaxValue); break;
	case 3: max = Math::LogN(1.042, MaxValue); break;
}
// Цветовая схема
TColor left_color = clYellow, right_color = clYellow, control_color[4];
control_color[0] = (TColor)(0);
control_color[1] = (TColor)(255);
control_color[2] = (TColor)(255 << 8);
control_color[3] = (TColor)(255 << 16);
double values[4] = {0., max / 3, 2. * max / 3, max};
TColorSceme *ColorSceme = new TColorSceme(left_color, right_color, 4, control_color, values, 1000);
//
unsigned char r, g, b;
for (auto y = 0; y < N2; y++) {
	p = (unsigned char *)bmp->ScanLine[y];
	i = 0;
	for (auto x = 0; x < sx; x++) {
		val = GetSpecterPointValue(x, N2 - y - 1, scale);
		ColorSceme->Get(val, r, g, b);
		p[i++] = r;
		p[i++] = g;
		p[i++] = b;
	}
}
// Re
ReMin = 1e8;
ReMax = -1e8;
ImMin = 1e8;
ImMax = -1e8;
for (auto i = 0; i < N * Count; i++) {
	if (Re[i] > ReMax) ReMax = Re[i];
	if (Re[i] < ReMin) ReMin = Re[i];
	if (Im[i] > ImMax) ImMax = Im[i];
	if (Im[i] < ImMin) ImMin = Im[i];
}
// Цветовая схема
TColor control_color2[3];
control_color2[0] = (TColor)(255);
control_color2[1] = (TColor)(255 << 8);
control_color2[2] = (TColor)(255 << 16);
double values2[3] = {ImMin, 0., ImMax};
TColorSceme *ColorSceme2 = new TColorSceme(clYellow, clYellow, 3, control_color2, values2, 500);
//
for (auto y = 0; y < N; y++) {
	p = (unsigned char *)bmp->ScanLine[N2 + y + 1];
	i = 0;
	for (auto x = 0; x < sx; x++) {
		val = Re[N * x + N - 1 - y];
		ColorSceme2->Get(val, r, g, b);
		p[i++] = r;
		p[i++] = g;
		p[i++] = b;
	}
}
// Im
for (auto y = 0; y < N; y++) {
	p = (unsigned char *)bmp->ScanLine[N2 + N + y + 2];
	i = 0;
	for (auto x = 0; x < sx; x++) {
		val = Im[N * x + N - 1 - y];
		ColorSceme2->Get(val, r, g, b);
		p[i++] = r;
		p[i++] = g;
		p[i++] = b;
	}
}
BMP->Assign(bmp);
delete bmp;
//ShowMessage("ReMin=" + UnicodeString(ReMin) + " ReMax=" + UnicodeString(ReMax) + " ImMin=" + UnicodeString(ImMin) + " ImMax=" + UnicodeString(ImMax));
}

//---------------------------------------------------------------------------
// Рисование 3D спектрограмы
void SpecterWork::DrawSpectrograms3D(TBitmap *BMP, int scale) {
if (!Loaded) return;
int sx, sy, dx = 20, dy = 5, h = 200, x, y;
unsigned char *p, color;
double val, max;
sx = Count * dx + N2;
sy = N2 + h;
TBitmap *bmp = new TBitmap;
bmp->PixelFormat = pf24bit;
bmp->SetSize(sx,sy);
switch (scale) {
	case 1: max = MaxValue; break;
	case 2: max = log10(MaxValue); break;
	case 3: max = Math::LogN(1.042, MaxValue); break;
}
bmp->Canvas->Pen->Color = (TColor)(255 << 16);
for (auto i = 0; i < Count; i++) {
	x = i * dx;
	y = (int)(-GetSpecterPointValue(i, 0, scale) * h / max + sy - 1);
	bmp->Canvas->MoveTo(x, y);
	for (auto j = 1; j < N2; j++) {
		/*x = i * dx + j * dy;
		y = (int)(-GetSpecterPointValue(i, j, scale) * h / max + sy - 1 - dy * j);*/
		x = i * dx + j;
		y = (int)(-GetSpecterPointValue(i, j, scale) * h / max + sy - 1 - j);
		bmp->Canvas->LineTo(x, y);
	}
}
BMP->Assign(bmp);
delete bmp;
}
//---------------------------------------------------------------------------
// Сохранение главных спектров
bool SpecterWork::SaveMain(UnicodeString fname) {
	if (Main == NULL) return false;
	FILE *fd = _wfopen(fname.w_str() ,L"wb");
	fwrite(Main, sizeof(Main), N2 * MainCount, fd);
	fclose(fd);
	return true;
}
//---------------------------------------------------------------------------
// Загрузка главных спектров
bool SpecterWork::LoadMain(UnicodeString fname) {
	if (Main == NULL) return false;
	FILE *fd = _wfopen(fname.w_str() ,L"rb");
	fread(Main, sizeof(Main), N2 * MainCount, fd);
	fclose(fd);
	return true;
}
//---------------------------------------------------------------------------
// Сравнение главных спектров
double SpecterWork::Compare(SpecterWork *sw, int scale) {
	double dif = 0.;
	UnicodeString s;
	for (auto i = 0; i < MainCount; i++) {
		//for (auto j = 0; j < N2; j++) {
		for (auto j = 0; j < 20; j++) {
			dif += fabs(GetSpecterValue(Main[i * N2 + j], scale) - sw->GetSpecterValue(sw->Main[i * N2 + j], scale));
		}
	}
	return dif / (N2 * MainCount);
}
//---------------------------------------------------------------------------
int SpecterWork::GetLogN(int n) {
	switch (n) {
		case   256: return 8;
		case   512: return 9;
		case  1024: return 10;
		case  2048: return 11;
		case  4096: return 12;
		case  8192: return 13;
		default:    return 0;
	}
}
//---------------------------------------------------------------------------
// Вычисление усреднённых значений
void SpecterWork::CountMiddleValues(int pos, int n, int pts, double *min, double *max, double *middle) {
    double v;
    // Обнуляем значения
    for (auto i = 0; i < pts; i++) {
        min[i] = 1e8;
        max[i] = 0.;
        middle[i] = 0.;
    }
    // Цикл по спектрам
    for (auto j = pos; j < pos + n; j++) {
        for (auto i = 0; i < pts; i++) {
        	v = Data[1LL * j * N2 + i];
            if (v < min[i]) min[i] = v;
	        if (v > max[i]) max[i] = v;
	        middle[i] += v;
        }
    }
    // Находим средние значения
    for (auto i = 0; i < pts; i++) {
        middle[i] /= n;
    }
}
//---------------------------------------------------------------------------
/*
Вычисление позиций процентов
Дано: A[n] - спектр
percent[k] - массив интересующих процентов, например percent[9] = {10, 20, 30, 40, 50, 60, 70, 80, 90}
Результат: pos[k] - массив значений позиций начиная слева
*/
void SpecterWork::GetPercentsPos(double *A, int n, double *percent, int *pos, int k) {
    double sum = 0, sum2 = 0.;
    int j = 0;
    // Находим сумму значений
    for (auto i = 0; i < n; i++) sum += A[i];
    // Ищем позиции
    for (auto i = 0; i < n; i++) { // Повторный цикл
    	sum2 += A[i]; // Новая сумма
        while (sum2 * 100. / sum >= percent[j]) { // Если сумма >= очередного процента
            pos[j++] = i;   // Нашли очередную точку процента
            if (j >= k) return; // Нашли все точки
        }
    }
}
//---------------------------------------------------------------------------
// Вычисление позиций вертикальных тиков
void SpecterWork::GetFreqPos(void) {
	int K;
    switch (N2) {
    	case   256: K = 84; break;
    	case   512: K = 170; break;
    	case  1024: K = 340; break;
    	case  2048: K = 682; break;
    	case  4096: K = 1364; break;
    	case  8192: K = 2730; break;
    	case 16384: K = 5460; break;
    	case 32768: K = 10920; break;
    	case 65536: K = 21840; break;
    }

	// Очистка
	for (auto i = 0; i < N2; i++) FreqLinePos[i] = 0;
    // 200, 400, 600, 800 (Гц)
	for (auto i = 1; i < 5; i++) {
    	FreqLinePos[i * 200 * K / 8000] = 1;
    }
    // 1K, 2K, 3K, ... (Гц)
	for (auto i = 1; i <= 10; i++) {
    	FreqLinePos[i * 1000 * K / 8000] = 1;
    }
    
}
//---------------------------------------------------------------------------

