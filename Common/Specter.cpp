#include "Specter.h"
#include "../WhoIsSpeaking/Log.h"

extern Log *Log;

//------------------------------------------------------------------------------
__fastcall Specter::Specter() {
	Data = NULL;
}

__fastcall Specter::~Specter() {
	if (Data != NULL) delete[] Data;
}

//------------------------------------------------------------------------------
// Установка параметров
//------------------------------------------------------------------------------
void __fastcall Specter::SetParams(int n) {
	SetParams(n, 8);
}

//------------------------------------------------------------------------------
// Установка параметров
//------------------------------------------------------------------------------
void __fastcall Specter::SetParams(int n, int parts) {
	// Основные
	N = n;
	// Дополнительные
	Parts = parts;
	I = N / Parts;
	if (Data != NULL) delete[] Data;
	Data = new unsigned char [N];
}

//------------------------------------------------------------------------------
// Установка параметров
//------------------------------------------------------------------------------
void __fastcall Specter::SetDelta(int delta, int delta_column) {
	Delta = delta;
	DeltaColumn = delta_column;
	DeltaPart = Delta * I;
}

//------------------------------------------------------------------------------
// Задать в спектр значения из массива
//------------------------------------------------------------------------------
void __fastcall Specter::Set(unsigned char *data) {
	if (Data == NULL) return;
	for (int i = 0; i < N; i++) Data[i] = data[i];
}

//------------------------------------------------------------------------------
// Получить из спектра значения в массив
//------------------------------------------------------------------------------
void __fastcall Specter::Get(unsigned char *data) {
	if (Data == NULL) return;
	for (int i = 0; i < N; i++) data[i] = Data[i];
}

//------------------------------------------------------------------------------
// Сравнение спектра с массивом данных
// data[N]
// Возвращает суммарную разницу
//------------------------------------------------------------------------------
int __fastcall Specter::Compare(unsigned char *data) {
	if (Data == NULL) return -1;
	if (data == NULL) return -1;
	int dif = 0, d;
	for (int i = 0; i < N; i++) {
		// Находим разницу
		d = abs(data[i] - Data[i]);
		// Если она больше порогового значения, то добавляем штрафные баллы
		if (d > DeltaColumn) d *= 2;
		dif += d;
	}
	return dif;
}

//------------------------------------------------------------------------------
// Сравнение спектра с массивом данных
// data[N]
// Возвращает false или true
//------------------------------------------------------------------------------
/*bool __fastcall Specter::Check(unsigned char *data) {
	if (Data == NULL) return false;
	if (data == NULL) return false;
	int dif, d, i, p, n = 0;
	// Цикл по отрезкам
	for (p = 0; p < Parts; p++) {
		dif = 0;
		for (int i = 0; i < I; i++) {
			// Находим разницу
			d = abs(data[n] - Data[n]);
			// Поколоночное сравнение
			if (d > DeltaColumn) return false;
			n++;
            // Суммируем разницу
			dif += d;
		}
		if (dif > DeltaPart) return false;
	}
	return true;
}*/

bool __fastcall Specter::Check(unsigned char *data) {
	if (Data == NULL) return false;
	if (data == NULL) return false;
	int dif = 0, d, i;
	for (int i = 0; i < N; i++) {
		// Находим разницу
		d = abs(data[i] - Data[i]);
		// Поколоночное сравнение
		if (d > DeltaColumn) return false;
        // Суммируем разницу
		dif += d;
	}
	if (dif > Delta * N) return false;
	return true;
}
//------------------------------------------------------------------------------
// Сравнение спектра с массивом данных
// data[N]
// Возвращает false или true
//------------------------------------------------------------------------------
bool __fastcall Specter::FastCheck(unsigned char *data) {
	if (Data == NULL) return false;
	if (data == NULL) return false;
	int dif, d, i, p, n = 0;
	// Цикл по отрезкам
	for (p = 0; p < Parts; p++) {
		dif = 0;
		for (int i = 0; i < I; i++) {
			// Находим разницу
			d = abs(data[n] - Data[n]);
			n++;
			// Если она больше порогового значения, то добавляем штрафные баллы
			if (d > DeltaColumn) d *= 2;
			dif += d;
		}
		if (dif > DeltaPart) return false;
	}
	return true;
}

//------------------------------------------------------------------------------
// Чтение данных из файла
//------------------------------------------------------------------------------
bool __fastcall Specter::Read(FILE *fd) {
	if (Data == NULL) return false;
	if (fread(Data, 1, N, fd) != N) return false;
	return true;
}

//------------------------------------------------------------------------------
// Запись данных в файл
//------------------------------------------------------------------------------
bool __fastcall Specter::Write(FILE *fd) {
	if (Data == NULL) return false;
	if (fwrite(Data, 1, N, fd) != N) return false;
	return true;
}

//------------------------------------------------------------------------------
// Копирование данных в другой экземпляр
//------------------------------------------------------------------------------
void __fastcall Specter::CopyTo(Specter &specter) {
	specter.SetParams(N, Parts);
	specter.SetDelta(Delta, DeltaColumn);
	for (int i = 0; i < N; i++) specter.Data[i] = Data[i];
}

//------------------------------------------------------------------------------
// Копирование данных из другого экземпляра
//------------------------------------------------------------------------------
void __fastcall Specter::CopyFrom(Specter &specter) {
	SetParams(specter.N, specter.Parts);
	SetDelta(specter.Delta, specter.DeltaColumn);
	for (int i = 0; i < N; i++) Data[i] = specter.Data[i];
}

