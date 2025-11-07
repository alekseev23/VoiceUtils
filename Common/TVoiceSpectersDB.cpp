//---------------------------------------------------------------------------

#pragma hdrstop

#include "TVoiceSpectersDB.h"
#include <System.Math.hpp>
#include <algorithm> // For std::sort
#include <iostream>
#include <vector>
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/strings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace std;

//---------------------------------------------------------------------------
TVoiceSpectersDB::TVoiceSpectersDB() {
	Count = 0;
	N2 = 256;
	MainCount = 5;
	//
	N = 4096;
	N2 = 2048;
	NN = 2048;
}

//---------------------------------------------------------------------------
TVoiceSpectersDB::~TVoiceSpectersDB() {
	if (VoiceSpecters != NULL) delete[] VoiceSpecters;
}

//---------------------------------------------------------------------------
void TVoiceSpectersDB::SetSize(int count) {
	Count = count;
	if (VoiceSpecters != NULL) delete[] VoiceSpecters;
	VoiceSpecters = new TVoiceSpectersDB_Specters [Count];
}

//---------------------------------------------------------------------------
bool TVoiceSpectersDB::Load(UnicodeString fname) {
	int fsize = getFileSize(fname);
	if (fsize == 0) return false;
	//Count = fsize / sizeof(TVoiceSpectersDB_Specters);
	//Count = fsize / 215;
    Count = fsize / ((2 + TVoiceSpectersDB_MAX_PTS * (2 + 2)) * TVoiceSpectersDB_Main_Specters + 2 * 2 + 1);
//ShowMessage("TVoiceSpectersDB::Load Count=" + UnicodeString(Count));
//ShowMessage("TVoiceSpectersDB::Load sizeof(TVoiceSpectersDB_Specters)=" + UnicodeString(sizeof(TVoiceSpectersDB_Specters)));
	if (VoiceSpecters != NULL) delete[] VoiceSpecters;
	VoiceSpecters = new TVoiceSpectersDB_Specters [Count];
	FILE *fd = _wfopen(fname.w_str(), L"rb");
	for (auto i = 0; i < Count; i++) {
		// Чтение звука
		wchar_t SoundChar[3] = {0, 0, 0};
		fread(&SoundChar, 2, 2, fd);
		VoiceSpecters[i].Sound = UnicodeString(SoundChar);
		// Чтение версии
		unsigned char id;
		fread(&id, 1, 1, fd);
		VoiceSpecters[i].Id = id;
		// Чтение спектров
		for (auto j = 0; j < TVoiceSpectersDB_Main_Specters; j++) {
			// Кол-во точек
			fread(&VoiceSpecters[i].Specters[j].Count, 2, 1, fd);
			// Частоты
			fread(&VoiceSpecters[i].Specters[j].Freq, 2, TVoiceSpectersDB_MAX_PTS, fd);
			// Значения
			fread(&VoiceSpecters[i].Specters[j].Value, 2, TVoiceSpectersDB_MAX_PTS, fd);
		}
	}
	// Закрываем файл
	fclose(fd);
	return true;
}

//---------------------------------------------------------------------------
bool TVoiceSpectersDB::Save(UnicodeString fname) {
	// Создание объекта базы данных
	FILE *fd = _wfopen(fname.w_str(), L"wb");
	// Цик по звукам
	for (auto i = 0; i < Count; i++) {
		// Sound
		wchar_t SoundChar[10];
		wcscpy(SoundChar, VoiceSpecters[i].Sound.w_str());
		fwrite(SoundChar, 2, 2, fd);
		// Id (версия)
		unsigned char id;
		id = (unsigned char)VoiceSpecters[i].Id;
		fwrite(&id, 1, 1, fd);
		// Цикл по спектрам внутри одного звука
		for (auto j = 0; j < TVoiceSpectersDB_Main_Specters; j++) {
			// Сохранение спектров
			// Кол-во точек
			fwrite(&VoiceSpecters[i].Specters[j].Count, 2, 1, fd);
//cout <<  i << " " << j << " " << (int)VoiceSpecters[i].Specters[j].Count << endl;
			// Частоты
			fwrite(&VoiceSpecters[i].Specters[j].Freq, 2, TVoiceSpectersDB_MAX_PTS, fd);
			// Значения
			fwrite(&VoiceSpecters[i].Specters[j].Value, 2, TVoiceSpectersDB_MAX_PTS, fd);
		}
	}
	// Закрываем файл
	fclose(fd);
	return true;
}

//---------------------------------------------------------------------------
/*
A[n] - входной спектр
*/
int TVoiceSpectersDB::Analyze(int n, double *A, struct TVoiceSpectersDB_One_Specter *res) {
	double max = 0.;
	std::vector<TVoiceSpecters_Object> objects;
	// Находим максимальное значение
	for (auto i = 0; i < n ; i++) {
		if (A[i] > max) max = A[i];
	}
	// Ищем максимумы
	for (short i = 1; i < n - 1; i++) {
		if ((A[i] > A[i - 1]) && (A[i] > A[i + 1])) {
			objects.push_back({A[i], i});
		}
	}
	// Сортировка максимумов
	// Sort using the overloaded operator<
	std::sort(objects.begin(), objects.end());
	int k = objects.size();
	if (k > TVoiceSpectersDB_MAX_PTS) k = TVoiceSpectersDB_MAX_PTS;
	res->Count = k;
	for (auto i = 0; i < k; i++) {
		res->Freq[i] = objects[i].freq;
		res->Value[i] = (short)(32767. * objects[i].value / max);
	}
	return res->Count;
}


//---------------------------------------------------------------------------
/* Получение значения точки спектра
scale = 1 - линейная шкала
scale = 2 - десятичный логарифм
scale = 3 - логарифм по основанию 1.042
*/
double TVoiceSpectersDB::GetSpecterValue(double val, int scale) {
if (Count == 0) return 0.;
switch (scale) {
	case 1: // Линейная шкала
			return val;
			break;
	case 2: // Десятичный логарифм
			if (val <= 0.) val = 0.;
			else {
				val = log10(val);
				if (val <= 0.) val = 0.;
			}
			return val;
			break;
	case 3: // Логарифм по основанию
			if (val <= 0.) val = 0.;
			else {
				val = Math::LogN(1.042, val);
				if (val <= 0.) val = 0.;
			}
			return val;
			break;
}
return 0.;
}

//---------------------------------------------------------------------------
// Задать имя и версию (id) набора спектров
void TVoiceSpectersDB::SetParams(int i, UnicodeString sound, int id) {
	VoiceSpecters[i].Sound = sound;
	VoiceSpecters[i].Id = id;
}

//---------------------------------------------------------------------------
// Задать данные спектра
void TVoiceSpectersDB::Set(int i, int j, int n, double *A) {
	Analyze(n, A, &VoiceSpecters[i].Specters[j]);
}

//---------------------------------------------------------------------------
// Сравнение двух спектров s1 и s2 и возвращение кол-ва совпадений
int TVoiceSpectersDB::Compare(struct TVoiceSpectersDB_One_Specter *s1, struct TVoiceSpectersDB_One_Specter *s2, int delta) {
	int res = 0;
	for (auto i = 0; i < s1->Count; i++) {
		for (auto j = 0; j < s2->Count; j++) {
			if (abs(s1->Freq[i] - s2->Freq[j]) <= delta) res++;
		}
	}
	return res;
}
//---------------------------------------------------------------------------
// Поиск наиболее похожего спектра
int TVoiceSpectersDB::Search(struct TVoiceSpectersDB_One_Specter *specter, std::vector<TVoiceSpecters_Search> &result, int delta) {
	// Очистка вектора
	result.clear();
	// Цикл по всем звукам
	for (auto i = 0; i < Count; i++) {
		// Цикл по всем спектрам внутри звука
		for (auto j = 0; j < TVoiceSpectersDB_Main_Specters; j++) {
			// Сравнение
			short res = Compare(specter, &VoiceSpecters[i].Specters[j], delta);
			// Добавление результата в вектор
			result.push_back({(short)res, (short)i, (short)j});
		}
	}
	// Сортировка вектора
	std::sort(result.begin(), result.end());
	return 0;
}
//---------------------------------------------------------------------------

