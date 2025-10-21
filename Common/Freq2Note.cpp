//---------------------------------------------------------------------------

#pragma hdrstop

#include "Freq2Note.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall Freq2Note::Freq2Note() {
    Points = FREQ_POINTS;
    for (int i = 0; i <= Points; i++) {
        if (i == 0) {
            R[0] = F[0] - (F[1] - F[0]) / 2; // Самая левая граница
        }
        else if (i == Points) {
            R[Points] = F[i - 1] + (F[i - 1] - F[i - 2]) / 2; // Самая правая граница
        }
        else {
            R[i] = (F[i] + F[i - 1]) / 2;
        }
    }
    int i;
    for (i = 0; i < 256; i++) Pos256[i] = Get(i, 256);
    for (i = 0; i < 512; i++) Pos512[i] = Get(i, 512);
    for (i = 0; i < 1024; i++) Pos1024[i] = Get(i, 1024);
    for (i = 0; i < 2048; i++) Pos2048[i] = Get(i, 2048);
    for (i = 0; i < 4096; i++) Pos4096[i] = Get(i, 4096);
}

__fastcall Freq2Note::~Freq2Note() {
}

// Возвращает номер ноты (0 - 72)
unsigned char __fastcall Freq2Note::Get(double freq) {
    //if (freq < R[0]) return -1;
    if (freq < R[0]) return 0;
    if (freq > R[Points]) return Points - 1;
    for (int i = 0; i < Points; i++) {
        if (freq < R[i + 1]) return (unsigned char)i;
    }
}

// Возвращает номер ноты (0 - 116)
// n - (columns)
// i - текущая колонка
unsigned char __fastcall Freq2Note::Get(int i, int n) {
    double freq;
    switch (n) {
	    case  256: freq = 8000. * i / 84; break;
	    case  512: freq = 8000. * i / 170; break;
	    case 1024: freq = 8000. * i / 340; break;
	    case 2048: freq = 8000. * i / 682; break;
	    case 4096: freq = 8000. * i / 1364; break;
    }
    return Get(freq);
}

void __fastcall Freq2Note::Calculate(int n, unsigned char *data, unsigned char *res) {
    int i, j, k, l;
    int *Pos;
	// Задаём границы
    switch (n) {
        case  256: a = 2; b = 84; Pos = Pos256; break;
        case  512: a = 5; b = 170; Pos = Pos512; break;
        case 1024: a = 10; b = 340; Pos = Pos1024; break;
        case 2048: a = 20; b = 682; Pos = Pos2048; break;
        case 4096: a = 42; b = 1364; Pos = Pos4096; break;
    }
	// Обнуляем значения
    A = Pos[a];
    B = Pos[b];
    k = B - A + 1;
    for (i = 0; i < k; i++) res[i] = 0;
	// Цикл по точкам
    for (i = a; i <= b; i++) {
        l = Pos[i] - Pos[a];
        if (data[i] > res[l]) res[l] = data[i];
    }
}

void __fastcall Freq2Note::Calculate2(int n, unsigned char *data, unsigned char *res) {
    int i, j, k, l;
    int *Pos;
	// Задаём границы
    switch (n) {
        case  256: a = 2; b = 84; Pos = Pos256; break;
        case  512: a = 5; b = 170; Pos = Pos512; break;
        case 1024: a = 10; b = 340; Pos = Pos1024; break;
        case 2048: a = 20; b = 682; Pos = Pos2048; break;
        case 4096: a = 42; b = 1364; Pos = Pos4096; break;
    }
	// Обнуляем значения
    A = Pos[a];
    B = Pos[b];
    for (i = 0; i < n; i++) res[i] = 0;
	// Цикл по точкам
    for (i = a; i <= b; i++) {
        if ((data[i] > data[i - 1]) && (data[i] > data[i + 1])) { // Точка максимума
	        l = Pos[i] - Pos[a]; // Номер ноты
    	    res[l] = data[i]; // Присваиваем значение в найденную ноту
        }
    }
}

void __fastcall Freq2Note::CalculateAndMove(int n, unsigned char *data) {
    unsigned char *res = new unsigned char [FREQ_POINTS];
    Calculate(n, data, res);
    for (int i = 0; i < FREQ_POINTS; i++) data[i] = res[i];
    delete[] res;
}

void __fastcall Freq2Note::CalculateAndMove2(int n, unsigned char *data) {
    unsigned char *res = new unsigned char [FREQ_POINTS];
    Calculate2(n, data, res);
    for (int i = 0; i < FREQ_POINTS; i++) data[i] = res[i];
    delete[] res;
}

