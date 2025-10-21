//---------------------------------------------------------------------------

#pragma hdrstop

#include "TSpecterAnalyzer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


__fastcall TSpecterAnalyzer::TSpecterAnalyzer() {

}

__fastcall TSpecterAnalyzer::~TSpecterAnalyzer() {

}

void __fastcall TSpecterAnalyzer::Analyze(int n, float *data) {
    N = n;
    double current_sum = 0.;
    int k, percent, old_percent = 0;
    // Находим интегральную сумму и максимальное значение
    Sum = Max = 0.;
    MaxPos = 0;
    for (int i = 0; i < N; i++) {
    	Sum += data[i];
        if (data[i] > Max) {
        	Max = data[i];
            MaxPos = i;
        }
    }
    // Второй проход
    for (int i = 0; i < N; i++) {
        // Заново суммируем
        current_sum += data[i];
        // Находим текущий процент
        percent = 100. * current_sum / Sum;
        for (int j = old_percent + 1; j <= percent; j++) x[j] = i;
        old_percent = percent;
    }
}

void __fastcall TSpecterAnalyzer::Analyze(int n, double *data) {
    N = n;
    double current_sum = 0.;
    int k, percent, old_percent = 0;
    // Находим интегральную сумму и максимальное значение
    Sum = Max = 0.;
    MaxPos = 0;
    for (int i = 0; i < N; i++) {
    	Sum += data[i];
        if (data[i] > Max) {
        	Max = data[i];
            MaxPos = i;
        }
    }
    // Второй проход
    for (int i = 0; i < N; i++) {
        // Заново суммируем
        current_sum += data[i];
        // Находим текущий процент
        percent = 100. * current_sum / Sum;
        for (int j = old_percent + 1; j <= percent; j++) x[j] = i;
        old_percent = percent;
    }
}


