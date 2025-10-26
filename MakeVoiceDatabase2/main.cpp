#include <vcl.h>
#include <tchar.h>
#include <algorithm> // For std::sort
#include <iostream>
#include <vector>
#include "../Common/SpecterWork.h"
#include "../Common/TVoiceSpecters.h"
#include "../Common/TVoiceSpectersDB.h"
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/strings.h"

using namespace std;


struct MyObject {
	double value;
	short freq;

	// Overload the less-than operator for default sorting by 'value'
	bool operator<(const MyObject& other) const {
		return value > other.value;
    }
};



/*
A[n] - входной спектр
*/
int Analyze(int n, double *A, int max_m, short *f, short *a) {
	double max = 0.;
	short *F = new short [n];
	double *M = new double [n];
	int k = 0;
	std::vector<MyObject> objects;
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
	int res = objects.length;
	if (res > 10) res = 10;

    return res;
}

int _tmain(int argc, _TCHAR* argv[])
{
UnicodeString s, c, DirName, DBName;
//int N2 = 256;
int N2 = 4096;
// Анализ аргументов командной строки
if (argc != 3) {
	cout << "Usage: MakeVoiceDatabase2.exe SourceDir ResultBase" << endl
		<<  "Example: MakeVoiceDatabase2.exe D:\\Data\\Voice1 D:\\result\\voice1.vdb2" << endl;
		return 1;
}
// Получение списка WAV файлов
DirName = UnicodeString(argv[1]);
DBName = UnicodeString(argv[2]);
TStringList *ts = new TStringList;
GetFilesFromDirectory(DirName, "*.wav", false, false, ts);
ts->Sorted = true;
// Создание объекта базы данных
FILE *fd = _wfopen(DBName.w_str(), L"wb");
// Цикл по списку
class SpecterWork *sw = new class SpecterWork;
for (auto i = 0; i < ts->Count; i++) {
	// Вычисляем спектр
	sw->ReadDataFromWAV(ts->Strings[i]);
	// Нахождение спектров
	sw->CountSpecters4X(N2 * 2, true);
	// Сохранение буквы (звука) (2 байта)
	s = without_extension(get_file_name(ts->Strings[i]));
	c = getFieldFromString(0, s, "_");
	fwrite(c.w_str(), 2, 1, fd);
	// Сохранение номера (id) варианта буквы
	char id = (char)getFieldFromString(1, s, "_").ToIntDef(0);
	fwrite(&id, 1, 1, fd);
	// Сохранение спектров
	fwrite(sw->Main, 8, sw->MainCount * N2, fd);
}
// Закрываем файл
fclose(fd);
// Удаляем ненужные объекты
delete sw;
delete ts;
// Возвращаем код ошибки
return 0;
}
