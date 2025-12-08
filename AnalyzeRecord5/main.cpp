#include <vcl.h>
//#include <tchar.h>
#include <wchar.h>
#include <windows.h>
#include <iostream>
#include <System.Math.hpp>
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
int
	//N = 8192,
    N = 2048,
	N2 = N / 2, Count, d = 0, id, fpos, param = 0, windows_size = 1024, shape_type = 1;
double dif, same;
double Min[TVoiceSpectersDB4_M], Max[TVoiceSpectersDB4_M], Middle[TVoiceSpectersDB4_M];
UnicodeString s;
// Анализ аргументов командной строки
if (argc != 3) {
	cout << "Usage: AnalyzeRecord5.exe BaseFile AudioFile" << endl
		<<  "Example: AnalyzeRecord5.exe D:\\result\\voice1.vdb5 D:\\Data\\record1.wav " << endl;
		return 1;
}
// Вычисление всех спектров
class SpecterWork *sw = new class SpecterWork;

//sw->ReadDataFromDAT(dat_fname);
sw->ReadDataFromWAV(UnicodeString(argv[2]));
// Нахождение спектров
sw->CountSpectersThread(N, true, 1200, 0, sw->BufSize, windows_size, shape_type);
// Находим средний спектр
sw->CountMiddleValues(0, sw->Count, TVoiceSpectersDB4_M, Min, Max, Middle);
// Добавляем спектр в вектор звука
TVoiceSpectersDB4_Sound Sound;
Sound.Add(Min, Max, Middle);
// Загрузка базы спектров
TVoiceSpectersDB4 *db = new class TVoiceSpectersDB4;
db->Load(UnicodeString(argv[1]));
//cout << "db->Count=" << db->Count << endl;
TStringList *ts = new TStringList;
TBallsSum bs;
db->Search(Sound.Specters, 0, 1, 1, bs);
for (auto i = 0; i < bs.N; i++) {
    //ts->Add(bs.Sound[bs.Order[i]] + "\t" + UnicodeString(std::round(bs.Balls[bs.Order[i]])));
    ts->Add(bs.Sound[bs.Order[i]] + "\t" + UnicodeString((double)System::Math::RoundTo(bs.Balls[bs.Order[i]], -3)));
}
// Сохраняем результат
ts->SaveToFile("result.dat", TEncoding::UTF8);
//ShowMessage(ts->Text);
// Удаляем ненужные объекты
delete ts;
delete sw;
delete db;
//delete bs;
// Возвращаем код ошибки
return 0;
}


