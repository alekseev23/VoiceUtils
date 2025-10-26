//---------------------------------------------------------------------------

#ifndef TCompareSpectersH
#define TCompareSpectersH
//---------------------------------------------------------------------------

class TCompareSpecters {

public:

	TCompareSpecters();
	~TCompareSpecters();

	// Находим максимальное значение массива A[n]
	double GetMaxValue(int n, double *A);

	// Находим сумму массива A[n]
	double GetSumValue(int n, double *A);

	// Находим разницу массивов m1 * A1[n] и m2 * A2[n]
	double GetDif(int n, double *A1, double *A2, double m1, double m2);

	// Изменение значений массива A[n] в зависимости от scale
	void SetSpecterValue(int n, double *A, int scale);
    void SetSpecterValue(int n, double *A, double *B, int scale);
};
#endif
