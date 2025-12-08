#include <vcl.h>
#include <tchar.h>
#include <algorithm> // For std::sort
#include <iostream>
#include <vector>
#include "../Common/SpecterWork.h"
#include "../Common/TSpecterProps.h"
#include "../Common/TVoiceSpecters2.h"
#include "../Common/TVoiceSpectersDB4.h"
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/strings.h"

using namespace std;

#define SOUNDS_N 31
int _tmain(int argc, _TCHAR* argv[])
{
	UnicodeString s, c, DirName, DBName;
	int
    	//N = 8192,
    	N = 2048,
    	N2, M = 320, d = 480, Id, count, SpectersCount, windows_size = 1024, shape_type = 1;
	double v;
	UnicodeString Sound;
    UnicodeString Sounds[SOUNDS_N] = {"А", "Б", "В", "Г", "Д", "Е", "Ё", "Ж", "З", "И", "Й",
    	"К", "Л", "М", "Н", "О", "П", "Р", "С", "Т", "У", "Ф", "Х", "Ц", "Ч", "Ш", "Щ",
        "Ы", "Э", "Ю", "Я"};
	double Min[TVoiceSpectersDB4_M], Max[TVoiceSpectersDB4_M], Middle[TVoiceSpectersDB4_M];
    double Sum[TVoiceSpectersDB4_M];
	N2 = N / 2;
	// Анализ аргументов командной строки
	if (argc != 3) {
		cout << "Usage: MakeVoiceDatabase5.exe SourceDir ResultBase" << endl
			<<  "Example: MakeVoiceDatabase5.exe D:\\Data\\Voice1 D:\\result\\voice1.vdb5" << endl;
			return 1;
	}
	// Получение списка WAV файлов
	DirName = UnicodeString(argv[1]);
	DBName = UnicodeString(argv[2]);
	TStringList *ts = new TStringList;
	GetFilesFromDirectory(DirName, "*.wav", false, false, ts);
	ts->Sorted = true;
	ts->SaveToFile("sounds.list", TEncoding::UTF8);
	// Создание объекта базы данных
	class SpecterWork *sw = new class SpecterWork;
	TVoiceSpectersDB4 *db = new class TVoiceSpectersDB4;
	//db->SetSize(ts->Count);
    db->SetSize(SOUNDS_N);
    // Цикл по звукам
    for (auto k = 0; k < SOUNDS_N; k++) {
        // Очистка суммы сректра
        for (auto j = 0; j < TVoiceSpectersDB4_M; j++) Sum[j] = 0.;
        SpectersCount = 0;
		// Цикл по списку
		for (auto i = 0; i < ts->Count; i++) {
			// Находим звук (4 байта)
			s = without_extension(get_file_name(ts->Strings[i]));
			Sound = getFieldFromString(0, s, "_");
            if (Sound != Sounds[k]) continue; // Если не наш, то пропускаем
			// Сохранение номера (id) варианта буквы
			Id = (char)getFieldFromString(1, s, "_").ToIntDef(0);
			// Вычисляем спектр
			sw->ReadDataFromWAV(ts->Strings[i]);
			// Нахождение спектров
	    	//sw->CountSpectersThread(N, false, sw->BufSize);
            //sw->CountSpectersThread(N, true, sw->BufSize);
            sw->CountSpectersThread(N, true, 1200, 0, sw->BufSize, windows_size, shape_type);

            // Находим средний спектр
            sw->CountMiddleValues(0, sw->Count, TVoiceSpectersDB4_M, Min, Max, Middle);
            // Добавляем средний спектр в сумматор
	        for (auto j = 0; j < TVoiceSpectersDB4_M; j++) Sum[j] += Middle[j];
            SpectersCount++;
		}
        // Вычисляем средний спектр
        for (auto j = 0; j < TVoiceSpectersDB4_M; j++) Sum[j] /= SpectersCount;
        // Добавляем в базу данных
		// Сохранение спектров
		count = sw->BufSize / d;
        db->Sounds[k].Set(Sounds[k], 0);
       	db->Sounds[k].Add(Sum, Sum, Sum);
        std::wcout << "k=" << k << std::endl;
    }
	db->Save(DBName);
	delete db;
	// Удаляем ненужные объекты
	delete sw;
	delete ts;
	// Возвращаем код ошибки
	return 0;
}
