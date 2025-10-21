//---------------------------------------------------------------------------

#ifndef EnvelopeH
#define EnvelopeH
//---------------------------------------------------------------------------

class Envelope {
	public:

	int Samples,        // Кол-во аудио отсчётов на единицу огибающей
		Size,           // Максимальное кол-во точек огибающей
		Items,          // Кол-во точек огибающей
		Pos,            // Позиция следующего элемента
		Min,            // Минимум огибающей на всех значениях в буфере
		Max,            // Максимум огибающей на всех значениях в буфере
		SilencePercent, // Значение в процентах от максимума, ниже которого звук считается тишиной
        Last;           // Последнее значение параметра i в вызове FindLocalMinMax
	unsigned int
		AudioCounter,   // Счётчик добавленных аудио точек
		AudioMax,       // Локальный максимум аудио точек
        Counter;        // Кол-во добавленных элементов за всё время (для 100 точек в секунду хватит на 497 дней)

	short
    	*Data = NULL, 	// Точки огибающей
        *LocalMin = NULL,
        *LocalMax = NULL;

	__fastcall Envelope(int samples, int size);
	__fastcall ~Envelope();
    void __fastcall Init(void);         		// Сбросить значения
	void __fastcall Add(short val);             // Добавить значение
    int __fastcall GetData(unsigned int pos, short *buf, int n);    // Получить данные огибающей
	short __fastcall Get(int i);
	short __fastcall GetBack(int i);
	short __fastcall FindMinMax(void);          // Находим минимум и максимум
	void __fastcall FindAllLocalMinMax(int radius, int delta); // Находим минимум и максимум в окрестностях i с радиусом radius
	short __fastcall FindLocalMinMax(int i, int radius); // Находим минимум и максимум в окрестностях i с радиусом radius
	void __fastcall AddAudioPoint(short val);   // Добавляем аудио точку
    short __fastcall GetForAudioPoint(int k);
	short __fastcall GetForAudioPoint(int k, int pos, int items, int audio_counter);
	bool __fastcall CheckSilence(short val);  	// Проверка тишины

    // Работа с файлами
    bool __fastcall Save(UnicodeString fname); // Сохранить огибающую
    bool __fastcall Load(UnicodeString fname); // Загрузить огибающую
    bool __fastcall FromFile(int samples, UnicodeString fname, int offset); // Создание огибающей из DAT или WAV файла
};

#endif

