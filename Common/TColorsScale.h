//---------------------------------------------------------------------------

#ifndef TColorsScaleH
#define TColorsScaleH
//---------------------------------------------------------------------------

#include <vector>

struct TColorPoint {
    unsigned char R, G, B;
    unsigned int Color;
    double Val;

    __fastcall TColorPoint();
    __fastcall TColorPoint(double val, unsigned char r, unsigned char g, unsigned char b);
};

//---------------------------------------------------------------------------

class TDoubleValuesMapper {

public:

    int Count;
    std::vector <double> V1;
    std::vector <double> V2;

    __fastcall TDoubleValuesMapper();
    __fastcall ~TDoubleValuesMapper();
    void __fastcall Add(double v1, double v2);
    void __fastcall Clear(void);
    double __fastcall Get(double v);

};
//---------------------------------------------------------------------------

class TColorsScale {

public:

    std::vector <TColorPoint> Point;
    class TDoubleValuesMapper *Bright;
    unsigned int
        ColorItems,     // Кол-во цветов
    	LowColor,    	// Цвет по умолчанию для более низких значений
        HighColor;   	// Цвет по умолчанию для более высоких значений

    __fastcall TColorsScale();
    __fastcall ~TColorsScale();

    // Добавить точку цвета
    void __fastcall AddPoint(double val, unsigned char r, unsigned char g, unsigned char b);

    // Добавить яркость для значения
    void __fastcall AddBright(double val, double new_val);

    // Очистить список цветов и яркостей
    void __fastcall Clear(void);
    // Очистить список цветов
    void __fastcall ClearColors(void);
    // Очистить список яркостей
    void __fastcall ClearBright(void);

    // Получить цвет
    void __fastcall GetColor(double val, unsigned char &r, unsigned char &g, unsigned char &b);
    void __fastcall GetColor(double val, double bright, unsigned char &r, unsigned char &g, unsigned char &b);
    unsigned int __fastcall GetColor(double val);
    unsigned int __fastcall GetColor(double val, double bright);

private:

    // Вычислить промежуточный цвет
    unsigned int __fastcall Calculate(double x, double dx, int k);

};

//---------------------------------------------------------------------------

#endif
