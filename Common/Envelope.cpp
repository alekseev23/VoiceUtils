//---------------------------------------------------------------------------

#pragma hdrstop

#include <math.h>
#include "Envelope.h"
#include "../../_Common_/Lib/files.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

// Инициализация огибающей
// samples - кол-во точек данных в одной точке огибающей
__fastcall Envelope::Envelope(int samples, int size) {
	Samples = samples;
	Size = size;
	SilencePercent = 10;
	Data = new short [Size];
    Init();
}

__fastcall Envelope::~Envelope() {
	if (Data != NULL) delete[] Data;
    if (LocalMin != NULL) delete[] LocalMin;
    if (LocalMax != NULL) delete[] LocalMax;
}

// Сбросить значения
void __fastcall Envelope::Init(void) {
	Items = 0;
	Pos = 0;
	AudioCounter = 0;
    Counter = 0;
	AudioMax = 0;
}

// Добавить значение
void __fastcall Envelope::Add(short value)  {
	Data[Pos++] = value;    // Добавляем точку
	if (Pos >= Size) Pos = 0;   // Находим новую позицию
	if (Items < Size) Items++;  // Увеличиваем кол-во элементов в буфере
	Counter++;  // Счётчик добавленных точек огибающей
}

// Получить данные огибающей
int __fastcall Envelope::GetData(unsigned int pos, short *buf, int n) {
    // Проверка границ
    if (pos < Counter - Items) return 2; // Выход за границу слева
    if (pos + n > Counter) return 3; // Выход за границу справа
    if (n > Items) return 4; // Недостаточно данных
    // Находим начальную позицию в буфере данных
    int p = Pos - (Counter - pos);
	if (p < 0 ) p += Size;
    // Копируем данные
    for (int i = 0; i < n; i++) {
        buf[i] = Data[p++];
        if (p >= Size) p = 0; // Проверяем границы буфера
    }
    return 0;
}

// Получить значение с начала
short __fastcall Envelope::Get(int i)   {
	int p;
	if ((i < 0) || (i >= Items)) return 0;
	p =  Pos - Items + i;
	if (p < 0) p += Size;
	return Data[p];
}

// Получить значение с конца
short __fastcall Envelope::GetBack(int i)   {
	int p;
	if ((i < 0) || (i >= Items)) return 0;
	p =  Pos - 1 - i;
	if (p < 0) p += Size;
	return Data[p];
}

// Находим минимум и максимум
short __fastcall Envelope::FindMinMax(void) {
	short value, min, max;
	int i, j;
	if (Items == 0) return 0; // Если нет элементов
	max = 0;
	min = 32767;
	for (i = 1; i <= Items; i++) {
		j = Pos - i;
		if (j < 0) j = Size + j;
		value = Data[j];
		if (value < min) min = value;
		if (value > max) max = value;
	}
	Min = min;
	Max = max;
	return Max;
}

// Находим минимум и максимум в окрестностях i с радиусом radius
void __fastcall Envelope::FindAllLocalMinMax(int radius, int delta) {
    // Пересоздаём массивы
    if (LocalMin != NULL) delete[] LocalMin;
    if (LocalMax != NULL) delete[] LocalMax;
    LocalMin = new short [Items];
    LocalMax = new short [Items];
    // Цикл по всем точкам огибающей
    for (int i = 0; i < Items; i++) {
        LocalMin[i] = 0;
        LocalMax[i] = 0;
        // Цикл по точкам радиуса
        for (int j = i - radius; j <= i + radius; j++) {
            if ((j >= 0) && (j < Items)) {
                if (Data[j] < LocalMin[i]) LocalMin[i] = Data[j];
                else if (Data[j] > LocalMax[i]) LocalMax[i] = Data[j];
            }
        }
        // Части слева и справа (для более гладкого перехода условного максимума огибающей)
        int val, j, k;
        for (int k = 0; k < delta; k++) {
            // Часть слева
            j = i - radius - k;
            if ((j >= 0) && (j < Items)) {
                val = Data[j] * (delta - k) / delta;
                if (val > LocalMax[i]) LocalMax[i] = val;
            }
            // Часть справа
            j = i + radius + k;
            if ((j >= 0) && (j < Items)) {
                val = Data[j] * (delta - k) / delta;
                if (val > LocalMax[i]) LocalMax[i] = val;
            }
        }
    }
}

// Находим минимум и максимум в окрестностях i с радиусом radius
short __fastcall Envelope::FindLocalMinMax(int i, int radius) {
    int a, b, j, min, max, value;
	if (Items == 0) return 0; // Если нет элементов
    if (Last == i) return Max; // Результат уже нашли ранее
	max = 0;
	min = 32767;
    a = i - radius;
    if (a < 0) a = 0;
    b = i + radius;
    if (b > Items) b = Items;
	for (j = a; j < b; j++) {
		value = Data[j];
		if (value < min) min = value;
		if (value > max) max = value;
	}
	Min = min;
	Max = max;
    Last = i;
	return Max;
}

// Добавляем аудио точку
void __fastcall Envelope::AddAudioPoint(short val) {
	if (val < 0) val = -val;
	if (val > AudioMax) AudioMax = val;
	AudioCounter++;
	if (AudioCounter >= Samples) {
		Add(AudioMax);
		AudioMax = 0;
		AudioCounter = 0;
	}
}

// Получить значение огибающей для аудио точки в прошлом
short __fastcall Envelope::GetForAudioPoint(int k) {
	if (k <= AudioCounter) return AudioMax; // Последние точки
	int i = (k - AudioCounter) / Samples;
	if ((i >= Items) || (i < 0)) return 0;
	return GetBack(i);
}

// Получить значение огибающей для аудио точки в прошлом
short __fastcall Envelope::GetForAudioPoint(int k, int pos, int items, int audio_counter) {
	int i, p;
	if (k <= audio_counter) return AudioMax; // Последние точки
	i = (k - audio_counter) / Samples;
	if ((i < 0) || (i >= Items)) return 0;
	p =  pos - 1 - i;
	if (p < 0) p += Size;
	return Data[p];
}

// Находим является ли текущая точка огибающей тишиной или паузой
// Возвращает true если значение низкое и false если высокое
bool __fastcall Envelope::CheckSilence(short val) {
	if (100 * val / Max < SilencePercent) return true;
	return false;
}

// Сохранить огибающую
bool __fastcall Envelope::Save(UnicodeString fname) {
    // Если нет данных, выходим
    if ((Items == 0) || (Data == NULL)) return false;
    // Сохраняем данные
    // Открываем файл на запись
	FILE *fd = _wfopen(fname.w_str(), L"wb");
	if (fd == NULL) return false;
    // Записываем данные
    if (fwrite(Data, 2, Items, fd) != Items) {
        fclose(fd);
    	return false;
    }
    // Закрываем файл
    fclose(fd);
    return true;
}

// Загрузить огибающую
bool __fastcall Envelope::Load(UnicodeString fname) {
	SilencePercent = 10;
	Size = getFileSize(fname);
    Items = Size / 2;
	if (Data != NULL) delete[] Data;
    if (LocalMin != NULL) {
    	delete[] LocalMin;
        LocalMin = NULL;
    }
    if (LocalMax != NULL) {
    	delete[] LocalMax;
        LocalMax = NULL;
    }
	Data = new short [Items];
    // Читаем данные
    // Открываем файл на чтение
	FILE *fd = _wfopen(fname.w_str(), L"rb");
	if (fd == NULL) return false;
    // Читаем данные
    if (fread(Data, 2, Items, fd) != Items) {
        fclose(fd);
    	return false;
    }
    // Закрываем файл
    fclose(fd);
    // Находим минимум и максимум
    FindMinMax();
    return true;
}

// Создать огибающую из DAT или WAV файла
// samples - кол-во точек данных в одной точке огибающей
//
bool __fastcall Envelope::FromFile(int samples, UnicodeString fname, int offset) {
    int i, j, n, k = 1000000, l;
    // Открываем файл на чтение
	FILE *fd = _wfopen(fname.w_str(), L"rb");
	if (fd == NULL) return false;
    // Если есть смещение, то сдвигаемся
    if (offset != 0) fseek(fd, offset, SEEK_SET);
    //
	Samples = samples;
	SilencePercent = 10;
    // Кол-во аудио точек
    n = (getFileSize(fname) - offset) / 2; // Кол-во аудио точек в файле
    Size = (n + Samples - 1) / Samples; // Кол-во точек огибающей
    short *buf = new short [k];
	if (Data != NULL) delete[] Data;
	Data = new short [Size];
    Init();
    for (i = 0; i < n; i += k) { // Цикл по всем данным
	    // Читаем данные
        l = fread(buf, 2, k, fd);
        for (j = 0; j < l; j++) AddAudioPoint(buf[j]); // Добавляем аудио точку
    }
    // Закрываем файл
    fclose(fd);
    // Находим минимум и максимум
    FindMinMax();
    // Удаляем буфер
    delete[] buf;
    return true;
}


