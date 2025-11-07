#include <vcl.h>
#include <tchar.h>
#include "../Common/TVoiceSpecters2.h"
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/RunProcess.h"
#include "../../_Common_/Lib/strings.h"

//#define NG 10
#define NG 6
#define _N2 4096
#define NDB 1

using namespace std;

//---------------------------------------------------------------------------
int FindMiddlePos(double *V, int a, int b) {
	double left = 0., right = 0.;
	int aa, bb;
	aa = a;
	bb = b;
	while (aa < bb) {
		if (left <= right) left += V[aa++];
		else right += V[bb--];
	}
	return aa;
}
//---------------------------------------------------------------------------
double CountSumMax(double *A, int n, double &max) {
	double sum;
	sum = max = 0.;
	for (auto i = 0; i < n; i++) {
		if (A[i] > max) max = A[i];
		sum += A[i];
	}
	return sum;
}
//---------------------------------------------------------------------------

int _tmain(int argc, _TCHAR* argv[])
{
// Создаём рабочие объекты
//UnicodeString Glasnaya[NG] = {"А", "Е", "Ё", "И", "О", "У", "Ы", "Э", "Ю", "Я"};
UnicodeString Glasnaya[] = {"А", "О", "И", "Э", "Ы", "У", "Я", "Е", "Ё", "Ю"};
//UnicodeString DB[NDB] = {
/*UnicodeString DB[3] = {
	"D:\\Work\\CPP\\2025\\VoiceUtils\\bin\\2.0\\data\\Александр.vdb2",
	"D:\\Work\\CPP\\2025\\VoiceUtils\\bin\\2.0\\data\\Алексей.vdb2",
	"D:\\Work\\CPP\\2025\\VoiceUtils\\bin\\2.0\\data\\Татьяна.vdb2"
};*/
UnicodeString DB[3] = {
	"D:\\Work\\CPP\\2025\\VoiceUtils\\MakeVoiceDatabase2\\Win64\\Release\\Александр.vdb2",
	"D:\\Work\\CPP\\2025\\VoiceUtils\\bin\\2.0\\data\\Алексей.vdb2",
	"D:\\Work\\CPP\\2025\\VoiceUtils\\bin\\2.0\\data\\Татьяна.vdb2"
};
short CounterPro[NG][_N2], CounterContra[NG][_N2];
TVoiceSpecters2 *VoiceSpecters2 = new TVoiceSpecters2;
// Инициализируем значения
for (auto i = 0; i < NG; i++) {
	for (auto j = 0; j < _N2; j++) {
		CounterPro[i][j] = 0;
		CounterContra[i][j] = 0;
	}
}

TStringList *ts = new TStringList;
TStringList *ts2 = new TStringList;
TStringList *ts3 = new TStringList;

// Цикл по параметру B
for (auto B = 300; B < 400; B++) {

ts->Clear();
ts2->Clear();

// Цикл по базам со звуками
for (auto d = 0; d < NDB; d++) {
	bool res = VoiceSpecters2->Load(DB[d]);
	//cout << res << endl;
	// Цикл по гласным
	for (auto g = 0; g < NG; g++) {
		// Цикл по звукам в текущей базе
		for (auto z = 0; z < VoiceSpecters2->Count; z++) {
			// Ищем имя звука
			UnicodeString SoundName = VoiceSpecters2->VoiceSpecters[z].Sound;
			bool flag = VoiceSpecters2->VoiceSpecters[z].Sound == Glasnaya[g];
			// Цикл по спектрам (5 штук)
			for (auto s = 0; s < VoiceSpecters2->MainCount; s++) {
				double *data = &VoiceSpecters2->VoiceSpecters[z].Data[s * VoiceSpecters2->N2];
				// Находим максимум
				double max = 0., porog;
				for (auto i = 1; i < VoiceSpecters2->N2; i++) {
					if (data[i] > max) max = data[i];
				}
				porog = max / 100;
				// Ищем пики
				for (auto i = 1; i < VoiceSpecters2->N2; i++) {
					if ((data[i] > data[i - 1]) && (data[i] > data[i + 1]) && (data[i] > porog)) {
						// В зависимости от звука (наша гласная или нет) добавляем в тот или иной счётчик
						if (flag) CounterPro[g][i] += 1;
						else CounterContra[g][i] += 1;
					}
				}

				////////////////////////////////////////
				if (flag) {
					//int mm = FindMiddlePos(data, 0, 360);
					int mm = FindMiddlePos(data, 0, B);
					char w[100];
					sprintf(w, "%3i", mm);
					ts2->Add(UnicodeString(w) + "\t" +SoundName);
				}
			}
		}
	}
}
// Формируем результат
char w[100];
UnicodeString s;
// Шапка
for (auto j = 0; j < NG; j++) {
	//s = s + "    " + Glasnaya[j] + "    \t";
	s = s + "    " + Glasnaya[j] + "\t";
}
ts->Add(s);
ts->Add("");
// Данные
for (auto i = 0; i < _N2; i++) {
	s = "";
	//for (auto j = 0; j < NG; j++) {
	for (auto j = 0; j < NG - 4; j++) {
		//sprintf(w, "%4i %4i\t", CounterPro[j][i], CounterContra[j][i]);
		sprintf(w, "%4i\t", CounterPro[j][i], CounterContra[j][i]);
		s = s + UnicodeString(w);
	}
	ts->Add(s);
}
// Сохраняем файл
ts->SaveToFile("result.txt", TEncoding::UTF8);

// Ищем медианы амплитуды
ts2->Sort();
//ts2->SaveToFile("result2.txt", TEncoding::UTF8);
s = "";
int num1 = 0, num2;
for (auto i = 0; i < ts2->Count; i++) {
	num2 = getFieldFromString(0, ts2->Strings[i], "\t").ToIntDef(0);
	if (num2 != num1) {
		s = s + " " + getFieldFromString(1, ts2->Strings[i], "\t");

	}
	else {
		s = s + getFieldFromString(1, ts2->Strings[i], "\t");
	}
    num1 = num2;
}
ts3->Add(UnicodeString(B) + " " + s);

}

ts3->SaveToFile("result3.txt", TEncoding::UTF8);



// Удаляем ненужные объекты
delete ts;
delete ts2;
delete ts3;
delete VoiceSpecters2;
return 0;
}
