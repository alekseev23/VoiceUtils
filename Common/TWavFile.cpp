//---------------------------------------------------------------------------

#pragma hdrstop

#include <mmeapi.h>
#include "TWavFile.h"
#include <System.Math.hpp>
#include "../../_Common_/Lib/files.h"
#include "../Common/FFT.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


TWavFile::TWavFile() {
}

TWavFile::TWavFile(int n) {
	N = n;
	N2 = N / 2;
}

TWavFile::~TWavFile() {
	if (Buf != NULL) delete[] Buf;
	if (Re != NULL) delete[] Re;
	if (Im != NULL) delete[] Im;
}

bool TWavFile::Load(UnicodeString filename) {
	FileName = filename;
	Loaded = false;
	// Грузим данные из DAT файла
	if (!FileExists(filename)) {
		ShowMessage("Файл не найден: " + filename);
		return false;
	}
	int fsize = getFileSize(filename);
	if (fsize == 0) return false;
	BufSize = (fsize - 44) / 2;
	if (Buf != NULL) delete[] Buf;
	Buf = new short[BufSize];
	FILE *fd;
	if ((fd = _wfopen(filename.w_str(), L"rb")) == NULL) {
		ShowMessage("Ошибка открытия файла: " + filename);
		return false;
	}
	fseek(fd, 44, SEEK_SET);
	fread(Buf, 2, BufSize, fd);
	fclose(fd);
	Loaded = true;
	return true;
}

bool TWavFile::Save(UnicodeString filename) {
	WAVEFORMATEX *wf;
    char *recbuf = NULL;		// recording buffer
	DWORD reclen;				// recording length
	// allocate initial buffer and make space for WAVE header
	recbuf = (char*)malloc(44);
	reclen = 44;
	// fill the WAVE header
	memcpy(recbuf, "RIFF\0\0\0\0WAVEfmt \20\0\0\0", 20);
	memcpy(recbuf + 36, "data\0\0\0\0", 8);
	wf = (WAVEFORMATEX*)(recbuf + 20);
	wf->wFormatTag = 1;
	wf->nChannels = 1;
	wf->wBitsPerSample = 16;
	wf->nSamplesPerSec = 48000;
	wf->nBlockAlign = wf->nChannels*wf->wBitsPerSample / 8;
	wf->nAvgBytesPerSec = wf->nSamplesPerSec*wf->nBlockAlign;
	// complete the WAVE header
	*(DWORD*)(recbuf + 4) = reclen - 8 + BufSize * 2;
	*(DWORD*)(recbuf + 40) = reclen - 44 + BufSize * 2;
	FILE *fd;
	if (!(fd = _wfopen(filename.w_str(), L"wb"))) {
		ShowMessage("Can't create the file");
		return false;
	}
	// Сохраняем заголовок
	fwrite(recbuf, 44, 1, fd);
	// Сохраняем данные
	fwrite(Buf, 2, BufSize, fd);
	fclose(fd);
	free(recbuf);
    return true;
}

// Вычисляем спектры из аудио данных
void TWavFile::Calculate() {
	SpectersCount = BufSize / N;
	if (Re != NULL) delete[] Re;
	Re = new double [N * SpectersCount];
	if (Im != NULL) delete[] Im;
	Im = new double [N * SpectersCount];
	for (auto i = 0; i < SpectersCount; i++) {
		// Готовим значения
		auto p = i * N;
		for (auto j = 0; j < N; j++) {
			Re[p + j] = Buf[p + j];
			Im[p + j] = 0.;
		}
		//FFT(&Re[p], &Im[p], N, 9, FT_DIRECT);
		FFT(&Re[p], &Im[p], N, GetLogN(), FT_DIRECT);
	}
	for (auto i = 0; i < BufSize; i++) Buf[i] = 0;
}

// Пересчитываем аудио данные из спектров
void TWavFile::ReCalculate() {
	for (auto i = 0; i < SpectersCount; i++) {
		auto p = i * N;
		//FFT(&Re[p], &Im[p], N, 9, FT_INVERSE);
		FFT(&Re[p], &Im[p], N, GetLogN(), FT_INVERSE);
		for (auto j = 0; j < N; j++) Buf[p + j] = (short)Re[p + j];
	}
}

void TWavFile::Cut(int left, int right, int d_left, int d_right, double kf, double kb) {
	int p;
	// Вычисляем множители
	double *K = new double [N];
	//
	for (auto i = 0; i < left - d_left; i++) {
		if ((i >= 0) && (i < N)) K[i] = kb;
	}
	for (auto i = left - d_left; i < left; i++) {
		//ShowMessage("Point 1");
		if ((i >= 0) && (i < N)) K[i] = kb + (kf - kb) * (i - (left - d_left)) / d_left;
	}
	for (auto i = left; i <= right; i++) {
		if ((i >= 0) && (i < N)) K[i] = kf;
	}
	for (auto i = right + 1; i < right + d_right; i++) {
		//ShowMessage("Point 2");
		if ((i >= 0) && (i < N)) K[i] = kf + (kb - kf) * (i - right) / d_right;
	}
	for (auto i = right + d_right; i < N; i++) {
		if ((i >= 0) && (i < N)) K[i] = kb;
	}
	for (auto i = 0; i < SpectersCount; i++) {
		for (auto j = 0; j < N; j++) {
			p = i * N + j;
			Re[p] = Re[p] * K[j];
			Im[p] = Im[p] * K[j];
		}
	}
	delete[] K;
}

// Сдвиг спектра вправо или влево
void TWavFile::Shift(int val) {
	if (val == 0) return;
	int p = 0;
	for (auto i = 0; i < SpectersCount; i++) {
		p = i * N;
		if (val > 0) { // Сдвиг вправо
			for (auto j = N - 1; j > 0; j--) {
				if (j < val) {
					Re[p + j] = 0.;
					Im[p + j] = 0.;
				}
				else {
					Re[p + j] = Re[p + j - val];
					Im[p + j] = Im[p + j - val];
				}
			}
		}
		else { // Сдвиг влево
			for (auto j = 0; j < N; j++) {
				if (j < -val) {
					Re[p + j] = Re[p + j - val];
					Im[p + j] = Im[p + j - val];
				}
				else {
					Re[p + j] = 0.;
					Im[p + j] = 0.;
				}
			}

		}
	}
}

void TWavFile::Move() {
}

void TWavFile::Copy(int left, int right, double amp, int sm) {
    // Если нет изменений
    if ((sm == 0) && (amp == 1.0)) return;
    // Цикл по спектрам
    double Im2[N], Re2[N];
    int k, p;
	for (auto i = 0; i < SpectersCount; i++) {
		p = i * N;
        // Ищем добавки
        for (auto j = 0; j < N; j++) {
            k = j - sm;
            if ((k < 0) || (k >= N)) {
                Re2[j] = 0.;
                Im2[j] = 0.;
            }
            else {
	            Re2[j] = amp * Re[p + k];
	            Im2[j] = amp * Im[p + k];
            }
        }
        // Добавляем
        for (auto j = 0; j < N; j++) {
            Re[p + j] += Re2[j];
            Im[p + j] += Im2[j];
        }
	}
}

void TWavFile::Resize(double ratio) {
    // Если нет изменений
    if (ratio == 1.0) return;

    ratio = 2.0;
    // Делаем копии массивов
    double *Re2, *Im2;
    int m = N * SpectersCount;
    Re2 = new double [m];
    Im2 = new double [m];
    for (auto i = 0; i < m; i++) {
        Re2[i] = Re[i];
        Im2[i] = Im[i];
    }
    delete[] Re;
    delete[] Im;
    int SpectersCount2 = (int)(ratio * SpectersCount);
    int m2 = N * SpectersCount2;
    Re = new double [m2];
    Im = new double [m2];
    for (auto i = 0; i < SpectersCount2; i++) {
        int p = i * N;
        int p2 = ((int)(1. * i / ratio)) * N;
        for (auto j = 0; j < N; j++) {
	        Re[p + j] = Re2[p2 + j];
	        Im[p + j] = Im2[p2 + j];
        }
    }
    SpectersCount = SpectersCount2;
    delete[] Re2;
    delete[] Im2;

    BufSize *= 2;
    delete[] Buf;
    Buf = new short [BufSize];
}

int TWavFile::GetLogN(void) {
	switch (N) {
		case   256: return 8;
		case   512: return 9;
		case  1024: return 10;
		case  2048: return 11;
		case  4096: return 12;
		case  8192: return 13;
		default:    return 0;
	}
}

