//---------------------------------------------------------------------------

#pragma hdrstop

#include "TInterpolation.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


TInterpolation::TInterpolation() {

}

TInterpolation::~TInterpolation() {

}


/*
Пересчитывает значения в массив new_val с количеством точек вдвое больше!
Точек в массиве val должно быть минимум n+1 !!!
Точек в массисве new_val должно быть 2*n
*/
void TInterpolation::InterpolateX2(int n, short *val, short *new_val) {
	auto n2 = 2 * n, j = 0, a = 0;
	double X[4], Y[4];
	for (auto i = 0; i < n2; i += 2) {
		// Берём готовые значения
		new_val[i] = val[j];
		// Считаем значения
		a = j;
		if (a + 3 >= n) a = n - 4;
		if (a > 0) a--;
		for (auto k = 0; k < 4; k++) {
			X[k] = 1. * a + k;
			Y[k] = 1. * val[a + k];
		}
		new_val[i + 1] = (short)Lagrange(0.5 * (i + 1), 4, X, Y);
		j++;
	}
}

/*
Пересчитывает значения в массив new_val с количеством точек вчетверо боьше!
Точек в массиве val должно быть минимум n+1 !!!
Точек в массисве new_val должно быть 4n
*/
void TInterpolation::InterpolateX4(int n, short *val, short *new_val) {
	auto n4 = 4 * n, j = 0, a = 0;
	double X[4], Y[4];
	for (auto i = 0; i < n4; i += 4) {
		// Берём готовые значения
		new_val[i] = val[j];
		// Считаем значения
		a = j;
		if (a + 3 >= n) a = n - 4;
		if (a > 0) a--;
		for (auto k = 0; k < 4; k++) {
			X[k] = 1. * a + k;
			Y[k] = 1. * val[a + k];
		}
		new_val[i + 1] = (short)Lagrange(0.25 * (i + 1), 4, X, Y);
		new_val[i + 2] = (short)Lagrange(0.25 * (i + 2), 4, X, Y);
		new_val[i + 3] = (short)Lagrange(0.25 * (i + 3), 4, X, Y);
		j++;
	}
}

double TInterpolation::Lagrange(double x, int n, double x_arr[], double y_arr[]) {
	double sum = 0;
	for (int i = 0; i < n; i++){
		double l = 1;
		for (int j = 0; j < n; j++) {
			if (j != i) l *= (x - x_arr[j]) / (x_arr[i] - x_arr[j]);
		}
		sum += y_arr[i] * l;
	}
	return sum;
}

double TInterpolation::Newton(double x, int n, double x_arr[], double y_arr[]) {
	double sum = y_arr[0];
	for(int i = 1; i < n; ++i){
		double F = 0;
		for(int j = 0; j <= i; ++j){
			double den = 1;
			for (int k = 0; k <= i; ++k) {
				if (k != j) den *= (x_arr[j] - x_arr[k]);
			}
			F += y_arr[j]/den;
		}
		for (int k = 0; k < i; ++k) F *= (x - x_arr[k]);
		sum += F;
	}
	return sum;
}

