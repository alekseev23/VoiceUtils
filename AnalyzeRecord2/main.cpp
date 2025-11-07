#include <vcl.h>
//#include <tchar.h>
#include <wchar.h>
#include <windows.h>
#include <iostream>
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/strings.h"
#include "../Common/common.h"
#include "../Common/TVoiceSpectersDB.h"
#include "../Common/SpecterWork.h"

using namespace std;

//#pragma execution_character_set( "utf-8" )
//------------------------------------------------------------------------------
bool Convert(UnicodeString SOX, UnicodeString in, UnicodeString out) {
UnicodeString BITS = "16", CHANNELS = "1", FREQ = "48000", command;
command = SOX + " \"" + in + "\" --bits " + BITS
	+ " --channels " + CHANNELS
	+ " --encoding signed-integer --rate " + FREQ
	+ " --type raw \"" + out +"\"";
_wsystem(command.w_str());
wcout << command.w_str() << endl;
return true;
}

//------------------------------------------------------------------------------
//int _tmain(int argc, _TCHAR* argv[])
int wmain(int argc, wchar_t* argv[])
{
/*
// Кодировка самого файла болжна быить UTF8!
printf( u8"Testing unicode -- English -- ???????? -- Espa?ol -- Русский. a?bcdefghijklmno?pqrs?tu?vwxyz\n" );
wcout << u8"Привет буква Ё!" << endl;
wcout << u8"Hello, world! ??" << endl;
*/
// Set console input code page to UTF-8
/*if (!SetConsoleCP(CP_UTF8)) {
	std::cerr << "Error setting console input code page: " << GetLastError() << std::endl;
	return 1;
}
// Set console output code page to UTF-8
//if (!SetConsoleOutputCP(CP_UTF8)) {
if (!SetConsoleOutputCP(65001)) {
	std::cerr << "Error setting console output code page: " << GetLastError() << std::endl;
	return 1;
} */

//setlocale(LC_ALL, "");


/*SetConsoleOutputCP(1251);
SetConsoleCP(1251);
printf("Testing unicode -- English -- ???????? -- Espa?ol -- Русский. a?bcdefghijklmno?pqrs?tu?vwxyz\n" );
cout << "Привет буква Ё!" << endl;
wprintf(L"Testing unicode -- English -- ???????? -- Espa?ol -- Русский. a?bcdefghijklmno?pqrs?tu?vwxyz\n" );
wcout << L"Привет буква Ё!" << endl;
wcout << L"Hello, world! ??" << endl;
return 0;*/

//SetConsoleOutputCP(1251);
//SetConsoleCP(1251);
SetConsoleCP(CP_UTF8);
SetConsoleOutputCP(65001);

int N = 8192, N2 = N / 2, m = 80, scale = 1, n, delta = 0;
double dif;
wchar_t Char;
char w[2] = {0, 0};
char Id;
// Анализ аргументов командной строки
if (argc != 3) {
	cout << "Usage: AnalyzeRecord2.exe BaseFile AudioFile" << endl
		<<  "Example: AnalyzeRecord2.exe D:\\result\\voice1.vdb D:\\Data\\record1.mp3 " << endl;
		return 1;
}
// Загрузка базы спектров
TVoiceSpectersDB *VoiceSpectersDB = new TVoiceSpectersDB;
VoiceSpectersDB->Load(UnicodeString(argv[1]));
// Преобразование адио файла (MP3 или WAV) в DAT
UnicodeString audio_fname, source_fname, dat_fname;
// Конвертируем аудио в DAT
audio_fname = UnicodeString(argv[2]);
source_fname = without_extension(audio_fname);
dat_fname = source_fname + ".dat";
Common Common;
UnicodeString SOX = Common.GetSOX();
Convert(SOX, audio_fname, dat_fname);
// Загружаем данные
int fsize = getFileSize(dat_fname); // Находим размер файла
int items = fsize / 2;  // Кол-во элементов
FILE *fd = _wfopen(dat_fname.w_str(), L"rb");
short *Data = new short [items];
fread(Data, 2, items, fd);
fclose(fd);
// Вычисление всех спектров
class SpecterWork *sw = new class SpecterWork;
sw->ReadDataFromDAT(dat_fname);
sw->Normalize();
sw->CountAllSpecters4X(N, true);
// Цикл по спектрам
struct TVoiceSpectersDB_One_Specter OSpecter;
std::vector<TVoiceSpecters_Search> result;
fd = _wfopen(L"result.txt", L"wt");
TStringList *ts = new TStringList;
for (auto i = 0; i < sw->Count; i += m / 2) {
	wcout << "i=" << i << endl;
	fwprintf(fd, L"i=%i\n", i);
	ts->Add("i=" + UnicodeString(i));
	// Усреднение спектров
	sw->GetMiddle(scale, i, m);
	// Переход к набору максимумов
	VoiceSpectersDB->Analyze(N2, sw->Middle, &OSpecter);
	// Поиск похожих спектров
	VoiceSpectersDB->Search(&OSpecter, result, delta);
	//wcout << result.size() << endl;
	// Вывод результатов поиска
	for (auto j = 0; j < 10; j++) {
		wcout << "\tj=" << j << "\tvalue=" << result[j].value
			<< "\t" << VoiceSpectersDB->VoiceSpecters[result[j].specter].Sound
			<< "\tsound=" << result[j].specter << "\tnum=" << result[j].num << endl;
		fwprintf(fd, L"\tj=%i\tvalue=%i\t%ls\tsound=%i\tnum=%i\n",
			j, result[j].value, VoiceSpectersDB->VoiceSpecters[result[j].specter].Sound.w_str(),
			result[j].specter, result[j].num);
		ts->Add("\tj=" + UnicodeString(j) +"\tvalue=" + UnicodeString(result[j].value) +"\t"
			+ VoiceSpectersDB->VoiceSpecters[result[j].specter].Sound + "\tsound="
			+ UnicodeString(result[j].specter) +"\tnum=" + UnicodeString(result[j].num));
	}
	//cout << "i=" << i << "\tdif=" << dif << "\t" << w << "\t" << (int)(Char - 0x0410) << "\tid=" << (int)Id << "\tn=" << n << endl;
}
// Удаляем ненужные объекты
fclose(fd);
ts->SaveToFile("result.dat", TEncoding::UTF8);
delete ts;
delete[] Data;
delete sw;
delete VoiceSpectersDB;
// Возвращаем код ошибки
return 0;
}
