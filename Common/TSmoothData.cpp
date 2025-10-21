//---------------------------------------------------------------------------

#pragma hdrstop

#include "TSmoothData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

/*
//__fastcall TSmoothData::TSmoothData() {
template<> __fastcall TSmoothData<class T>::TSmoothData() {
}

template<> __fastcall TSmoothData<class T>::~TSmoothData() {
    Clear();
}

// Очистка данных
template<> void __fastcall TSmoothData<class T>::Clear(void) {
	if (Data != NULL) delete[] Data;
    M = N = 0;
}

// Задание размеров данных
template<> void __fastcall TSmoothData<class T>::SetSize(int m, int n) {
    Clear();
    M = m;
    N = n;
    Data = new T [M * N];
}

// Добавление строки данных
void __fastcall TSmoothData::AddLine(int line, T *f) {
    int i, pos;
    pos = line * M;
    for (i = 0; i < M; i++) Data[i] = f[i];
}

// Чтение строки данных
void __fastcall TSmoothData::ReadLine(int line, T *f) {
    int i, pos;
    pos = line * M;
    for (i = 0; i < M; i++) f[i] = Data[pos++];
}

// Сглаживание по N
void __fastcall TSmoothData::SmoothColumns(int radius) {
    for (int i = 0; i < M; i++) SmoothColumnElements(i, radius);
}

// Сглаживание по M всех строк
void __fastcall TSmoothData::SmoothLines(int radius) {
    for (int i = 0; i < N; i++) SmoothElements(i, radius);
}

// Сглаживание элементов конкретной строки
void __fastcall TSmoothData::SmoothLineElements(int line, int radius) {
    int count, pos;
    double sum;
    T *data2 = new T [M];
    // Начало данных строки
    pos = line * M;
    // Дублируем данные в строку
    for (int i = 0; i < M; i++) data2[i] = Data[pos + i];
    //
    for (int i = 0; i < M; i++) {
        count = 0;
        sum = 0;
        for (int j = i - radius; j <= i + radius; j++) {
            if (j < 0) continue;
            if (j >= M) break;
            sum += data2[pos + j];
            count++;
        }
        Data[i] = data2[i] / count;
    }
    delete[] data2;
}

// Сглаживание элементов конкретной колонки
void __fastcall TSmoothData::SmoothColumnElements(int column, int radius) {
    int counts;
    double sum;
    T *data2 = new T [N];
    // Дублируем данные в массив
    for (int i = 0; i < N; i++) data2[i] = Data[column + i * M];
    //
    for (int i = 0; i < N; i++) {
        count = 0;
        sum = 0;
        for (int j = i - radius; j <= i + radius; j++) {
            if (j < 0) continue;
            if (j >= N) break;
            sum += data2[column + j * M];
            count++;
        }
        Data[column + i * M] = data2[i] / count;
    }
    delete[] data2;
}


// Сглаживание сперва по M потом по N
void __fastcall TSmoothData::SmoothMN(int radiusM, int radiusN) {
    SmoothColumns(radiusM);
    SmoothLines(radiusN);
}

// Сглаживание сперва по N потом по N
void __fastcall TSmoothData::SmoothNM(int radiusN, int radiusM) {
    SmoothLines(radiusN);
    SmoothColumns(radiusM);
}
*/