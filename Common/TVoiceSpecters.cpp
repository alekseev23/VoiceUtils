//---------------------------------------------------------------------------

#pragma hdrstop

#include "TVoiceSpecters.h"
//#include "TCompareSpecters.h"
#include <System.Math.hpp>
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/strings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TVoiceSpecters::TVoiceSpecters() {
	Count = 0;
	N2 = 256;
	MainCount = 5;
}

TVoiceSpecters::~TVoiceSpecters() {
	if (VoiceSpecters != NULL) delete[] VoiceSpecters;
}

void TVoiceSpecters::SetSize(int count) {
	Count = count;
	if (VoiceSpecters != NULL) delete[] VoiceSpecters;
	VoiceSpecters = new TVoiceSpecter [Count];
}

bool TVoiceSpecters::Load(UnicodeString fname) {
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

bool TVoiceSpecters::Save(UnicodeString fname) {
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

double TVoiceSpecters::Search(double *data, wchar_t &c, char &id, int &n) {
	double dif = 1e8, d, val;
	for (auto i = 0; i < Count; i++) {
		for (auto j = 0; j < MainCount; j++) {
		//for (auto j = 2; j < 3; j++) {
			d = 0.;
			for (auto k = 0; k < N2; k++) {
				val = GetSpecterValue(VoiceSpecters[i].Data[j * N2 + k], 3);
				d += fabs(data[k] - val);
			}
			d /= N2;
			if (d < dif) {
				dif = d;
				c = VoiceSpecters[i].Char;
				id = VoiceSpecters[i].Id;
				n = j;
			}
		}
	}
	return dif;
}

double TVoiceSpecters::Search1(double *data, wchar_t &c, char &id, int &n, int scale) {
	TCompareSpecters CompareSpecters;
	double *A1, *A2, m1, m2;
	// Изменяем scale для data
	A1 = new double [N2];
	A2 = new double [N2];
	//for (auto i = 0; i < N2; i++) A1[i] = data[i];
	CompareSpecters.SetSpecterValue(N2, data, A1, scale);
	// Находим множитель для data
	m1 = 100. / CompareSpecters.GetMaxValue(N2, A1);
	//
	//
	//
	//
	//



	double dif = 1e8, d, val1, val2;
	for (auto i = 0; i < Count; i++) {
		for (auto j = 0; j < MainCount; j++) {
			d = 0.;
			CompareSpecters.SetSpecterValue(N2, &VoiceSpecters[i].Data[j * N2], A2, scale);
			// Находим множитель
			m2 = 100. / CompareSpecters.GetMaxValue(N2, A2);
			/*for (auto k = 0; k < N2; k++) {
				val1 = GetSpecterValue(VoiceSpecters[i].Data[j * N2 + k], scale);
				val2 = GetSpecterValue(data[k], scale);
				d += fabs(val1 - val2);
			}
			d /= N2;
			*/
			d = CompareSpecters.GetDif(N2, A1, A2, m1, m2) / N2;

			if (d < dif) {
				dif = d;
				c = VoiceSpecters[i].Char;
				id = VoiceSpecters[i].Id;
				n = j;
			}
		}
	}
	return dif;
}

//---------------------------------------------------------------------------

/* Получение значения точки спектра
scale = 1 - линейная шкала
scale = 2 - десятичный логарифм
scale = 3 - логарифм по основанию 1.042
*/
double TVoiceSpecters::GetSpecterValue(double val, int scale) {
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
