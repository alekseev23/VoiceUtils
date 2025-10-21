//---------------------------------------------------------------------------

#ifndef SpecterSectionH
#define SpecterSectionH
//---------------------------------------------------------------------------
#include "Specter.h"

class SpecterSection {
	public:

	int
		N,              // Размерность спектра
		Items,          // Кол-во спектров
		AllSections,    // Кол-во отрезков
		GoodSections,   // Кол-во отрезков со звуком (не тишина)
		Delta,      	// Средняя допустимая разница
		DeltaColumn,	// Допустимая разница для значения

		LastNum,        // Номер спектра с минимальной разницей
		Counter,        // Кол-во удачных поисков
		Min,            // Минимальная разница в последнем поиске
		Max;            // Максимальная разница в последнем поиске

	Specter *Specter;   // Данные спектров
	int *Popularity;    // Кол-во найденных спектров данного типа

	__fastcall SpecterSection();
	__fastcall ~SpecterSection();

	// Установка параметров
	void __fastcall SetParams(int n, int items);
	void __fastcall SetParams(int n, int items, int all_sections, int good_sections);
	void __fastcall SetDelta(int delta, int delta_column);

	//--------------------------------------------------------------------------
	// Чтение  и запись
	//--------------------------------------------------------------------------
	// Чтение данных спектров
	bool __fastcall ReadSpecter(FILE *fd);
	// Чтение популярности
	bool __fastcall ReadPopularity(FILE *fd);
	// Запись данных спектров
	bool __fastcall WriteSpecter(FILE *fd);
	// Запись популярности
	bool __fastcall WritePopularity(FILE *fd);
	// Чтение
	bool __fastcall Read(FILE *fd);
	// Запись
	bool __fastcall Write(FILE *fd);

	// Копирование
	void __fastcall CopyTo(SpecterSection &specterSection);
	void __fastcall CopyFrom(SpecterSection &specterSection);

	//--------------------------------------------------------------------------
	// Операции со спектрами
	//--------------------------------------------------------------------------
	// Удаление всех спектров из набора
	void __fastcall Clear(void);
	// Резервирование для n спектров
	void __fastcall Create(int n);
	// Добавление спектра
	bool __fastcall Add(unsigned char *data, int popularity);
	// Удаление спектра
	bool __fastcall Delete(int n);
	// Замена спектра
	bool __fastcall Replace(int n, unsigned char *data, int popularity);

	//--------------------------------------------------------------------------
	// Поиск и сравнение
	//--------------------------------------------------------------------------
	// Поиск спектра, удовлетворяющего условию
	bool __fastcall Check(unsigned char *data);
	// Поиск спектра, удовлетворяющего условию и возврат его номера или -1
	int __fastcall Find(unsigned char *data);
	// Поиск спектра с минимальной разницей
	int __fastcall FindMin(unsigned char *data);

};
#endif
