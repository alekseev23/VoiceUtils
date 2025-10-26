//---------------------------------------------------------------------------

#pragma hdrstop

#include "TCompareSpecters.h"
#include <System.Math.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)

TCompareSpecters::TCompareSpecters() {

}
TCompareSpecters::~TCompareSpecters() {

}

// Находим максимальное значение массива A[n]
double TCompareSpecters::GetMaxValue(int n, double *A) {
	double max = 0.;
	for (auto i = 0; i < n; i++) {
		if (A[i] > max) max = A[i];
	}
	return max;
}

// Находим сумму массива A[n]
double TCompareSpecters::GetSumValue(int n, double *A) {
	double sum = 0.;
	for (auto i = 0; i < n; i++) {
		sum += A[i];
	}
	return sum;
}

// Находим разницу массивов m1 * A1[n] и m2 * A2[n]
double TCompareSpecters::GetDif(int n, double *A1, double *A2, double m1, double m2) {
	double dif = 0.;
	for (auto i = 0; i < n; i++) {
		dif += fabs(m1 * A1[i] - m2 * A2[i]);
	}
	return dif;
}

//---------------------------------------------------------------------------

/* Изменение значений массива A[n] в зависимости от scale
scale = 1 - линейная шкала
scale = 2 - десятичный логарифм
scale = 3 - логарифм по основанию 1.042
*/
void TCompareSpecters::SetSpecterValue(int n, double *A, int scale) {
	if (scale == 1) return; // Линейная шкала
	for (auto i = 0; i < n; i++) {
		if (A[i] < 0.) A[i] = 0.;
		else {
			if (scale == 2) A[i] = log10(A[i]);
			else if (scale == 3) A[i] = Math::LogN(1.042, A[i]);
			if (A[i] < 0.) A[i] = 0.;
		}
	}
}
//---------------------------------------------------------------------------

/* Изменение значений массива A[n] в массив B[n] в зависимости от scale
scale = 1 - линейная шкала
scale = 2 - десятичный логарифм
scale = 3 - логарифм по основанию 1.042
*/
void TCompareSpecters::SetSpecterValue(int n, double *A, double *B, int scale) {
	if (scale == 1) return; // Линейная шкала
	for (auto i = 0; i < n; i++) {
		if (scale == 1) {
			B[i] = A[i];
			continue;
		}
		if (A[i] < 0.) B[i] = 0.;
		else {
			if (scale == 2) B[i] = log10(A[i]);
			else if (scale == 3) B[i] = Math::LogN(1.042, A[i]);
			if (B[i] < 0.) B[i] = 0.;
		}
	}
}
