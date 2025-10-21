#include "SpecterSet.h"
#include "Freq2Note.h"
#include "../../_Common_/Lib/files.h"

__fastcall SpecterSet::SpecterSet() {
	SpecterTypes = 5;
	SpecterSection = new class SpecterSection [SpecterTypes];
	LastNum = -1;
	Counter = 0;
    //int N[] = {256, 512, 1024, 2048, 4096}; // Размерности спектров
    int N[] = {FREQ_POINTS, FREQ_POINTS, FREQ_POINTS, FREQ_POINTS, FREQ_POINTS}; // Размерности спектров
	for (int i = 0; i < SpecterTypes; i++) {
		SpecterSection[i].SetParams(N[i], 1);
	}
}

__fastcall SpecterSet::~SpecterSet() {
	delete[] SpecterSection;
}

//------------------------------------------------------------------------------
// Установка параметров
//------------------------------------------------------------------------------
void __fastcall SpecterSet::SetDelta(int *delta, int *delta_column) {
	for (int i = 0; i < SpecterTypes; i++) {
		Delta[i] = delta[i];
		DeltaColumn[i] = delta_column[i];
		SpecterSection[i].SetDelta(Delta[i], DeltaColumn[i]);
	}
}

//------------------------------------------------------------------------------
// Поиск спектра, удовлетворяющего условию
//------------------------------------------------------------------------------
bool __fastcall SpecterSet::Check(int specter_type, unsigned char *data) {
	bool res = SpecterSection[specter_type].Check(data);
	LastNum = SpecterSection[specter_type].LastNum;
	Counter++;
	return res;
}

//------------------------------------------------------------------------------
// Поиск спектра с минимальной разницей
//------------------------------------------------------------------------------
int __fastcall SpecterSet::FindMin(int specter_type, unsigned char *data) {
	bool res = SpecterSection[specter_type].Check(data);
	Min = SpecterSection[specter_type].Min;
	Max = SpecterSection[specter_type].Max;
	LastNum = SpecterSection[specter_type].LastNum;
	return res;
}

// Запись набора
bool __fastcall SpecterSet::Write(FILE *fd) {
	int val;
	// Сохраняем разделы разных видов спектров
	for (int i = 0; i < SpecterTypes; i++) {
		if (!SpecterSection[i].Write(fd)) return false;
	}
	// Сохраняем продолжительность исходного аудио в секундах
	if (fwrite(&Duration, 4, 1, fd) != 1) return false;
	// Преобразуем дату и время в строку без разделителей
	if (CreationDate == "") CreationDate = FormatDateTime("yyyy-mm-dd hh:mm:ss", Now());
	AnsiString a = AnsiString(CreationDate);
	// Сохраняем дату и время создания
	if (fwrite(a.c_str(), 1, 19, fd) != 19) return false;
	// Находим длину описания
	val = Description.Length() * 2;
	// Сохраняем длину описания
	if (fwrite(&val, 4, 1, fd) != 1) return false;
	// Сохраняем описание
	if (fwrite(Description.w_str(), 2, val / 2, fd) != val / 2) return false;
	// Сохраняем длину URL
	val = URL.Length() * 2;
	if (fwrite(&val, 4, 1, fd) != 1) return false;
	// Сохраняем URL
	fwrite(URL.w_str(), 2, val / 2, fd);
	return true;
}

// Чтение набора
bool __fastcall SpecterSet::Read(FILE *fd) {
	int val;
	wchar_t *w;
	Error = 0;
	// Читаем разделы разных видов спектров
	for (int i = 0; i < SpecterTypes; i++) {
		if (!SpecterSection[i].Read(fd)) { Error = 1; return false; }
	}
	// Читаем продолжительность исходного аудио в секундах
	if (fread(&Duration, 4, 1, fd) != 1) { Error = 2; return false; }
	// Читаем дату и время создания
	char v[20];
	if (fread(v, 1, 19, fd) != 19) { Error = 3; return false; }
	v[19] = 0;
	CreationDate = UnicodeString(v);
	// Находим длину описания
	if (fread(&val, 4, 1, fd) != 1) { Error = 4; return false; }
	// Читаем описание
	if (val == 0) Description = "";
	else {
		w = new wchar_t [val];
		if (fread(w, 1, val, fd) != val) {
			delete[] w;
			Error = 5;
			return false;
		}
		w[val / 2] = 0;
		Description = UnicodeString(w);
		delete[] w;
	}
	// Читаем длину URL
	if (fread(&val, 4, 1, fd) != 1) { Error = 6; return false; }
	// Читаем URL
	if (val == 0) URL = "";
	else {
		w = new wchar_t [val];
		if (fread(w, 1, val, fd) != val) {
			delete[] w;
			Error = 7;
			return false;
		}
		w[val / 2] = 0;
		URL = UnicodeString(w);
		delete[] w;
	}
	return true;
}

// Задание параметров
void __fastcall SpecterSet::SetInfo(int duration, UnicodeString creation_date,
	UnicodeString description, UnicodeString url) {
	Duration = duration;
	CreationDate = creation_date;
	Description = description;
	URL = url;
}

void __fastcall SpecterSet::CopyTo(SpecterSet &a) {
	delete[] a.SpecterSection;
	a.SpecterTypes = SpecterTypes;
	a.SpecterSection = new class SpecterSection [a.SpecterTypes];
	for (int i = 0; i < SpecterTypes; i++) {
		a.Delta[i] = Delta[i];
		a.DeltaColumn[i] = DeltaColumn[i];
        a.SpecterSection[i].CopyFrom(SpecterSection[i]);
	}
	a.LastNum = LastNum;
	a.Counter = Counter;
	a.Min = Min;
	a.Max = Max;
	a.Duration = Duration;
	a.CreationDate = CreationDate;
	a.Description = Description;
	a.URL = URL;
}

void __fastcall SpecterSet::CopyFrom(SpecterSet &a) {
	delete[] SpecterSection;
	SpecterTypes = a.SpecterTypes;
	SpecterSection = new class SpecterSection [SpecterTypes];
	for (int i = 0; i < SpecterTypes; i++) {
		Delta[i] = a.Delta[i];
		DeltaColumn[i] = a.DeltaColumn[i];
        SpecterSection[i].CopyFrom(a.SpecterSection[i]);
	}
	LastNum = a.LastNum;
	Counter = a.Counter;
	Min = a.Min;
	Max = a.Max;
	Duration = a.Duration;
	CreationDate = a.CreationDate;
	Description = a.Description;
	URL = a.URL;
}

