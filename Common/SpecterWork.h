//---------------------------------------------------------------------------

#ifndef SpecterWorkH
#define SpecterWorkH
//---------------------------------------------------------------------------
#include "../Common/Envelope.h"

class SpecterWork {

public:
	UnicodeString FileName;
	double
		*Data = NULL,   // Значения спектров
		*Middle = NULL, // Средние значения спектров
		*Main = NULL,   // Основные 5 спектров (усреднённые)
		MaxValue;       // Максимальное значение точки спетра

	double
		*Re = NULL,
		*Im = NULL;
    double ReMin, ReMax, ImMin, ImMax;
	int
		N,              // Кол-во точек для вычисления спектра
		N2,             // N2 = N / 2
		BufSize,        // Кол-во точек аудиоданных
		Count,          // Кол-во спектров (зависит от длины аудио данных)
        MainCount = 5;  // Кол-во основных спектров
	short *Buf = NULL;  // Аудио данные
	Envelope *Env;      // Огибающая
	bool Loaded;        // Флаг загрузки данных

	SpecterWork();
	~SpecterWork();

	// Чтение данных из WAV файла
	bool ReadData(UnicodeString filename);

	// Чтение данных из WAV файла
	bool ReadDataFromWAV(UnicodeString filename);

	// Чтение данных из DAT файла
	bool ReadDataFromDAT(UnicodeString filename);

	// Вычисление спектров (n - Кол-во точек для анализа, например 512)
	// specters_in_second - кол-во спектров в секунду
	void CountSpecters(int n, int specters_in_second);

	// Вычисляем основные спектры
	void CountMainSpecters(int main_count);

	// Из обычных вычисляем энергетические спектры
	void GetEnergySpecter(double *specter1, double *specter2);

		//  Deprikated
		void CountSpecters(int n);
		void CountSpecters(int n, bool energy_specters);
		void CountSpecters4X(int n, bool energy_specters);

		// Вычисление всех спектров
		void CountAllSpecters(int n);
		void CountAllSpecters(int n, bool energy_specters);
		void CountAllSpecters4X(int n, bool energy_specters);

	// Получение спектров в массив Middle[N2]
	void GetMiddle(int type, int scale);

	// Получение спектров в массив Middle[N2]
	void GetMiddle(int scale, int start, int num);

	double GetSpecterPointValue(int specter, int i, int scale);
	double GetSpecterValue(double val, int scale);

    void DrawSpectrograms(TBitmap *BMP, int scale);
	void DrawSpectrograms2(TBitmap *BMP, int scale);
	void DrawSpectrograms3(TBitmap *BMP, int scale);

	void DrawSpectrograms3D(TBitmap *BMP, int scale);

	// Ищет номер спектра с макимальной суммой нижних частот
	int FindBest(void);

	// Сохранение главных спектров
	bool SaveMain(UnicodeString fname);

	// Загрузка главных спектров
	bool LoadMain(UnicodeString fname);

	// Сравнение главных спектров
	double Compare(SpecterWork *sw, int scale);

	// Нормировка аудиоданных
    void Normalize(void);

};
#endif
