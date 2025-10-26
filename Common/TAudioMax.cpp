//---------------------------------------------------------------------------

#pragma hdrstop

#include "TAudioMax.h"
#include <Math.hpp>
#include "../../_Common_/Lib/files.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//---------------------------------------------------------------------------

TAudioMaxData::TAudioMaxData() {
	for (auto i = 0; i < TAudioMax_N; i++) {
		dx_p[i] = dy_p[i] = dx_m[i] = dy_m[i] = 0;
	}
}

TAudioMaxData::~TAudioMaxData() {
}
//---------------------------------------------------------------------------


TAudioMax::TAudioMax() {
}

TAudioMax::~TAudioMax() {
	if (Data != NULL) delete[] Data;
}

void TAudioMax::Set(short *data, int n) {
	int min_last, max_last, min_pos = -1, max_pos = -1, k, d;
	// Кол-во блоков
	Count = n / Area;
	if (Data != NULL) delete[] Data;
	Data = new struct TAudioMaxData [Count];
	for (auto i = 1; i < n - 1; i++) {
		// Находим точки минимума и максимума
		// Находим текущую область
		k = i / Area;
		// Если концовка, то завершаем
		if (k >= Count) break;
		// Точка максимума
		if ((data[i] > data[i - 1]) && (data[i] > data[i + 1])) {
			if (min_pos != -1) {
				// dx+
				d = i - min_pos;
				if (d < 0) d = 0;
				else if (d >= TAudioMax_N) d = TAudioMax_N - 1;
				Data[k].dx_p[d] += 1;
				// dy+
				//d = (data[i] - min_last) / 200;
				d = data[i] - min_last;
				if (d < 0) d = 0;
				else {
					d = (int)Math::LogN(1.12, 1. * d);
					if (d < 0) d = 0;
				}
				//else if (d >= TAudioMax_N) d = TAudioMax_N - 1;
				Data[k].dy_p[d] += 1;
			}
			max_pos = i;
			max_last = data[i];
		}
		// Точка минимума
		else if ((data[i] < data[i - 1]) && (data[i] < data[i + 1])) {
			if (max_pos != -1) {
				// dx-
				d = i - max_pos;
				if (d < 0) d = 0;
				else if (d >= TAudioMax_N) d = TAudioMax_N - 1;
				Data[k].dx_m[d] += 1;
				// dy-
				//d = (max_last - data[i]) / 200;
				d = max_last - data[i];
				if (d < 0) d = 0;
				else {
					d = (int)Math::LogN(1.12, 1. * d);
					if (d < 0) d = 0;
				}
				//else if (d >= TAudioMax_N) d = TAudioMax_N - 1;
				Data[k].dy_m[d] += 1;
			}
			min_pos = i;
			min_last = data[i];
		}
	}
	// Обнуляем средние значения
	for (auto i = 0; i < TAudioMax_N; i++) {
		DX_P[i] = DY_P[i] = DX_M[i] = DY_M[i] = 0.;
	}
	// Добавляем значения
	for (auto i = 0; i < TAudioMax_N; i++) {
		for (auto j = 0; j < Count; j++) {
			DX_P[i] += Data[j].dx_p[i];
			DY_P[i] += Data[j].dy_p[i];
			DX_M[i] += Data[j].dx_m[i];
			DY_M[i] += Data[j].dy_m[i];
		}
	}
	// Находим средние значения
	for (auto i = 0; i < TAudioMax_N; i++) {
		DX_P[i] /= Count;
		DY_P[i] /= Count;
		DX_M[i] /= Count;
		DY_M[i] /= Count;
	}
}

bool TAudioMax::Save(UnicodeString fname) {
	FILE *fd = _wfopen(fname.w_str(), L"wb");
	fwrite(Data, 1, TAudioMax_N * 4 * Count, fd);
	fclose(fd);
	return true;
}

bool TAudioMax::Load(UnicodeString fname) {
	int fsize = getFileSize(fname);
	if (fsize == 0) return false;
	// Кол-во блоков
	Count = fsize / (TAudioMax_N * 4);
	if (Data != NULL) delete[] Data;
	Data = new struct TAudioMaxData [Count];
	FILE *fd = _wfopen(fname.w_str(), L"rb");
	fread(Data, 1, TAudioMax_N * 4 * Count, fd);
	fclose(fd);
	return true;
}

void TAudioMax::Draw(TBitmap *bmp) {
	TBitmap *bmp2 = new TBitmap;
	int color;
	bmp2->PixelFormat = pf24bit;
	bmp2->SetSize(Count * 4, TAudioMax_N);
	for (auto x = 0; x < Count; x++) {
		for (auto y = 0; y < TAudioMax_N; y++) {
			if (Data[x].dx_p[y] > 0) {
				color = 255 * Data[x].dx_p[y] / 10;
				bmp2->Canvas->Pixels[x][TAudioMax_N - 1 - y] = (TColor)color;
			}
			if (Data[x].dy_p[y] > 0) {
				color = 255 * Data[x].dy_p[y] / 10;
				bmp2->Canvas->Pixels[x + Count * 1][TAudioMax_N - 1 - y] = (TColor)(color + (color << 8));
			}
			if (Data[x].dx_m[y] > 0) {
				color = 255 * Data[x].dx_m[y] / 10;
				bmp2->Canvas->Pixels[x + Count * 2][TAudioMax_N - 1 - y] = (TColor)((color << 8));
			}
			if (Data[x].dy_m[y] > 0) {
				color = 255 * Data[x].dy_m[y] / 10;
				bmp2->Canvas->Pixels[x + Count * 3][TAudioMax_N - 1 - y] = (TColor)((color << 16));
			}
		}
	}
	bmp->Assign(bmp2);
	delete bmp2;
}

void TAudioMax::DrawMiddleValues(TBitmap *bmp) {
	TBitmap *bmp2 = new TBitmap;
	bmp2->PixelFormat = pf24bit;
	bmp2->SetSize(4, TAudioMax_N);
	float val;
	int color;
	for (auto i = 0; i < TAudioMax_N; i++) {
		val = 255. * DX_P[i] / 1;
		color = (int)val;
		if (color > 255) color = 255;
		bmp2->Canvas->Pixels[0][TAudioMax_N - 1 - i] = (TColor)(color);

		val = 255. * DY_P[i] / 1;
		color = (int)val;
		if (color > 255) color = 255;
		bmp2->Canvas->Pixels[1][TAudioMax_N - 1 - i] = (TColor)(color + (color << 8));

		val = 255. * DX_M[i] / 1;
		color = (int)val;
		if (color > 255) color = 255;
		bmp2->Canvas->Pixels[2][TAudioMax_N - 1 - i] = (TColor)((color << 8));

		val = 255. * DY_M[i] / 1;
		color = (int)val;
		if (color > 255) color = 255;
		bmp2->Canvas->Pixels[3][TAudioMax_N - 1 - i] = (TColor)((color << 16));
	}
	bmp->Assign(bmp2);
	delete bmp2;
}

UnicodeString TAudioMax::Print(void) {
	UnicodeString s, ss;
	char w[100];
	for (auto i = 0; i < TAudioMax_N; i++) {
		ss = "";
		for (auto j = 0; j < Count; j++) {
			sprintf(w, "%3i,%3i,%3i,%3i   ", Data[j].dx_p[i], Data[j].dy_p[i], Data[j].dx_m[i], Data[j].dy_m[i]);
			ss = ss+ UnicodeString(w);
		}
		s = s + ss + "\r\n";
	}
	return s;
}

UnicodeString TAudioMax::PrintMiddleValues(void) {
	UnicodeString s;
	char w[100];
	for (auto i = 0; i < TAudioMax_N; i++) {
		sprintf(w, "%3i %6.3f %6.3f %6.3f %6.3f", i, DX_P[i], DY_P[i], DX_M[i], DY_M[i]);
		s = s + UnicodeString(w) + "\r\n";
	}
	return s;
}

int TAudioMax::Compare() {

}


