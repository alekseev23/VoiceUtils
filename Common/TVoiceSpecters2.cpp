//---------------------------------------------------------------------------

#pragma hdrstop

#include "TVoiceSpecters2.h"
//#include "TCompareSpecters.h"
#include <System.Math.hpp>
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/strings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TVoiceSpecters2::TVoiceSpecters2() {
	Count = 0;
	N2 = 4096;
	MainCount = 5;
}

TVoiceSpecters2::~TVoiceSpecters2() {
	if (VoiceSpecters != NULL) delete[] VoiceSpecters;
}

void TVoiceSpecters2::SetSize(int count) {
	Count = count;
	if (VoiceSpecters != NULL) delete[] VoiceSpecters;
	VoiceSpecters = new TVoiceSpecter [Count];
}

bool TVoiceSpecters2::Load(UnicodeString fname) {
	int fsize = getFileSize(fname);
	if (fsize == 0) return false;
	//Count = fsize / sizeof(TVoiceSpecter);
	Count = fsize / (5 + 8 * N2 * MainCount);
//ShowMessage("Count=" + UnicodeString(Count));
	if (VoiceSpecters != NULL) delete[] VoiceSpecters;
	VoiceSpecters = new TVoiceSpecter [Count];
	FILE *fd = _wfopen(fname.w_str(), L"rb");
	for (auto i = 0; i < Count; i++) {
		//fread(&VoiceSpecters[i].Char, 2, 1, fd);
		// Чтение звука
		wchar_t SoundChar[3] = {0, 0, 0};
		fread(&SoundChar, 2, 2, fd);
		VoiceSpecters[i].Sound = UnicodeString(SoundChar);
//ShowMessage("i=" + UnicodeString(i) + " Sound=" + VoiceSpecters[i].Sound);
		fread(&VoiceSpecters[i].Id, 1, 1, fd);
//ShowMessage("i=" + UnicodeString(i) + " Id=" + UnicodeString(VoiceSpecters[i].Id));
		// Сохранение спектров
		fread(&VoiceSpecters[i].Data, 8, N2 * MainCount, fd);
	}
	// Закрываем файл
	fclose(fd);
	return true;
}

bool TVoiceSpecters2::Save(UnicodeString fname) {
	// Создание объекта базы данных
	FILE *fd = _wfopen(fname.w_str(), L"wb");
	for (auto i = 0; i < Count; i++) {
		//fwrite(&VoiceSpecters[i].Char, 2, 1, fd);
		wchar_t SoundChar[10];
		wcscpy(SoundChar, VoiceSpecters[i].Sound.w_str());
		fwrite(SoundChar, 2, 2, fd);
		fwrite(&VoiceSpecters[i].Id, 1, 1, fd);
		// Сохранение спектров
		fwrite(&VoiceSpecters[i].Data, 8, N2 * MainCount, fd);
	}
	// Закрываем файл
	fclose(fd);
	return true;
}

/*
double TVoiceSpecters2::Search(double *data, UnicodeString &c, char &id, int &n) {
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
				c = VoiceSpecters[i].Sound;
				id = VoiceSpecters[i].Id;
				n = j;
			}
		}
	}
	return dif;
}
*/
//---------------------------------------------------------------------------
/* Получение значения точки спектра
scale = 1 - линейная шкала
scale = 2 - десятичный логарифм
scale = 3 - логарифм по основанию 1.042
*/
double TVoiceSpecters2::GetSpecterValue(double val, int scale) {
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
