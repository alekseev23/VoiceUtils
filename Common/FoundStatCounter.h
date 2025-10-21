//---------------------------------------------------------------------------

#ifndef FoundStatCounterH
#define FoundStatCounterH
//---------------------------------------------------------------------------
class FoundStatCounter {

public:

    int
        Voices,     // Кол-во голосов
        Specters,   // Кол-во спектров в голосе (должно быть одинаковым для всех голосов)
        Types,      // Кол-во типов спектров (обычно 5)
        Times;      // Кол-во секунд хранения информации (для каждой новой секунды своя ячейка)
    unsigned char *Data;    // Кол-во совпадений спектров

    // Конструктор и деструктор
    __fastcall FoundStatCounter(int voices, int specters, int types, int times);
    __fastcall ~FoundStatCounter();

    // Получить индекс элемента
    int __fastcall GetIndex(int voice, int specter, int type, int time);

    // Отметить найденный спектр
    void __fastcall Set(int voice, int specter, int type);

    // Подсчитать кол-во разных спектров за указанный период времени
    int __fastcall GetSpecters(int voice, int type, int times_period);

    // Очистить данные
    void __fastcall Clear(void);

    // Переход к следующей секунде
    void __fastcall Next(void);

};
#endif
