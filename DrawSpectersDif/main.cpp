#include <vcl.h>
#include <tchar.h>
#include <System.Math.hpp>

#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/strings.h"
#include "../../_Common_/Lib/bitmap.h"
#include "../Common/common.h"
#include "../Common/TVoiceSpectersDB4.h"
#include "../Common/SpecterWork.h"
#include "../Common/TColorSceme.h"
#include "../Common/TBallsSum.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
// Анализ аргументов командной строки
if (argc != 3) {
	cout << "Usage: DrawSpectersDif.exe AudioFile PictureFile" << endl
		<<  "Example: DrawSpectersDif.exe D:\\Data\\record1.wav D:\\result\\record1.png" << endl;
		return 1;
}
int N = 1024, N2, scale = 1, specters_per_second = 48000, windows_size,
	shape_type = 1, y_sm = 2, delta_specters = 480;
bool energy_specters = true, show_grid_lines = true;
//TBitmap *bmp = new TBitmap;
N2 = N / 2;
windows_size = 512;
// Грузим файл
SpecterWork *SpecterWork = new class SpecterWork;
SpecterWork->ReadData(UnicodeString(argv[1]));
// Вычисляем спектры
SpecterWork->CountSpectersThread(N, energy_specters, specters_per_second, 0, SpecterWork->BufSize, windows_size, shape_type);
// Рисуем спектры
//SpecterWork->DrawSpectrograms2(bmp, scale);

int sx, sy, i, yy, SpectersPerSecond10, delta;
unsigned char *p, color;
double val, max, val1, val2;
bool flag;
// Вычисляем тики частоте
SpecterWork->GetFreqPos();
sx = SpecterWork->Count;
sy = N2 * 2 + y_sm;
SpectersPerSecond10 = SpecterWork->SpectersPerSecond / 10;
TBitmap *bmp = new TBitmap;
bmp->PixelFormat = pf24bit;
bmp->SetSize(sx, sy);
//
switch (scale) {
	case 1: max = SpecterWork->MaxValue; break;
	case 2: max = log10(SpecterWork->MaxValue); break;
	case 3: max = Math::LogN(1.042, SpecterWork->MaxValue); break;
}
// Цветовая схема
TColor left_color = clYellow, right_color = clYellow, control_color[4];
control_color[0] = (TColor)(0);
control_color[1] = (TColor)(255);
control_color[2] = (TColor)(255 << 8);
control_color[3] = (TColor)(255 << 16);
double values[4] = {0., 1. * max / 20, 1. * max / 4, max};
//double values[5] = {0., 1. * max / 1000, 1. * max / 100, 1. * max / 10, max};
TColorSceme *ColorSceme = new TColorSceme(left_color, right_color, 4, control_color, values, 1000);
//
unsigned char r, g, b;
for (auto y = 0; y < N2; y++) {
	p = (unsigned char *)bmp->ScanLine[y];
	i = 0;
	yy = N2 - 1 - y;
   	if (show_grid_lines) {
    	if (SpecterWork->FreqLinePos[yy]) {
			for (auto x = 0; x < sx; x++) {
				p[i++] = 30;
				p[i++] = 30;
				p[i++] = 30;
            }
			continue;
        }
    }
	for (auto x = 0; x < sx; x++) {
        // Отрисовка сетки
    	if (show_grid_lines) {
            if (x % SpecterWork->SpectersPerSecond == 0) { // Линия 1 секунда
				p[i++] = 80;
				p[i++] = 80;
				p[i++] = 80;
                continue;
            }
            if (x % SpectersPerSecond10 == 0) { // Линия 0.1 секунды
				p[i++] = 30;
				p[i++] = 30;
				p[i++] = 30;
                continue;
            }
        }
		val = SpecterWork->GetSpecterPointValue(x, yy, scale);
		ColorSceme->Get(val, r, g, b);
		p[i++] = r;
		p[i++] = g;
		p[i++] = b;
	}
}

// Рисуем разницу между спектрами
for (auto y = 0; y < N2; y++) {
	p = (unsigned char *)bmp->ScanLine[y + N2 + y_sm];
	i = delta_specters * 3;
	yy = N2 - 1 - y;
   	if (show_grid_lines) {
    	if (SpecterWork->FreqLinePos[yy]) {
			for (auto x = 0; x < sx; x++) {
				p[i++] = 30;
				p[i++] = 30;
				p[i++] = 30;
            }
			continue;
        }
    }
	for (auto x = delta_specters; x < sx; x++) {
        // Отрисовка сетки
    	if (show_grid_lines) {
            if (x % SpecterWork->SpectersPerSecond == 0) { // Линия 1 секунда
				p[i++] = 80;
				p[i++] = 80;
				p[i++] = 80;
                continue;
            }
            if (x % SpectersPerSecond10 == 0) { // Линия 0.1 секунды
				p[i++] = 30;
				p[i++] = 30;
				p[i++] = 30;
                continue;
            }
        }
		val1 = SpecterWork->GetSpecterPointValue(x - delta_specters, yy, scale);
		val2 = SpecterWork->GetSpecterPointValue(x, yy, scale);
        delta = (int)(3000. * (val1 - val2) / SpecterWork->MaxValue);
        if (delta > 0) {
            if (delta > 255) delta = 255;
			p[i++] = delta;
			p[i++] = 0;
			p[i++] = 0;
        }
        else {
            if (delta < -255) delta = -255;
			p[i++] = 0;
			p[i++] = 0;
			p[i++] = -delta;
        }
	}
}

// Сохраняем изображение
save_bitmap(bmp, UnicodeString(argv[2]), false, 9);
// Удаляем ненужные объекты
delete bmp;
delete SpecterWork;
}
