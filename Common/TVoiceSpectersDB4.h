//---------------------------------------------------------------------------

#ifndef TVoiceSpectersDB4H
#define TVoiceSpectersDB4H
//---------------------------------------------------------------------------
#include <algorithm> // For std::sort
#include <iostream>
#include <vector>
#include "TBallsSum.h"

//#define TVoiceSpectersDB4_M 320
#define TVoiceSpectersDB4_M 1024

//---------------------------------------------------------------------------
/*
Точка спектра
*/
class TVoiceSpectersDB4_Point {
public:
    double
        Min = 0.,
        Max = 0.,
        Middle = 0.;
};

//---------------------------------------------------------------------------
/*
Один спектр, включающий сумму для средних значений
*/
class TVoiceSpectersDB4_Specter {

public:
    double Sum = 0.;
    class TVoiceSpectersDB4_Point *Points;

    TVoiceSpectersDB4_Specter() {
        Points = new class TVoiceSpectersDB4_Point[TVoiceSpectersDB4_M];
    }

    ~TVoiceSpectersDB4_Specter() {
        delete[] Points;
    }
};
//---------------------------------------------------------------------------
/*
Объект для хранения информации о пиках звука
*/
class TVoiceSpectersDB4_Peaks {

public:

    int
    	N,      // Кол-во точек в спектре
        Count;  // Кол-во пиков
    int Tag;
    double
    	*Max = NULL,    // Максимумв пике
        *Sum = NULL,    // Сумма значений в районе пика
        GlobalSum,      // Глобальная сумма
        GlobalMax;      // Глобальный максимум

    TVoiceSpectersDB4_Peaks();
    ~TVoiceSpectersDB4_Peaks();
    void Set(double *values, int n);
    double CompareMax(double *max, double *sum, double &dif, double &same);
    double CompareSum(double *max, double *sum, double &dif, double &same);
    double CompareMax(TVoiceSpectersDB4_Peaks *peak, double &dif, double &same);
    double CompareSum(TVoiceSpectersDB4_Peaks *peak, double &dif, double &same);
};
//---------------------------------------------------------------------------
/*
Объект для хранения звука и его спектров
*/
class TVoiceSpectersDB4_Sound {

public:
    UnicodeString Sound;    // Имя звука (А, Б, ...)
    int
    	Id = 0,     // Версия звука (1, 2, ...)
        Count = 0;  // Кол-во спектров
    std::vector<TVoiceSpectersDB4_Specter> Specters;
    TVoiceSpectersDB4_Peaks *Peaks = NULL;

    TVoiceSpectersDB4_Sound();
    TVoiceSpectersDB4_Sound(const TVoiceSpectersDB4_Sound &s);
    TVoiceSpectersDB4_Sound(TVoiceSpectersDB4_Sound &&s);
    ~TVoiceSpectersDB4_Sound();
    bool Read(FILE *fd);
    bool Write(FILE *fd);
    void Set(UnicodeString sound, int id);

    // Очистка списка
    void Clear(void);
    void Add(class TVoiceSpectersDB4_Specter &s);
    void Add(double *min, double *max, double *middle);
    class TVoiceSpectersDB4_Specter* Get(int i);

	// Сравнение двух спектров s1 и s2 и возвращение кол-ва совпадений
	double Compare(std::vector<TVoiceSpectersDB4_Specter> &s1, std::vector<TVoiceSpectersDB4_Specter> &s2,
    	int p1, int p2, int n, double &dif, double &same);

    // Поиск натболее похожего фрагмента спектров
    int SearchMin(std::vector<TVoiceSpectersDB4_Specter> &s, int p, int n, double &dif, double &same);

};

//---------------------------------------------------------------------------
/*
База звуков
*/
class TVoiceSpectersDB4 {

public:
    int Count;  // Кол-во версий разных звуков

    //std::vector<TVoiceSpectersDB4_Sound> Sounds;
    TVoiceSpectersDB4_Sound Sounds[31];

	TVoiceSpectersDB4();
    ~TVoiceSpectersDB4();

	// Создание базы спектров
	void SetSize(int count);

	// Чтение базы со спектрами
	bool Load(UnicodeString fname);

	// Запись базы со спектрами
	bool Save(UnicodeString fname);

    // Очистка списка
    void Clear(void);

	// Поиск наиболее похожего спектра
	UnicodeString Find(std::vector<TVoiceSpectersDB4_Specter> &s, int p, int n,
    	double &dif, double &same, int &id, int &fpos);

	// Поиск наиболее похожего звука
	bool Search(std::vector<TVoiceSpectersDB4_Specter> &s, int from, int len, int dn, TBallsSum &sum);
	bool SearchPeaks(std::vector<TVoiceSpectersDB4_Specter> &s, TBallsSum &sum);

};
#endif
