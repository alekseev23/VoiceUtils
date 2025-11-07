#include <vcl.h>
#include <tchar.h>
#include <algorithm> // For std::sort
#include <iostream>
#include <vector>
#include "../Common/SpecterWork.h"
#include "../Common/TSpecterProps.h"
#include "../Common/TVoiceSpecters2.h"
#include "../Common/TVoiceSpectersDB.h"
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/strings.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
UnicodeString s, c, DirName, DBName;
//int N2 = 256;
//int N2 = 4096;
int N2 = 1024;
// Анализ аргументов командной строки
if (argc != 3) {
	cout << "Usage: MakeVoiceDatabase3.exe SourceDir ResultBase" << endl
		<<  "Example: MakeVoiceDatabase3.exe D:\\Data\\Voice1 D:\\result\\voice1.vdb4" << endl;
		return 1;
}
// Получение списка WAV файлов
DirName = UnicodeString(argv[1]);
DBName = UnicodeString(argv[2]);
TStringList *ts = new TStringList;
TStringList *ts2 = new TStringList;
double *data = new double [N2];
GetFilesFromDirectory(DirName, "*.wav", false, false, ts);
ts->Sorted = true;
// Создание объекта базы данных
FILE *fd = _wfopen(DBName.w_str(), L"wb");
// Цикл по списку
class SpecterWork *sw = new class SpecterWork;
class TSpecterProps *sp = new class TSpecterProps;
for (auto i = 0; i < ts->Count; i++) {
	// Вычисляем спектр
	sw->ReadDataFromWAV(ts->Strings[i]);
	// Нахождение спектров
	//sw->CountSpecters4X(N2 * 2, true);
	sw->CountSpecters(N2 * 2, 400);
	sw->CountMainSpecters(5);
	// Сохранение буквы (звука) (4 байта)
	s = without_extension(get_file_name(ts->Strings[i]));
	c = getFieldFromString(0, s, "_");
	//fwrite(c.w_str(), 2, 1, fd);
	wchar_t SoundChar[10] = {0, 0, 0};
	wcscpy(SoundChar, c.w_str());
	fwrite(SoundChar, 2, 2, fd);
	// Сохранение номера (id) варианта буквы
	char id = (char)getFieldFromString(1, s, "_").ToIntDef(0);
	fwrite(&id, 1, 1, fd);
	// Сохранение спектров
	fwrite(sw->Main, 8, sw->MainCount * N2, fd);

	// Добавляем информацию о звуке
	UnicodeString Man;
	Man = get_file_name(get_dir_name(ts->Strings[i]));
	for (auto j = 2; j < 3; j++) {
		//double *data = &sw->Main[j * N2];
		sw->GetEnergySpecter(&sw->Main[j * N2], data);
		s = UnicodeString(SoundChar) + "\t" + UnicodeString((int)id) + "\t" + Man + "\t" + UnicodeString(j) + "\t";
		// Общий подсчёт
		sp->GlobalAnalyze(N2, data);
		s = s + UnicodeString(sp->Max) + "\t" + UnicodeString(sp->Sum) + "\t" + UnicodeString(sp->Middle) + "\t";
		sp->Analyze(0, 79, data);
		s = s + UnicodeString(sp->Max) + "\t" + UnicodeString(sp->Sum) + "\t" + UnicodeString(sp->Middle) + "\t";
		sp->Analyze(80, N2 - 1, data);
		s = s + UnicodeString(sp->Max) + "\t" + UnicodeString(sp->Sum) + "\t" + UnicodeString(sp->Middle) + "\t";
		s = str_replace(",", ".", s);
		ts2->Add(s);
	}
}
// Закрываем файл
fclose(fd);
ts2->SaveToFile("result.csv", TEncoding::UTF8);
// Удаляем ненужные объекты
delete sw;
delete sp;
delete ts;
delete ts2;
delete[] data;
// Возвращаем код ошибки
return 0;
}
