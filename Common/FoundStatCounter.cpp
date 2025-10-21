//---------------------------------------------------------------------------

#pragma hdrstop

#include "FoundStatCounter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall FoundStatCounter::FoundStatCounter(int voices, int specters, int types, int times) {
    // Запоминаем основные параметры
    Voices = voices;
    Specters = specters;
    Types = types;
    Times = times;
    // Создаём массив
    Data = new unsigned char [Voices * Specters * Types * Times];
    // Очищаем данные
    Clear();
}

__fastcall FoundStatCounter::~FoundStatCounter() {
    // Удаляем массив
    delete[] Data;
}

// Получить индекс элемента
int __fastcall FoundStatCounter::GetIndex(int voice, int specter, int type, int time) {
    int index;
    index = voice * (Specters * Types * Times) + specter * (Types * Times) + type * Times + time;
    return index;
}

// Отметить найденный спектр
void __fastcall FoundStatCounter::Set(int voice, int specter, int type) {
    int i, t;
    // Находим индекс начального элемента
    i = GetIndex(voice, specter, type, 0);
    // Цикл по временам
    for (t = 0; t < Times; t++) {
        // Если значение не перешло границы, то увеличиваем его
        if (Data[i] < 255) Data[i] += 1;
        // Переходим к следующему элементу
        i++;
    }
}

// Подсчитать кол-во разных спектров за указанный период времени
int __fastcall FoundStatCounter::GetSpecters(int voice, int type, int time_period) {
    int i, n, p, res = 0;
    p = GetIndex(voice, 0, type, time_period);
    n = Types * Times;
    for (i = 0; i < Specters; i++) {
        if (Data[p] > 0 ) res++;
        p += n;
    }
    //res = random(10);
    return res;
}

// Очистить данные
void __fastcall FoundStatCounter::Clear(void) {
    int i, n;
    n = Voices * Specters * Types * Times;
    for (i = 0; i < n; i++) Data[i] = 0;
}

// Переход к следующей секунде
void __fastcall FoundStatCounter::Next(void) {
    int i, t, n, p;
    n = Voices * Specters * Types;
    for (i = 0; i < n; i++) {
        p = i * Times;
        // Переносим значения вправо (Data[0] - это последняя секунда)
	    for (t = Times - 1; t > 0; t--) {
            Data[p + t] = Data[p + t - 1];
        }
        Data[p] = 0; // Обнуляем самое первое значение
    }
}


