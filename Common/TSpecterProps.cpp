//---------------------------------------------------------------------------

#pragma hdrstop

#include "TSpecterProps.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TSpecterProps::TSpecterProps() {
}
//---------------------------------------------------------------------------
TSpecterProps::~TSpecterProps() {

}
//---------------------------------------------------------------------------
// Обязательный предварительный анализ всего спектра
void TSpecterProps::GlobalAnalyze(int n, double *A) {
	GlobalMax = GlobalSum = 0.;
	N = n;
	for (auto i = 0; i < N; i++) {
		if (A[i] > GlobalMax) GlobalMax = A[i];
		GlobalSum += A[i];
	}
	GlobalMiddle = Middle = FindMiddlePos(A, 0, N - 1);
	Max = 1.;
    Sum = 1.;
}

//---------------------------------------------------------------------------
// Анализ части спектра
void TSpecterProps::Analyze(int a, int b, double *A) {
	Max = Sum = 0.;
	for (auto i = a; i <= b; i++) {
		if (A[i] > Max) Max = A[i];
		Sum += A[i];
	}
	if (Max == 0.) {
		Max = 0.;
		Sum = 0.;
	}
	else {
		Max = Max / GlobalMax;
		Sum = Sum / GlobalSum;
	}
	Middle = FindMiddlePos(A, a, b);
}
//---------------------------------------------------------------------------
// Находим центр тяжести
int TSpecterProps::FindMiddlePos(double *V, int a, int b) {
	double left = 0., right = 0.;
	int aa, bb;
	aa = a;
	bb = b;
	while (aa < bb) {
		if (left <= right) left += V[aa++];
		else right += V[bb--];
	}
	return aa;
}
//---------------------------------------------------------------------------

