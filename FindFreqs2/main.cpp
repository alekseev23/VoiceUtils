#include <vcl.h>
#include <tchar.h>
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/strings.h"
#include "../Common/SpecterWork.h"

#define _N 8192
#define _K 9

#define _CHARS 31
#define _RANGES 9

//bool SpecterType = false; // Обычные спектры
bool SpecterType = true; // Энергетические спектры

class TSoundAnalyzer {
public:
    UnicodeString Chars[_CHARS] = {"А", "Б", "В", "Г", "Д", "Е", "Ё", "Ж", "З", "И",
    	"Й", "К", "Л", "М", "Н", "О", "П", "Р", "С", "Т", "У", "Ф", "Х", "Ц",
        "Ч", "Ш", "Щ", "Ы", "Э", "Ю", "Я"};
    int Min[_CHARS][_RANGES], Max[_CHARS][_RANGES], Middle[_CHARS][_RANGES], Count[_CHARS];

    TSoundAnalyzer() {
     	Clear();
    }
    ~TSoundAnalyzer() {}

    void Clear(void) {
        for (auto i = 0; i < _CHARS; i++) {
            Count[i] = 0;
            for (auto j = 0; j < _RANGES; j++) {
                Min[i][j] = 10000;
                Max[i][j] = 0;
                Middle[i][j] = 0;
            }
        }
    }

    void Add(UnicodeString data_line) {
        UnicodeString sound;
        int val, k;
        sound = getFieldFromString(0, data_line, "\t");
        for (k = 0; k < _CHARS; k++) {
            if (sound == Chars[k]) break; // В k - индекс звука
        }
        for (auto i = 0; i < _RANGES; i++) {
            val = getFieldFromString(i + 2, data_line, "\t").ToIntDef(0);
            if (val < Min[k][i]) Min[k][i] = val;
            if (val > Max[k][i]) {
    	        Max[k][i] = val;
            }
            Middle[k][i] += val;
std::cout << k << "\t" << i << "\tval=" << val << "\tMin[k][i]=" << Min[k][i]
<< "\tMax[k][i]=" << Max[k][i] << "\tMiddle[k][i]=" << Middle[k][i] << std::endl;
        }
        Count[k] += 1;
    }

    void Save(UnicodeString fname) {
        TStringList *ts = new TStringList;
        UnicodeString s;
        for (auto i = 0; i < _CHARS; i++) {
            s = Chars[i];
            for (auto j = 0; j < _RANGES; j++) {
                s = s + "\t" + UnicodeString(Min[i][j]) + "\t" + UnicodeString(Max[i][j])
                	+ "\t" + UnicodeString(Middle[i][j] / Count[i]);
            }
            ts->Add(s);
        }
        ts->SaveToFile(fname, TEncoding::UTF8);
        delete ts;
    }

};

int _tmain(int argc, _TCHAR* argv[])
{
	// Объявление переменных
	int Id, pos[_K], pos2[_K], _N2, dif, min_dif = 1000000;
	UnicodeString s, DirName, BaseName, AudioName, Sound;
    double percent[9] = {10, 20, 30, 40, 50, 60, 70, 80, 90};
    double Max[_N], Min[_N], Middle[_N];
    _N2 = _N / 2;
	// Анализ аргументов командной строки
	if (argc != 4) {
		std::cout << "Usage: FindFreqs2.exe make DirName BaseFile" << std::endl
			<< "Example: FindFreqs2.exe make D:\\voice D:\\Data\\base.csv " << std::endl
            << std::endl
            << "Usage: FindFreqs2.exe search ResultFile AudioName" << std::endl
			<< "Example: FindFreqs2.exe search D:\\Data\\base.csv D:\\voice\\1.wav" << std::endl
            << std::endl
            << "Usage: FindFreqs2.exe search2 ResultFile AudioName" << std::endl
			<< "Example: FindFreqs2.exe search2 D:\\Data\\base.csv D:\\voice\\1.wav" << std::endl;
			return 1;
	}
    if (UnicodeString(argv[1]) == "make") {
		// Получение списка WAV файлов
		DirName = UnicodeString(argv[2]);
		BaseName = UnicodeString(argv[3]);
		TStringList *ts = new TStringList;
		TStringList *ts2 = new TStringList;
		GetFilesFromDirectory(DirName, "*.wav", false, false, ts);
		ts->Sorted = true;
		// Создание объекта работы со спектрами
		class SpecterWork *sw = new class SpecterWork;
		// Цикл по списку
		for (auto i = 0; i < ts->Count; i++) {
			// Вычисляем спектр
			sw->ReadDataFromWAV(ts->Strings[i]);
			// Нахождение спектров
		    sw->CountSpectersThread(_N, SpecterType, sw->BufSize);
			// Сохранение буквы (звука) (4 байта)
			s = without_extension(get_file_name(ts->Strings[i]));
			Sound = getFieldFromString(0, s, "_");
			// Сохранение номера (id) варианта буквы
			Id = (char)getFieldFromString(1, s, "_").ToIntDef(0);
			// Средние значения
	        sw->CountMiddleValues(0, sw->Count, _N2, Min, Max, Middle);
        	// Вычисления критических значений
	        sw->GetPercentsPos(Middle, _N2, percent, pos, _K);
	        s = Sound + "\t" + Id;
	        for (auto j = 0; j < _K; j++) s = s + "\t" + UnicodeString(pos[j]);
	        ts2->Add(s);
	        std::wcout << s << std::endl;
		}
	    // Сохраняем результат
	    ts2->SaveToFile(BaseName, TEncoding::UTF8);
        // Ещё один результат
        class TSoundAnalyzer *Analyzer = new class TSoundAnalyzer;
        for (auto i = 0; i < ts->Count; i++) {
            Analyzer->Add(ts2->Strings[i]);
        }
        Analyzer->Save(str_replace(".dat", ".csv", BaseName));
        delete Analyzer;

		// Удаляем ненужные объекты
		delete sw;
		delete ts;
		delete ts2;
		// Возвращаем код ошибки
		return 0;
    }
    else if (UnicodeString(argv[1]) == "search") {
		BaseName = UnicodeString(argv[2]);
		AudioName = UnicodeString(argv[3]);
		TStringList *ts = new TStringList;
		TStringList *ts2 = new TStringList;
        if (!FileExists(BaseName)) {
	        ShowMessage("File not found: " + BaseName);
            return 4;
        }
        ts->LoadFromFile(BaseName, TEncoding::UTF8);
        //ShowMessage(ts->Text);
		// Создание объекта работы со спектрами
		class SpecterWork *sw = new class SpecterWork;
		// Вычисляем спектр
		sw->ReadDataFromWAV(AudioName);
        // Нахождение спектров
		sw->CountSpectersThread(_N, SpecterType, sw->BufSize);
		// Средние значения
        sw->CountMiddleValues(0, sw->Count, _N2, Min, Max, Middle);
       	// Вычисления критических значений
        sw->GetPercentsPos(Middle, _N2, percent, pos, _K);
        // Цикл по записям в таблице
        for (auto i = 0; i < ts->Count; i++) {
            s = ts->Strings[i];
	        // Находим все коэффициенты
            dif = 0;
            for (auto j = 0; j < _K; j++) {
                pos2[j] = getFieldFromString(2 + j, s, "\t").ToIntDef(0);
                //dif += abs(pos2[j] - pos[j]);
                dif += (pos2[j] - pos[j]) * (pos2[j] - pos[j]);
            }
                Sound = getFieldFromString(0, s, "\t");
                Id = getFieldFromString(1, s, "\t").ToIntDef(0);
                UnicodeString t;
                t.printf(L"%8i", dif);
                t = t + "\t" + Sound + "\t" + UnicodeString(Id);
                ts2->Add(t);
    	    // Сравниваем
            if (dif < min_dif) {
	        	// Если разница меньше, чем была раньше, то запоминаем новую разницу и имя звука
                std::wcout << t << std::endl;
                //ts2->Add(t);
                min_dif = dif;
            }
        }
        // Выводим результат
        ts2->Sort();
        ts2->SaveToFile("search.res", TEncoding::UTF8);
		// Удаляем ненужные объекты
		delete sw;
        delete ts;
        delete ts2;
		// Возвращаем код ошибки
		return 0;
    }
    else if (UnicodeString(argv[1]) == "search2") {
		BaseName = UnicodeString(argv[2]);
		AudioName = UnicodeString(argv[3]);
		TStringList *ts = new TStringList;
		TStringList *ts2 = new TStringList;
        if (!FileExists(BaseName)) {
	        ShowMessage("File not found: " + BaseName);
            return 4;
        }
        ts->LoadFromFile(BaseName, TEncoding::UTF8);
        //ShowMessage(ts->Text);
		// Создание объекта работы со спектрами
		class SpecterWork *sw = new class SpecterWork;
		// Вычисляем спектр
		sw->ReadDataFromWAV(AudioName);
        // Нахождение спектров
		sw->CountSpectersThread(_N, SpecterType, sw->BufSize);
		// Средние значения
        sw->CountMiddleValues(0, sw->Count, _N2, Min, Max, Middle);
       	// Вычисления критических значений
        sw->GetPercentsPos(Middle, _N2, percent, pos, _K);
        // Цикл по записям в таблице
        for (auto i = 0; i < ts->Count; i++) {
            s = ts->Strings[i];
	        // Находим все коэффициенты
            dif = 0;
            for (auto j = 0; j < _K; j++) {
                pos2[j] = getFieldFromString(3 + j * 3, s, "\t").ToIntDef(0);
                //dif += abs(pos2[j] - pos[j]);
                dif += (pos2[j] - pos[j]) * (pos2[j] - pos[j]);
            }
                Sound = getFieldFromString(0, s, "\t");
                //Id = getFieldFromString(1, s, "\t").ToIntDef(0);
                UnicodeString t;
                t.printf(L"%8i", dif);
                t = t + "\t" + Sound;
                ts2->Add(t);
        }
        // Выводим результат
        ts2->Sort();
        ts2->SaveToFile("search2.res", TEncoding::UTF8);
		// Удаляем ненужные объекты
		delete sw;
        delete ts;
        delete ts2;
		// Возвращаем код ошибки
		return 0;
    }
    std::cout << "Bad syntax" << std::endl;
    return 2;
}
