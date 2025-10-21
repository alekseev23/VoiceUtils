#include <vcl.h>
#include <tchar.h>
#include "../RecordChars/SpecterWork.h"
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/strings.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
UnicodeString s, c, DirName, DBName;
int N2 = 256;
// Анализ аргументов командной строки
if (argc != 3) {
	cout << "Usage: MakeVoiceDatabase.exe SourceDir ResultBase" << endl
		<<  "Example: MakeVoiceDatabase.exe D:\\Data\\Voice1 D:\\result\\voice1.vdb" << endl;
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
	sw->ReadData(ts->Strings[i]);
	// Нахождение спектров
	sw->CountSpecters(N2 * 2);
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
