//---------------------------------------------------------------------------

#pragma hdrstop

#include "TSpecters.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace std;

__fastcall TSpecterData::TSpecterData() {
    N = 256;
}

__fastcall TSpecterData::TSpecterData(int n) {
    ReSize(n);
}

__fastcall TSpecterData::~TSpecterData() {
    if (Data != NULL) {
    	delete[] Data;
    	delete[] SourceData;
    	delete[] Power;
    }
}

// Изменить размерность массива Data[]
void __fastcall TSpecterData::ReSize(int n) {
    N = n;
    if (Data != NULL) {
    	delete[] Data;
    	delete[] SourceData;
    	delete[] Power;
    }
    Data = new float [N];
    SourceData = new float [N];
    Power = new float [N];
}

// Передать массив с данными и параметрами
void __fastcall TSpecterData::Set(int n, unsigned short env_abs, unsigned short env_max, float *data) {
    ReSize(n);
    EnvelopeAbs = env_abs;
    EnvelopeMax = env_max;
    for (int i = 0; i < N; i++) {
    	SourceData[i] = data[i];
    	Data[i] = data[i];
    }
    ReCount();
}

// Передать массив с данными и параметрами
void __fastcall TSpecterData::Set(int n, unsigned short env_abs, unsigned short env_max, double *data) {
    ReSize(n);
    EnvelopeAbs = env_abs;
    EnvelopeMax = env_max;
    for (int i = 0; i < N; i++) {
    	SourceData[i] = data[i];
    	Data[i] = data[i];
    }
    ReCount();
}

// Пересчитать значения Max и Sum
void __fastcall TSpecterData::ReCount(void) {
    if (Data == NULL) return;
    Sum = Max = 0.;
    SourceSum = SourceMax = 0.;
    PowerSum = PowerMax = 0.;
    for (int i = 0; i < N; i++) {
        Sum += Data[i];
        if (Data[i] > Max) Max = Data[i];
        SourceSum += SourceData[i];
        if (SourceData[i] > SourceMax) SourceMax = SourceData[i];
        PowerSum += Power[i];
        if (Power[i] > PowerMax) PowerMax = Power[i];
    }
}

// Считать значения из файла
bool __fastcall TSpecterData::Read(FILE *fd) {
    if (fread(&EnvelopeAbs, 2, 1, fd) != 1) return false;
    if (fread(&EnvelopeMax, 2, 1, fd) != 1) return false;
    if (fread(&SourceMax, 4, 1, fd) != 1) return false;
    if (fread(&SourceSum, 4, 1, fd) != 1) return false;
    if (fread(SourceData, 4, N, fd) != N) return false;
    for (int i = 0; i < N; i++) Data[i] = SourceData[i];
    CalculatePower();
    ReCount();
    return true;
}

// Сохранить значения в файл
bool __fastcall TSpecterData::Write(FILE *fd) {
    if (fwrite(&EnvelopeAbs, 2, 1, fd) != 1) return false;
    if (fwrite(&EnvelopeMax, 2, 1, fd) != 1) return false;
    if (fwrite(&SourceMax, 4, 1, fd) != 1) return false;
    if (fwrite(&SourceSum, 4, 1, fd) != 1) return false;
    if (fwrite(SourceData, 4, N, fd) != N) return false;
    return true;
}

void __fastcall TSpecterData::NormalizeSpecter(double norm) {
    for (int i = 0; i < N; i++) Data[i] = norm * SourceData[i];
}

void __fastcall TSpecterData::CalculatePower(void) {
    int i, k;
    double a, f, p;
    PowerSum = PowerMax = 0.;
    switch (N) {
        case   256: k = 84; break;
        case   512: k = 170; break;
        case  1024: k = 340; break;
        case  2048: k = 682; break;
        case  4096: k = 1364; break;
        case  8192: k = 2730; break;
        case 16384: k = 5460; break;
        case 32768: k = 10920; break;
        case 65536: k = 21840; break;
        default: return;
    }
    for (i = 0; i < N; i++) {
        f = 1. * i / k;
        a = Data[i];
        Power[i] = a * a * f * f;
        PowerSum += Power[i];
        if (Power[i] > PowerMax) PowerMax = Power[i];
    }
}

// Нормировка значений массива
void __fastcall TSpecterData::Normalize(float *array, int n, float val) {
    float max = array[0];
    for (int i = 1; i < n; i++) {
        if (array[i] > max) max = array[i];
    }
    // Если значения маленькие
    if (max == 0.) return;
    float k = val / max;
    for (int i = 0; i < n; i++) {
        array[i] *= k;
    }
}

//---------------------------------------------------------------------------

__fastcall TSpecters::TSpecters() {
    Init(256);
    Items = 0;
}

__fastcall TSpecters::TSpecters(int n) {
    Init(n);
    Items = 0;
}

__fastcall TSpecters::TSpecters(int n, int items) {
    Items = 0;
    Init(n);
    ReSize(items);
}

__fastcall TSpecters::~TSpecters() {
    Clear();
}

// Установить кол-во элементов в спектрах
void __fastcall TSpecters::Init(int n) {
    Clear();
    N = n;
}

// Задать кол-во спектров
void __fastcall TSpecters::ReSize(int items) {
    Clear();
    Items = items;
    for (int i = 0; i < Items; i++) {
        class TSpecterData *t = new TSpecterData(N);
        SpecterData.push_back(t);
    }
}

// Сохранить файл со спектрами
bool __fastcall TSpecters::Save(UnicodeString fname) {
    FILE *fd;
    fd = _wfopen(fname.w_str(), L"wb");
    if (fwrite(&Version, 2, 1, fd) != 1) return false;
    if (fwrite(&N, 2, 1, fd) != 1) return false;
    if (fwrite(&Items, 4, 1, fd) != 1) return false;
    for (int i = 0; i < Items; i++) SpecterData[i]->Write(fd);
    fclose(fd);
    return true;
}

// Загрузить файл со спектрами
bool __fastcall TSpecters::Load(UnicodeString fname) {
    FILE *fd;
    unsigned short ver;
    fd = _wfopen(fname.w_str(), L"rb");
    if (fread(&ver, 2, 1, fd) != 1) return false;
    if (Version != ver) return false;
    if (fread(&N, 2, 1, fd) != 1) return false;
    if (fread(&Items, 4, 1, fd) != 1) return false;
    for (int i = 0; i < Items; i++) {
        class TSpecterData *t = new TSpecterData(N);
    	t->Read(fd);
        SpecterData.push_back(t);
    }
    fclose(fd);
    return true;
}

// Очистить данные
void __fastcall TSpecters::Clear() {
    for (int i = 0; i < Items; i++) delete SpecterData[i];
    SpecterData.clear();
}

// Добавить данные
bool __fastcall TSpecters::Add(float *data, unsigned short env_abs, unsigned short env_max) {
	class TSpecterData *t = new TSpecterData(N);
    t->Set(N, env_abs, env_max, data);
    SpecterData.push_back(t);
}

// Добавить данные
bool __fastcall TSpecters::Add(double *data, unsigned short env_abs, unsigned short env_max) {
	class TSpecterData *t = new TSpecterData(N);
    t->Set(N, env_abs, env_max, data);
    SpecterData.push_back(t);
}

// Изменить данные
bool __fastcall TSpecters::Set(int i, float *data, unsigned short env_abs, unsigned short env_max) {
    if (i >= Items) return false;
    SpecterData[i]->Set(N, env_abs, env_max, data);
}

// Изменить данные
bool __fastcall TSpecters::Set(int i, double *data, unsigned short env_abs, unsigned short env_max) {
    if (i >= Items) return false;
    SpecterData[i]->Set(N, env_abs, env_max, data);
}

void __fastcall TSpecters::NormalizeToMax(void) {
    for (int i = 0; i < Items; i++) {
        if (SpecterData[i]->EnvelopeMax != 0.) {
	        SpecterData[i]->NormalizeSpecter(32767./ SpecterData[i]->EnvelopeMax);
        }
    }
}

void __fastcall TSpecters::NormalizeToAbs(void) {
    for (int i = 0; i < Items; i++) {
        SpecterData[i]->NormalizeSpecter(32767./ SpecterData[i]->EnvelopeAbs);
    }
}

void __fastcall TSpecters::CalculatePower(void) {
    for (int i = 0; i < Items; i++) {
        SpecterData[i]->CalculatePower();
    }

}

// Анализ спектров
void __fastcall TSpecters::Analyze(void) {
    // Находим глобальный максимум
    double Max = 0.;
    for (int i = 0; i < Items; i++) {
        for (int j = 0; j < N; j++) {
            if (SpecterData[i]->Power[j] > Max) Max = SpecterData[i]->Power[j];
        }
    }
    // Создаём массив счётчиков и обнуляем его
    int M = 1000000, Z, Z1, Z2, Z3, sum = 0, mode = 0;
    unsigned int *Counter = new unsigned int [M];
    // Цикл по всем точкам и определение приведённого значения
    for (int i = 0; i < Items; i++) {
        for (int j = 0; j < N; j++) {
            Counter[(int)(SpecterData[i]->Power[j] * M / Max)] += 1;
        }
    }
    // Находим распределение и узловые точки
    Z = N * Items;
    Z1 = Z * 80 / 100;
    Z2 = Z * 90 / 100;
    Z3 = Z * 99 / 100;
    for (int i = 0; i < M; i++) {
        sum += Counter[i];
        switch (mode) {
            case 0: //
                    if (sum > Z1) {
                    cout << "i=" << i << "\tsum=" << sum << "\tZ1=" << Z1 <<endl;
                        Level[0] = Max * i / M;
                        mode = 1;
                    }
                    break;
            case 1: //
                    if (sum > Z2) {
                    cout << "i=" << i << "\tsum=" << sum << "\tZ2=" << Z2 <<endl;
                        Level[1] = Max * i / M;
                        mode = 2;
                    }
                    break;
            case 2: //
                    if (sum > Z3) {
                    cout << "i=" << i << "\tsum=" << sum << "\tZ3=" << Z3 <<endl;
                        Level[2] = Max * i / M;
                        mode = 3;
                    }
                    break;
            case 3: //
                    break;
        }
    }
    // Последняя точка
    Level[3] = Max;
    // Удаляем рабочие массивы
    delete[] Counter;

    // Ищем кол-во экстремумов
    for (int i = 1; i < Items - 1; i++) {
        SpecterData[i]->Extremum = 0;
        for (int j = 0; j < N; j++) {
            // Точка максимума
            if ((SpecterData[i]->Power[j] > SpecterData[i - 1]->Power[j])
                && (SpecterData[i]->Power[j] > SpecterData[i + 1]->Power[j])) SpecterData[i]->Extremum++;
            // Точка минимума
            else if ((SpecterData[i]->Power[j] < SpecterData[i - 1]->Power[j])
                && (SpecterData[i]->Power[j] < SpecterData[i + 1]->Power[j])) SpecterData[i]->Extremum++;
        }
    }
    SpecterData[0]->Extremum = 0;
    SpecterData[Items - 1]->Extremum = 0;
}

// Получить уровень
int TSpecters::GetLevel(float val) {
    for (int i = 0; i < 4; i++) {
        if (val <= Level[i]) return i;
    }
    return 4;
}

// Поиск максиммумов
void TSpecters::FindMax(void) {
    Max = SourceMax = PowerMax = PowerSumMax = 0.;
    for (int i = 0; i < Items; i++) {
        if (SpecterData[i]->Max > Max) Max = SpecterData[i]->Max;
        if (SpecterData[i]->SourceMax > SourceMax) SourceMax = SpecterData[i]->SourceMax;
        if (SpecterData[i]->PowerMax > PowerMax) PowerMax = SpecterData[i]->PowerMax;
        if (SpecterData[i]->PowerSum > PowerSumMax) PowerSumMax = SpecterData[i]->PowerSum;
    }
}

