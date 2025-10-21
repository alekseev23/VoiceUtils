//---------------------------------------------------------------------------

#ifndef SpecterH
#define SpecterH
//---------------------------------------------------------------------------
#include <vcl.h>
#include "common.h"

// Спектр
class Specter {

public:

	unsigned char *Data;    // Массив данных (точки спектра)
	int
		N,          // Кол-во точек в спектре
		Parts,      // Кол-во отрезков по которым производится сравнение двух спектров
		I,          // Размер отрезка
		DeltaPart,  // Допустимая разница для отрезка
		Delta,      // Средняя допустимая разница
		DeltaColumn;// Допустимая разница для значения

	__fastcall Specter();
	__fastcall ~Specter();

	// Установка параметров
	void __fastcall SetParams(int n);
	void __fastcall SetParams(int n, int parts);
	void __fastcall SetDelta(int delta, int delta_column);

	// Задать в спектр значения из массива
	void __fastcall Set(unsigned char *data);

	// Получить из спектра значения в массив
	void __fastcall Get(unsigned char *data);

	// Сравнение спектра с массивом данных
	int __fastcall Compare(unsigned char *data);
	// Сравнение спектра с массивом данных
	bool __fastcall Check(unsigned char *data);
	bool __fastcall FastCheck(unsigned char *data);

	// Чтение и запись данных
	bool __fastcall Read(FILE *fd);
	bool __fastcall Write(FILE *fd);

	// Копирование
	void __fastcall CopyTo(Specter &specter);
	void __fastcall CopyFrom(Specter &specter);
};

#endif