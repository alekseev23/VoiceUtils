#include <vcl.h>
#include <tchar.h>
#include "../Common/SpecterWork.h"
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/strings.h"
#include "../../_Common_/Lib/RunProcess.h"

struct FreqInfo {
    int Count = 0;
    double Sum = 0., Max = 0.;

    double Middle(void) {
        if (Count == 0) return 0.;
        return Sum / Count;
    }

    void Add(double val) {
        if (val > Max) Max = val;
        Count++;
        Sum += val;
    }
};

int _tmain(int argc, _TCHAR* argv[])
{
// Анализ аргументов командной строки
if (argc != 2) {
	std::cout << "Usage: FindPeaks.exe DatFile" << std::endl
		<<  "Example: FindPeaks.exe D:\\Data\\1.dat" << std::endl;
		return 1;
}
// Чтение данных
class SpecterWork *sw = new class SpecterWork;
//sw->ReadDataFromDAT(UnicodeString(argv[1]));
sw->ReadDataFromWAV(UnicodeString(argv[1]));
// Вычисление спектров
int N = 8192, N2, p, d = 2, K = 320;
FreqInfo *FInfo = new FreqInfo [K];
N2 = N / 2;
sw->CountSpectersThread(N, false, sw->BufSize / 5);
// Цикл по спектрам
for (auto i = d; i < sw->Count - d; i++) {
	// Нахождение максимумов
	p = i * N2 + 1;
//    for (auto j = 1; j < N2 - 1; j++) {
    for (auto j = d; j < K - d; j++) {
        if (
        	(sw->Data[p] > sw->Data[p - 1])
            && (sw->Data[p] > sw->Data[p + 1])
            && (sw->Data[p] > sw->Data[p - N2])
            && (sw->Data[p] > sw->Data[p + N2])

        	&& (sw->Data[p] > sw->Data[p - 2])
            && (sw->Data[p] > sw->Data[p + 2])
            && (sw->Data[p] > sw->Data[p - N])
            && (sw->Data[p] > sw->Data[p + N])

            ) {

            if (sw->Data[p] > sw->MaxValue / 10) {
				// Вывод результатов в файл
    	        //printf("%5i\t%4i\t%6.2f\n", i, j, sw->Data[p]);
                FInfo[j].Add(sw->Data[p]);
            }
        }
  	    p++;
    }
}
std::cout << sw->MaxValue << std::endl;

for (auto i = 0; i < K; i++) {
    if (FInfo[i].Count > 0) {
        printf("Freq=%4i\tMax=%4.0f\tCount=%3i\tMiddle=%4.0f\n", i, FInfo[i].Max, FInfo[i].Count, FInfo[i].Middle());
    }
}
// Удаляем объекты
delete sw;
delete[] FInfo;
// Возвращаем код завершения
return 0;
}
