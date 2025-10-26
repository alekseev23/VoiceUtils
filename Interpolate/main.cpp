#include <vcl.h>
#include <tchar.h>

#include <iostream>
#include <cmath>
#include <iomanip>

#include "../Common/TInterpolation.h"

using namespace std;

double f(double x){
	return x*x - cos(x);
}

double Lagrange(double x, int n, double x_arr[], double y_arr[]){

	double sum = 0;
	for (int i = 0; i < n; i++){

		double l = 1;
		for (int j = 0; j < n; j++)
			if (j != i)
				l *= (x - x_arr[j]) / (x_arr[i] - x_arr[j]);
		sum += y_arr[i] * l;
	}

	return sum;
}

double Newton(double x, int n, double x_arr[], double y_arr[]){

	double sum = y_arr[0];
	for(int i = 1; i < n; ++i){

		double F = 0;
		for(int j = 0; j <= i; ++j){

			double den = 1;
			for(int k = 0; k <= i; ++k)
				if (k != j)
					den *= (x_arr[j] - x_arr[k]);
			F += y_arr[j]/den;
		}

		for(int k = 0; k < i; ++k)
			F *= (x - x_arr[k]);
		sum += F;
	}
	return sum;
}

int _tmain(int argc, _TCHAR* argv[])
{
    const int N = 4;

    double x_arr[N], y_arr[N];
    for (int i = 0; i < N; ++i){//áåðåì 4 òî÷êè îò 0 äî 0.75
        x_arr[i] = i*0.25;
        y_arr[i] = f(x_arr[i]);
    }

    cout << setw(3) << "x";
    cout << setw(12) << "x*x-cos(x)";
    cout << setw(12) << "Lagrange(x)";
    cout << setw(12) << "Newton(x)";
    cout << endl;
	cout << "---------------------------------------" << endl;

	for (double x = 0.1; x < 0.6 + 0.05; x += 0.1){

		cout << setw(3) << fixed << setprecision(1) << x;
		cout << setw(12) << fixed << setprecision(6) << f(x);
		cout << setw(12)<<fixed<<setprecision(6)<< Lagrange(x, N, x_arr, y_arr);
		cout << setw(12)<<fixed<<setprecision(6)<< Newton(x, N, x_arr, y_arr);
		cout << endl;
	}

	cout << "---------------------------------------" << endl;
	TInterpolation Interpolation;
	int M = 10;
	short X[M], Y[M], YY[M * 4];
	for (int i = 0; i < M; i++) {
		X[i] = i;
		Y[i] = (short)f(X[i]);
		cout << i << "\t" << X[i] << "\t" << Y[i] << endl;
	}
	cout << "---------------------------------------" << endl;
	Interpolation.InterpolateX4(10, Y, YY);
	for (int i = 0; i < M * 4; i++) {
		cout << i << "\t" << YY[i] << endl;
	}
}