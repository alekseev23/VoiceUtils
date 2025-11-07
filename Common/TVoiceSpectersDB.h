//---------------------------------------------------------------------------

#ifndef TVoiceSpectersDBH
#define TVoiceSpectersDBH
//---------------------------------------------------------------------------
#include <vector>

#define TVoiceSpectersDB_MAX_PTS 20
#define TVoiceSpectersDB_Main_Specters 5

//---------------------------------------------------------------------------
struct TVoiceSpecters_Object {
	double value;
	short freq;

	// Overload the less-than operator for default sorting by 'value'
	bool operator<(const TVoiceSpecters_Object& other) const {
		return value > other.value;
	}
};
//---------------------------------------------------------------------------
struct TVoiceSpecters_Search {
	short
		value,  	// Кол-во соврадений
		specter,	// Номер набора спектров
		num;          // Номер спектра в наборе

	// Overload the less-than operator for default sorting by 'value'
	bool operator<(const TVoiceSpecters_Search& other) const {
		return value > other.value;
	}
};
//---------------------------------------------------------------------------
// Точки максиммума в спектре и их значения
struct TVoiceSpectersDB_One_Specter {
	short
		Max = TVoiceSpectersDB_MAX_PTS,     // Максимальное кол-во точек максимума
		Count;                              // Кол-во точек максимума, но не более TVoiceSpectersDB_MAX_PTS
	short
		Freq[TVoiceSpectersDB_MAX_PTS], 	// Частота
		Value[TVoiceSpectersDB_MAX_PTS];	// Относительное значение
};


//---------------------------------------------------------------------------
// Один звук, содержащий имя, версию и TVoiceSpectersDB_Main_Specters спектров (в зависимости от времени)
struct TVoiceSpectersDB_Specters {
	UnicodeString Sound;
	int Id;
	TVoiceSpectersDB_One_Specter Specters[TVoiceSpectersDB_Main_Specters];
};

//---------------------------------------------------------------------------
class TVoiceSpectersDB {

public:

	int
		N,                  // Кол-во точек аудио данных для вычисления спектра
		N2,                 // Кол-во точек в исходных спектрах
        NN,
		Count,              // Кол-во звуков в базе
		MainCount;          // Кол-во спектров в звуке
	TVoiceSpectersDB_Specters *VoiceSpecters = NULL;

	TVoiceSpectersDB();
	~TVoiceSpectersDB();

	// Создание базы спектров
	void SetSize(int count);

	// Задать имя и версию (id) набора спектров
	void SetParams(int i, UnicodeString sound, int id);
	// Задать данные спектра
	void Set(int i, int j, int n, double *A);

	// Чтение базы со спектрами
	bool Load(UnicodeString fname);

	// Запись базы со спектрами
	bool Save(UnicodeString fname);

	// Получение значения точки
	double GetSpecterValue(double val, int scale);

	// Анализ спектра и получение точек максимума
	int Analyze(int n, double *A, struct TVoiceSpectersDB_One_Specter *res);

	// Сравнение двух спектров s1 и s2 и возвращение кол-ва совпадений
	int Compare(struct TVoiceSpectersDB_One_Specter *s1, struct TVoiceSpectersDB_One_Specter *s2, int delta);

	// Поиск наиболее похожего спектра
    int Search(struct TVoiceSpectersDB_One_Specter *specter, std::vector<TVoiceSpecters_Search> &result, int delta);

};
#endif
