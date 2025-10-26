//---------------------------------------------------------------------------

#pragma hdrstop

#include "TVoiceSpectersDB.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)



TVoiceSpectersDB::TVoiceSpectersDB() {
	Count = 0;
	N2 = 256;
	MainCount = 5;
	//
	N = 4096;
	N2 = 2048;
    NN = 2048;
}

TVoiceSpectersDB::~TVoiceSpectersDB() {
	if (VoiceSpecters != NULL) delete[] VoiceSpecters;
}

void TVoiceSpectersDB::SetSize(int count) {
	Count = count;
	if (VoiceSpecters != NULL) delete[] VoiceSpecters;
	VoiceSpecters = new TVoiceSpecter [Count];
}

bool TVoiceSpectersDB::Load(UnicodeString fname) {
	int fsize = getFileSize(fname);
	if (fsize == 0) return false;
	Count = fsize / sizeof(TVoiceSpecter);
	if (VoiceSpecters != NULL) delete[] VoiceSpecters;
	VoiceSpecters = new TVoiceSpecter [Count];
	FILE *fd = _wfopen(fname.w_str(), L"rb");
	for (auto i = 0; i < Count; i++) {
		fread(&VoiceSpecters[i].Char, 2, 1, fd);
		fread(&VoiceSpecters[i].Id, 1, 1, fd);
		// Сохранение спектров
		fread(&VoiceSpecters[i].Data, 8, N2 * MainCount, fd);
	}
	// Закрываем файл
	fclose(fd);

	return true;
}

bool TVoiceSpectersDB::Save(UnicodeString fname) {
	// Создание объекта базы данных
	FILE *fd = _wfopen(fname.w_str(), L"wb");
	for (auto i = 0; i < Count; i++) {
		fwrite(&VoiceSpecters[i].Char, 2, 1, fd);
		fwrite(&VoiceSpecters[i].Id, 1, 1, fd);
		// Сохранение спектров
		fwrite(&VoiceSpecters[i].Data, 8, N2 * MainCount, fd);
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
	for (auto i = 1; i < n - 1; i++) {
		if ((A[i] > A[i - 1]) && (A[i] > A[i + 1])) {
			objects.push_back({A[i], i});
		}
	}
	// Сортировка максимумов
	// Sort using the overloaded operator<
	std::sort(objects.begin(), objects.end());
	k = objects.length;
	if (k > 10) k = 10;
	res->Count = k;
	for (auto i = 0; i < k; i++) {
		res.Freq[i] = objects[i].freq;
		res.Value[i] = (short)(32767. * objects[i].value / max);
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