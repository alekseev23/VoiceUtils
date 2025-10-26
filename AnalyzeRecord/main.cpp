#include <vcl.h>
//#include <tchar.h>
#include <wchar.h>
#include <windows.h>
#include <iostream>
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/strings.h"
#include "../Common/common.h"
#include "../Common/TVoiceSpecters.h"
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

SetConsoleOutputCP(1251);
SetConsoleCP(1251);

int N2 = 256, m = 80, scale = 1, n;
double dif;
wchar_t Char;
char w[2] = {0, 0};
char Id;
// Анализ аргументов командной строки
if (argc != 3) {
	cout << "Usage: AnalyzeRecord.exe BaseFile AudioFile" << endl
		<<  "Example: AnalyzeRecord.exe D:\\result\\voice1.vdb D:\\Data\\record1.mp3 " << endl;
		return 1;
}
// Загрузка базы спектров
TVoiceSpecters *VoiceSpecters = new TVoiceSpecters;
VoiceSpecters->Load(UnicodeString(argv[1]));
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
sw->CountAllSpecters(2 * N2);
// Цикл по спектрам

fd = _wfopen(L"specters.dat", L"wb");

for (auto i = 0; i < sw->Count; i += m / 2) {
	// Усреднение спектров
	sw->GetMiddle(scale, i, m);
	// Поиск похожих спектров
	//dif = VoiceSpecters->Search(sw->Middle, Char, Id, n);
    dif = VoiceSpecters->Search1(sw->Middle, Char, Id, n, 3);
fwrite(sw->Middle, 8, N2, fd);
	w[0] = (char)(Char - 0x0410 + 192);
	if (w[0] == (char)177) w[0] = (char)168;
	// Вывод результатов поиска
	cout << "i=" << i << "\tdif=" << dif << "\t" << w << "\t" << (int)(Char - 0x0410) << "\tid=" << (int)Id << "\tn=" << n << endl;
}
fclose(fd);
// Удаляем ненужные объекты
delete[] Data;
delete sw;
delete VoiceSpecters;
// Возвращаем код ошибки
return 0;
}
