//---------------------------------------------------------------------------

#pragma hdrstop

#include "TBallsSum.h"
#include "../../_Common_/Lib/strings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TBallsSum::TBallsSum() {
    Clear();
}

TBallsSum::~TBallsSum() {
}

// Возвращает индекс звука
int TBallsSum::GetIndex(UnicodeString s) {
    for (auto i = 0; i < N; i++) {
        if (s == Sound[i]) return i;
    }
    return -1;
}

void TBallsSum::Clear(void) {
    for (auto i = 0; i < N; i++) {
    	Balls[i] = 0.;
        Prepare[i] = 0.;
    }
}

// Добавляет баллы в соответсвующий звук
void TBallsSum::Add(UnicodeString s, double balls) {
    int i = GetIndex(s);
    if (i != -1) Balls[i] += balls;
}

void TBallsSum::SoftAdd(UnicodeString s, double balls) {
    int i = GetIndex(s);
    if (i != -1) {
        if (balls > Prepare[i]) Prepare[i] = balls;
    }
}

//
void TBallsSum::Commit(void) {
    for (auto i = 0; i < N; i++) {
        Balls[i] += Prepare[i];
        Prepare[i] = 0.;
    }

}

// Анализирует результат и сортирует по убыванию
void TBallsSum::Analyze(void) {
    // Готовим набор строк
    TStringList *ts = new TStringList;
    UnicodeString s;
    for (auto i = 0; i < N; i++) {
        s.printf(L"%10.2f", 999999. + Balls[i]);
        //s.printf(L"%9.2f", Balls[i]);
        ts->Add(s + "\t" + Sound[i]);
    }
    // Сортируем
    ts->Sort();
//ShowMessage(ts->Text);
    // Получаем набор индексов
    for (auto i = 0; i < N; i++) {
        s = getFieldFromString(1, ts->Strings[i], "\t");
        Order[N - i - 1] = GetIndex(s);
    }
    delete ts;
}


