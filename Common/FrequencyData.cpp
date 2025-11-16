#include "FrequencyData.h"
#include "FFT.h"
#include "math.h"
#include <iostream>

using namespace std;

__fastcall FrequencyData::FrequencyData(int n) {
	N = n;
	N2 = N / 2;
	Data = new double [N2];
    Shape = new double [N];
    setShape();
}

__fastcall FrequencyData::~FrequencyData() {
    delete[] Data;
    delete[] Shape;
}

void __fastcall FrequencyData::setShape(void) {
    applyShape(0);
}

void __fastcall FrequencyData::applyShape(int shape_type) {
    int i, k;
    k = 5 * N / 100; // Для трапецевидного шейпера
    for (i = 0; i < N; i++) {
        switch (shape_type) {
            case 0: // Прямоугольный
                    Shape[i] = 1.; break;
            case 1: // Трапецевидный шейпер
                    if (i < k) Shape[i] = 1. * i / k;
			        else if (i > N - k) Shape[i] = 1. * (N - i) / k;
			        else Shape[i] = 1.;
                    break;
            case 2: // Нормальный Гауссиан
			        Shape[i] = exp(-pow(1. * i - N2, 2) / (2. * N2 * N2 / 50));
                    break;
            case 3: // Узкий Гауссиан
			        Shape[i] = exp(-pow(1. * i - N2, 2) / (2. * N2 * N2 / 100));
                    break;
            case 4: // Широкий Гауссиан
			        Shape[i] = exp(-pow(1. * i - N2, 2) / (2. * N2 * N2 / 25));
                    break;
        }
    }
}

// Задать данные
void __fastcall FrequencyData::set(unsigned char *data, int n) {
	delete[] Data;
    N = n;
    N2 = N / 2;
	Data = new double [N2];
    Shape = new double [N];
    setShape();
	for (int i = 0; i < N2; i++) Data[i] = data[i];
}

// Перевод данных в строку
// max = 4.0
// str[N2]
void __fastcall FrequencyData::toString(double max, unsigned char *str) {
	int val;
    // Автонормировка
	if (max <= 0.) {
        max = 0.;
	    for (int i = 0; i < N2; i++) {
    	    if (Data[i] >= max) max = Data[i];
        }
	}
    //
    for (int i = 0; i < N2; i++) {
        val = (Data[i] * 255 / max);
        if (val > 255) str[i] = 255;
        else str[i] = (unsigned char)val;
    }
}

// Перевод данных в строку используя поколоночные нормировочные значения
// str[N2]
void __fastcall FrequencyData::toString2(unsigned char *str) {
    int val;
    double *FreqMax;
    switch (N) {
        case  512: FreqMax = FreqMax1; break;
        case 1024: FreqMax = FreqMax2; break;
        case 2048: FreqMax = FreqMax3; break;
        case 4096: FreqMax = FreqMax4; break;
        case 8192: FreqMax = FreqMax5; break;
        default: return;
    }
	for (int i = 0; i < N2; i++) {
        val = (int)(255. * Data[i] / FreqMax[i]);
        if (val > 255) val = 255;
        str[i] = val;
    }
}

// Нормируем значения на max_val
// a[n]
void __fastcall FrequencyData::normalize(double *a, double max_val, int n) {
    double max = 0., k;
    int i;
    // Ищем максимум
    for (i = 0; i < n; i++) {
    	if (a[i] > max) max = a[i];
    }
    if (max > 0.) {
        // Нормируем
        k = max_val / max;
        for (i = 0; i < n; i++) {
            a[i] = k * a[i];
        }
    }
}

// Переводим значения в логарифмический масштаб
// f[n]
void __fastcall FrequencyData::LogScale(double *f, int n) {
    for (int i = 0; i < n; i++) {
    	if (f[i] <= 0.) f[i] = 0.;
        else f[i] = log10(f[i]);
    }
}

// Сравнение двух массивов d[n] и Data[n]
double __fastcall FrequencyData::compare(double *d, int n) {
    double diff = 0.;
    for (int i = 0; i < n; i++) {
        diff += fabs(Data[i] - d[i]);
    }
    return diff / n;
}

// Сглаживание значений массива
// values[n]
void __fastcall FrequencyData::smoothArray(double *values, int n, int radius) {
    int i, j, k, a;
    double *t = new double[n];
    k = 2 * radius + 1;
    for (i =  0; i < n; i++) {
        t[i] = 0.;
        for (j = i - radius; j <= i + radius; j++){
            a = j;
            if (a < 0) a = 0;
            else if (a >= n) a = n - 1;
            t[i] += values[a];
        }
    }
    for (i = 0; i < n; i++) {
        values[i] = t[i] / k;
    }
    delete[] t;
}

// Обрезаем частоты ниже 250 Гц и выше 8 кГц
void __fastcall FrequencyData::cut(void) {
	int i;
    switch (N2) {
        case   256: A = 3; B = 84; break;
        case   512: A = 5; B = 170; break;
        case  1024: A = 10; B = 340; break;
        case  2048: A = 21; B = 682; break;
        case  4096: A = 42; B = 1364; break;
        case  8192: A = 85; B = 2730; break;
        case 16384: A = 170; B = 5460; break;
        case 32768: A = 340; B = 10920; break;
        case 65536: A = 680; B = 21840; break;
    }
    // Занудяем значения до A и после B
    for (i = 0; i < A; i++)  Data[i] = 0.;
    for (i = B + 1; i < N2; i++)  Data[i] = 0.;
}

// Выровнять частоты
void __fastcall FrequencyData::equalizeFrequencies(void) {
    for (int i = 0; i < N2; i++) {
		Data[i] = Data[i] * (1. + 1. * i / N2);
        //Data[i] = Data[i] * (1. + 2. * i / N2);
    }
}

// Вычислить частотные значения из данных в открытом файле
// buf - массив типа short с аудиоданными
// pos - начальный индекс
void __fastcall FrequencyData::calculate(short *buf, int pos) {
    int i;
	double *a, *f;
    a = new double [N];
    f = new double [N];
    //for (i = 0; i < N; i++) a[i] = 1. * buf[pos + i];
    for (i = 0; i < N; i++) a[i] = Shape[i] * buf[pos + i];
    // Нормируем значения
    //normalize(a, 1., n); // Давайте пока без нормализации!
    // Находим частоты
    FFTAnalysis(a, f, N, N);
    for (i = 0; i < N2; i++) Data[i] = f[i];
    delete[] a;
    delete[] f;
}

// Вычислить частотные значения из данных в открытом файле
// buf - массив типа short с аудиоданными
// buf_size - размер массива buf[buf_size]
// pos - индекс центра
// len - длина области (должна быть <= N)
void __fastcall FrequencyData::calculate(short *buf, int buf_size, int pos, int len) {
    int i, j;
	double *a, *f;
    a = new double [N];
    f = new double [N];
    for (i = 0; i < N; i++) {
        j = i + pos - N / 2;
        // Выход за границы исходного массива
        if ((j < 0) || (j >= buf_size)) a[i] = 0.;
        // Выход за границы области (len)
        else if ((i < N / 2 - len / 2) || (i > N / 2 + len / 2)) a[i] = 0.;
    	else a[i] = Shape[i] * buf[j];
    }
    // Находим частоты
    FFTAnalysis(a, f, N, N);
    for (i = 0; i < N2; i++) Data[i] = f[i];
    delete[] a;
    delete[] f;
}

// Вычислить частотные значения из данных в открытом файле
// buf - массив типа short с аудиоданными
// pos - начальный индекс
void __fastcall FrequencyData::normalizeAndCalculate(short *buf, int pos, int max) {
    int i;
	double *a, *f;
    a = new double [N];
    f = new double [N];


    for (i = 0; i < N; i++) a[i] = Shape[i] * buf[pos + i];
    //for (i = 0; i < N; i++) a[i] = Shape[i] * 32767. * buf[pos + i] / max;
    //for (i = 0; i < N; i++) a[i] = 32767. * buf[pos + i] / max;
    // Находим частоты
    FFTAnalysis(a, f, N, N);
    for (i = 0; i < N2; i++) Data[i] = f[i];
    delete[] a;
    delete[] f;
}

// Вычислить частотные значения из данных в открытом файле
// buf - массив типа short с аудиоданными
// pos - начальный индекс
// radius - кол-во слева и справа, которые мы высчитываем для статистики
void __fastcall FrequencyData::calculateeWithStat(short *buf, int pos, int radius) {
    int i, j, counter = 0, max;
    double *f = new double [N2];
    for (i = 0; i < N2; i++) f[i] = 0.;
    // Находим максимум исходных значений
    //max = findMax(buf, pos, N);
	// Несколько циклов для сглаживания
    for (i = pos; i < pos + 1; i++) {
	//for (i = pos - radius; i <= pos + radius; i += radius) {
	//for (i = pos - radius; i <= pos + radius; i++) {
	//for (i = pos - radius; i <= pos + radius;  i += 10) {
		j = i;
		if (j < 0) j = 0;
		//calculate(buf, j);
		//normalizeAndCalculate(buf, j, max);
		normalizeAndCalculate(buf, j, 32767);
		for (j = 0; j < N2; j++) f[j] += Data[j];
		counter++;
	}
	// Сглаживаем полученные данные с радиусом = 2
	//smoothArray(f, N2, 2);
	//smoothArray(f, N2, SMOOTH_RADIUS);
	// Усредняем значения
	for (j = 0; j < N2; j++) Data[j] = f[j] / counter;
	// Делаем обрезку
	//cut();
	// Выравнивание частот
    //equalizeFrequencies();
    delete[] f;
}

// Найти максимум на отрезке
// buf - массив типа short с аудиоданными
// pos - начальный индекс
// len - кол-во элементов в наборе
int __fastcall FrequencyData::findMax(short *buf, int pos, int len) {
    int i, max = 0, a, b;
    a = pos;
    b = pos + len;
    if (a < 0) a = 0;
    if (b < 0) b = 0;
    for (i = a; i < b; i++) {
        if (abs(buf[i]) > max) max = abs(buf[i]);
    }
    return max;
}

// Пересчитываем массив Data из амплитуд чатот в энергетический спектр
void __fastcall FrequencyData::getPower(void) {
    int i, k;
    double a, f, p;
    Min = 1e10;
    Max = 0.;
    switch (N2) {
        case   256: k = 84; break;
        case   512: k = 170; break;
        case  1024: k = 340; break;
        case  2048: k = 682; break;
        case  4096: k = 1364; break;
        case  8192: k = 2730; break;
        case 16384: k = 5460; break;
        case 32768: k = 10920; break;
        case 65536: k = 21840; break;
        default: return;
    }
    for (i = 0; i < N2; i++) {
        //f = 8000. * i / k;
        f = 1. * i / k;
        a = Data[i] / 32768.;
        Data[i] = 100000. * a * a * f * f;
        if (Data[i] > Max) Max = Data[i];
        else if (Data[i] < Min) Min = Data[i];
    }
}

//------------------------------------------------------------------------------

