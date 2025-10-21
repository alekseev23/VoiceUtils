//---------------------------------------------------------------------------

#pragma hdrstop

#include "SpecterSection.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace std;

__fastcall SpecterSection::SpecterSection() {
	Specter = NULL;
	Popularity = NULL;
	Items = 0;
}

__fastcall SpecterSection::~SpecterSection() {
	if (Specter != NULL) delete[] Specter;
	if (Popularity != NULL) delete[] Popularity;
}

//------------------------------------------------------------------------------
// Установка параметров
//------------------------------------------------------------------------------
void __fastcall SpecterSection::SetParams(int n, int items) {
	N = n;
	Items = items;
	if (Specter != NULL) delete[] Specter;
	if (Popularity != NULL) delete[] Popularity;
	if (Items > 0) {
		Specter = new class Specter [Items];
		Popularity = new int [Items];
		for (int i = 0; i < Items; i++) {
			Specter[i].SetParams(N, 8);
		}
	}
	else {
		//Specter = NULL;
		//Popularity = NULL;
        Specter = new class Specter [1];
        Specter[0].SetParams(N, 8);
		Popularity = new int [1];
	}
}

//------------------------------------------------------------------------------
// Установка параметров
//------------------------------------------------------------------------------
void __fastcall SpecterSection::SetParams(int n, int items, int all_sections, int good_sections) {
	SetParams(n, items);
	AllSections = all_sections;
	GoodSections = good_sections;
}

void __fastcall SpecterSection::SetDelta(int delta, int delta_column) {
	Delta = delta;
	DeltaColumn = delta_column;
	for (int i = 0; i < Items; i++) Specter[i].SetDelta(Delta, DeltaColumn);
}

//------------------------------------------------------------------------------
// Чтение данных спектров
//------------------------------------------------------------------------------
bool __fastcall SpecterSection::ReadSpecter(FILE *fd) {
	if (Specter == NULL) return false;
	for (int i = 0; i < Items; i++) {
		if (!Specter[i].Read(fd)) return false;
	}
	return true;
}

//------------------------------------------------------------------------------
// Чтение популярности
//------------------------------------------------------------------------------
bool __fastcall SpecterSection::ReadPopularity(FILE *fd) {
	if (Popularity == NULL) return false;
    if (Items == 0) return true; // Да, пустой набор, ну и чё?
	if (fread(Popularity, 4, Items, fd) != Items) return false;
	return true;
}

//------------------------------------------------------------------------------
// Запись данных спектров
//------------------------------------------------------------------------------
bool __fastcall SpecterSection::WriteSpecter(FILE *fd) {
	for (int i = 0; i < Items; i++) {
		if (!Specter[i].Write(fd)) return false;
	}
	return true;
}

//------------------------------------------------------------------------------
// Запись популярности
//------------------------------------------------------------------------------
bool __fastcall SpecterSection::WritePopularity(FILE *fd) {
	if (Popularity == NULL) return false;
    if (Items == 0) return true; // Да, пустой набор, ну и чё?
	if (fwrite(Popularity, 4, Items, fd) != Items) return false;
	return true;
}

//------------------------------------------------------------------------------
// Чтение
//------------------------------------------------------------------------------
bool __fastcall SpecterSection::Read(FILE *fd) {
	// Items
	if (fread(&Items, 4, 1, fd) != 1) return false;
	// AllSections
	if (fread(&AllSections, 4, 1, fd) != 1) return false;
	// GoodSections
	if (fread(&GoodSections, 4, 1, fd) != 1) return false;
	// Переконфигурация
	SetParams(N, Items, AllSections, GoodSections);
	// Specter
	if (!ReadSpecter(fd)) return false;
	// Popularity
	if (!ReadPopularity(fd)) return false;
	return true;
}

//------------------------------------------------------------------------------
// Запись
//------------------------------------------------------------------------------
bool __fastcall SpecterSection::Write(FILE *fd) {
	// Items
	if (fwrite(&Items, 4, 1, fd) != 1) return false;
	// AllSections
	if (fwrite(&AllSections, 4, 1, fd) != 1) return false;
	// GoodSections
	if (fwrite(&GoodSections, 4, 1, fd) != 1) return false;
	// Specter
	if (!WriteSpecter(fd)) return false;
	// Popularity
	if (!WritePopularity(fd)) return false;
	return true;
}

//------------------------------------------------------------------------------
// Копирование данных в другой экземпляр
//------------------------------------------------------------------------------
void __fastcall SpecterSection::CopyTo(SpecterSection &specterSection) {
	specterSection.SetParams(N, Items, Delta, DeltaColumn);
	specterSection.AllSections = AllSections;
	specterSection.GoodSections = GoodSections;
	for (int i = 0; i < Items; i++) {
		specterSection.Specter[i].CopyFrom(Specter[i]);
		specterSection.Popularity[i] = Popularity[i];
	}
}

//------------------------------------------------------------------------------
// Копирование данных из другого экземпляра
//------------------------------------------------------------------------------
void __fastcall SpecterSection::CopyFrom(SpecterSection &specterSection) {
	SetParams(specterSection.N, specterSection.Items, specterSection.Delta, specterSection.DeltaColumn);
	AllSections = specterSection.AllSections;
	GoodSections = specterSection.GoodSections;
	for (int i = 0; i < Items; i++) {
		Specter[i].CopyFrom(specterSection.Specter[i]);
		Popularity[i] = specterSection.Popularity[i];
	}
}

//------------------------------------------------------------------------------
// Удаление всех спектров из набора
//------------------------------------------------------------------------------
void __fastcall SpecterSection::Clear(void) {
	if (Specter != NULL) delete[] Specter;
	if (Popularity != NULL) delete[] Popularity;
	Specter = NULL;
	Popularity = NULL;
	Items = 0;
}

//------------------------------------------------------------------------------
// Резервирование для n спектров
//------------------------------------------------------------------------------
void __fastcall SpecterSection::Create(int n) {
	SetParams(n, Items, Delta, DeltaColumn);
}

//------------------------------------------------------------------------------
// Добавление спектра
//------------------------------------------------------------------------------
bool __fastcall SpecterSection::Add(unsigned char *data, int popularity) {
	// Создаём новый массив спектров
	class Specter *tempSpecter = new class Specter [Items];
	int *tempPopularity = new int [Items];
	// Сохраняем данные
	int i;
	for (i = 0; i < Items; i++) {
		tempSpecter[i].CopyFrom(Specter[i]);
		tempPopularity[i] = Popularity[i];
	}
	// Удаляем старые спектры
	delete[] Specter;
	delete[] Popularity;
	// Увеличиваем кол-во спектров
	Items++;
	// Пересоздаём массивы
	Specter = new class Specter [Items];
	Popularity = new int [Items];
	// Восстанавливаем данные
	for (i = 0; i < Items; i++) {
		Specter[i].CopyFrom(tempSpecter[i]);
		Popularity[i] = tempPopularity[i];
	}
	// Добавляем новые значения
	Specter[Items - 1].SetParams(N);
	Specter[Items - 1].SetDelta(Delta, DeltaColumn);
	Specter[Items - 1].Set(data);
	Popularity[Items - 1] = popularity;
	// Удаляем временные массивы
	delete[] tempSpecter;
	delete[] tempPopularity;
	return true;
}

//------------------------------------------------------------------------------
// Удаление спектра
//------------------------------------------------------------------------------
bool __fastcall SpecterSection::Delete(int n) {
	if (n >= Items) return false;
	if (Items == 1) {
		 Clear();
		 return true;
	}
	// Создаём новый массив спектров
	class Specter *tempSpecter = new class Specter [Items - 1];
	int *tempPopularity = new int [Items - 1];
	// Сохраняем данные
	int i, j;
	for (i = 0, j = 0; i < Items; i++) {
		if (i == n) continue;
		tempSpecter[j].CopyFrom(Specter[i]);
		tempPopularity[j] = Popularity[i];
	}
	// Удаляем старые спектры
	delete[] Specter;
	delete[] Popularity;
	// Уменьшаем кол-во спектров
	Items--;
	// Пересоздаём массивы
	Specter = new class Specter [Items];
	Popularity = new int [Items];
	// Восстанавливаем данные
	for (i = 0; i < Items; i++) {
		Specter[i].CopyFrom(tempSpecter[i]);
		Popularity[i] = tempPopularity[i];
	}
	// Удаляем временные массивы
	delete[] tempSpecter;
	delete[] tempPopularity;
	return true;
}

//------------------------------------------------------------------------------
// Замена спектра
//------------------------------------------------------------------------------
bool __fastcall SpecterSection::Replace(int n, unsigned char *data, int popularity) {
	if (n >= Items) return false;
	Specter[n].Set(data);
	Popularity[n] = popularity;
	return true;
}

//------------------------------------------------------------------------------
// Поиск спектра, удовлетворяющего условию
//------------------------------------------------------------------------------
bool __fastcall SpecterSection::Check(unsigned char *data) {
	if (Items == 0) return false;
	for (int i = 0; i < Items; i++) {
		if (Specter[i].Check(data)) {
			LastNum = i;
			Counter++;
			return true;
		}
	}
    return false;
}

//------------------------------------------------------------------------------
// Поиск спектра, удовлетворяющего условию и возврат его номера или -1
//------------------------------------------------------------------------------
int __fastcall SpecterSection::Find(unsigned char *data) {
	if (Items == 0) return -1;
	for (int i = 0; i < Items; i++) {
		if (Specter[i].Check(data)) {
			//LastNum = i;
			//Counter++;
			return i;
		}
	}
    return -1;

}

//------------------------------------------------------------------------------
// Поиск спектра с минимальной разницей
// Возвращается минимальная разница или -1 в случае ошибки
//------------------------------------------------------------------------------
int __fastcall SpecterSection::FindMin(unsigned char *data) {
	if (Items == 0) return -1;
	Min = 1000000;
	Max = 0;
	int dif;
	for (int i = 0; i < Items; i++) {
		dif = Specter[i].Compare(data);
		if (dif < Min) {
			Min = dif;
			LastNum = i;
		}
		if (dif > Max) Max = dif;
	}
	return dif;
}
