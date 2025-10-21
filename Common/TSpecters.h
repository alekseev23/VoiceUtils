//---------------------------------------------------------------------------

#ifndef TSpectersH
#define TSpectersH
//---------------------------------------------------------------------------

#include <vector>

//---------------------------------------------------------------------------

class TSpecterData {

public:

    int
        N;  // Кол-во элементов массива Data
    unsigned short
    	EnvelopeAbs,    // Абсолютное значение огибающей (0 .. 32768)
        EnvelopeMax,    // Локальное макимальное значение огибающей (0 .. 1000)
        Extremum;       // Кол-во эксиремумов в спектре
    float
        Max,            // Максимальное значение из Data[]
        Sum,            // Сумма значений массива Data[]
        SourceMax,
        SourceSum,
        PowerMax,
        PowerSum,
        *SourceData = NULL, // Точки спектра
        *Data = NULL,   	// Точки спектра
        *Power = NULL;  	// Значение энергии спектра

    __fastcall TSpecterData();
    __fastcall TSpecterData(int n);
    __fastcall ~TSpecterData();

    // Изменить размерность массива Data[]
    void __fastcall ReSize(int n);

    // Передать массив с данными и параметрами
    void __fastcall Set(int n, unsigned short env_abs, unsigned short env_max, float *data);
    void __fastcall Set(int n, unsigned short env_abs, unsigned short env_max, double *data);

    // Пересчитать значения Max и Sum
    void __fastcall ReCount(void);

	// Считать значения из файла
	bool __fastcall Read(FILE *fd);
    // Сохранить значения в файл
    bool __fastcall Write(FILE *fd);

    // Нормировка значений спектра
    void __fastcall NormalizeSpecter(double norm);
    // Вычисление энергетического спектра
	void __fastcall CalculatePower(void);
    // Нормировка значений массива
    void __fastcall Normalize(float *array, int n, float val);

};

//---------------------------------------------------------------------------
class TSpecters {

public:
    unsigned short
        N,  			// Кол-во элементов массива Data
        Version = 2; 	// Версия файла
    unsigned int
        Items;      	// Кол-во спектров
    std::vector <class TSpecterData*> SpecterData;
    float
    	Level[4],
        Max,
        SourceMax,
        PowerMax,
        PowerSumMax;

    __fastcall TSpecters();
    __fastcall TSpecters(int n);
    __fastcall TSpecters(int n, int items);
    __fastcall ~TSpecters();

    // Установить кол-во элементов в спектрах
    void __fastcall Init(int n);
    // Задать кол-во спектров
	void __fastcall ReSize(int items);
    // Сохранить файл со спектрами
    bool __fastcall Save(UnicodeString fname);
    // Загрузить файл со спектрами
    bool __fastcall Load(UnicodeString fname);
    // Очистить данные
    void __fastcall Clear();
    // Добавить данные
	bool __fastcall Add(float *data, unsigned short env_abs, unsigned short env_max);
	bool __fastcall Add(double *data, unsigned short env_abs, unsigned short env_max);
    // Изменить данные
	bool __fastcall Set(int i, float *data, unsigned short env_abs, unsigned short env_max);
	bool __fastcall Set(int i, double *data, unsigned short env_abs, unsigned short env_max);
    // Нормировка
    void __fastcall NormalizeToMax(void);
    void __fastcall NormalizeToAbs(void);
    // Вычисление энергетического спектра
	void __fastcall CalculatePower(void);

    // Анализ спектров
    void __fastcall Analyze(void);

    // Получить уровень
	int GetLevel(float val);

    // Поиск максиммумов
    void FindMax(void);

};
#endif

/*

Points=
Points>
Points<
Sum=
Sum>
Sum<

EqualPoints
SamePoints
ChangePoints
Sum

*/
