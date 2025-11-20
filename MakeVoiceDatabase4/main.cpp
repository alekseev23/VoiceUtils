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

int _tmain(int argc, _TCHAR* argv[])
{
	UnicodeString s, c, DirName, DBName;
	int N = 8192, N2, M = 320, d = 480, Id, count;
	double v;
	UnicodeString Sound;
	double Min[TVoiceSpectersDB4_M], Max[TVoiceSpectersDB4_M], Middle[TVoiceSpectersDB4_M];

	N2 = N / 2;
	// Анализ аргументов командной строки
	if (argc != 3) {
		cout << "Usage: MakeVoiceDatabase4.exe SourceDir ResultBase" << endl
			<<  "Example: MakeVoiceDatabase4.exe D:\\Data\\Voice1 D:\\result\\voice1.vdb5" << endl;
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
	db->SetSize(ts->Count);
	// Цикл по списку
	for (auto i = 0; i < ts->Count; i++) {
		// Вычисляем спектр
		sw->ReadDataFromWAV(ts->Strings[i]);
		// Нахождение спектров
	    sw->CountSpectersThread(N, false, sw->BufSize);
		// Сохранение буквы (звука) (4 байта)
		s = without_extension(get_file_name(ts->Strings[i]));
		Sound = getFieldFromString(0, s, "_");
		// Сохранение номера (id) варианта буквы
		Id = (char)getFieldFromString(1, s, "_").ToIntDef(0);
		// Сохранение спектров
	    count = sw->BufSize / d;
	    db->Sounds[i].Set(Sound, Id);
std::cout << "BufSize=" << sw->BufSize << "\t count=" << count << std::endl;
	    // Цикл по спектрам
	    for (auto j = 0; j < count; j++) {
            /*
    	    // Инициализируем значения
        	for (auto k = 0; k < TVoiceSpectersDB4_M; k++) {
	            Min[k] = 1e8;
	            Max[k] = Middle[k] = 0.;
	        }
	        // Считаем значения
	        // Цикл по группе спектров
	        for (auto g = 0; g < d; g++) {
	    	    for (auto k = 0; k < TVoiceSpectersDB4_M; k++) {
	                v = sw->Data[(j * d + g) * sw->N2 + k];
	                if (v < Min[k]) Min[k] = v;
	                if (v > Max[k]) Max[k] = v;
	                Middle[k] += v;
	        	}
	        }
	        // Усредняем
	        for (auto k = 0; k < TVoiceSpectersDB4_M; k++) Middle[k] /= d;
            */
	        sw->CountMiddleValues(j * d, d, TVoiceSpectersDB4_M, Min, Max, Middle);
	        // Добавляем спектр в базу
	        db->Sounds[i].Add(Min, Max, Middle);
	    }
	}
	db->Save(DBName);
	delete db;
	// Удаляем ненужные объекты
	delete sw;
	delete ts;
	// Возвращаем код ошибки
	return 0;
}
