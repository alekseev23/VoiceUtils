//---------------------------------------------------------------------------

#ifndef TSpecterPropsH
#define TSpecterPropsH
//---------------------------------------------------------------------------
class TSpecterProps {

public:

	double
		GlobalMax,    	// Максимум
		GlobalSum,    	// Сумма
		Max,            // Текущий относительный максимум (0 ... 1.0) относительно GlobalMax
		Sum;            // Текущая относительная сумма (0 ... 1.0) относительно GlobalSum

	int
		GlobalMiddle, 	// Центр тяжести
		Middle,         // Текущий центр тяжести (a ... b)
		N;      		// Размер массива данных

	TSpecterProps();
	~TSpecterProps();

	// Обязательный предварительный анализ всего спектра
	void GlobalAnalyze(int n, double *A);
	// Анализ части спектра
	void Analyze(int a, int b, double *A);

private:

	// Находим центр тяжести
	int FindMiddlePos(double *V, int a, int b);
};
#endif
