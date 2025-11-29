#include <vcl.h>
//#include <tchar.h>
#include <wchar.h>
#include <windows.h>
#include <iostream>
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/strings.h"
#include "../Common/common.h"
#include "../Common/TVoiceSpectersDB4.h"
#include "../Common/SpecterWork.h"
#include "../Common/TBallsSum.h"
using namespace std;

//------------------------------------------------------------------------------
int wmain(int argc, wchar_t* argv[])
{
//Используем кодировку UTF-8
SetConsoleCP(CP_UTF8);
SetConsoleOutputCP(65001);
// Объявление переменных
int N = 8192, N2 = N / 2, Count, d = 3, id, fpos, param = 3;
double dif, same;
UnicodeString s;
// Анализ аргументов командной строки
if (argc != 3) {
	cout << "Usage: AnalyzeRecord4.exe BaseFile AudioFile" << endl
		<<  "Example: AnalyzeRecord4.exe D:\\result\\voice1.vdb5 D:\\Data\\record1.wav " << endl;
		return 1;
}
// Вычисление всех спектров
class SpecterWork *sw = new class SpecterWork;
//sw->ReadDataFromDAT(dat_fname);
sw->ReadDataFromWAV(UnicodeString(argv[2]));
sw->CountSpectersThread(N, false, sw->BufSize);
// Кол-во групповых спектров
Count = sw->Count / 480;
// Вычисляем групповые спектры
TVoiceSpectersDB4_Sound Sound;
double Min[TVoiceSpectersDB4_M], Max[TVoiceSpectersDB4_M], Middle[TVoiceSpectersDB4_M];
// Цикл по спектрам
for (auto j = 0; j < Count; j++) {
	sw->CountMiddleValues(j * 480, 480, TVoiceSpectersDB4_M, Min, Max, Middle);
	// Добавляем спектр в вектор звука
	Sound.Add(Min, Max, Middle);
}
// Загрузка базы спектров
TVoiceSpectersDB4 *db = new class TVoiceSpectersDB4;
db->Load(UnicodeString(argv[1]));

//cout << "db->Count=" << db->Count << endl;

TStringList *ts = new TStringList;
TBallsSum bs;
/*
// Цикл по групповым спектрам
for (auto i = 0; i < Count - d; i++) {
    // Поиск наиболее похожего спектра
    s = db->Find(Sound.Specters, i, d, dif, same, id, fpos);
    ts->Add(s + "\tid=" + UnicodeString(id) + "\tsame=" + UnicodeString(same) + "\tdif=" + UnicodeString(dif) + "\tfpos=" + UnicodeString(fpos));

}*/
//db->Search(Sound.Specters, 0, Count - d, d, bs);
db->Search(Sound.Specters, 0 + param, Count - d - param, d, bs);
for (auto i = 0; i < bs.N; i++) {
    ts->Add(bs.Sound[bs.Order[i]] + "\t" + UnicodeString(std::round(bs.Balls[bs.Order[i]])));
}

// Сохраняем результат
ts->SaveToFile("result.dat", TEncoding::UTF8);
ShowMessage(ts->Text);
// Удаляем ненужные объекты
delete ts;
delete sw;
delete db;
//delete bs;
// Возвращаем код ошибки
return 0;
}
