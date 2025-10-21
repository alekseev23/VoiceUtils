//---------------------------------------------------------------------------

#pragma hdrstop

#include "TColorsScale.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


TColorPoint::TColorPoint() {
    Val = 0.;
    R = G = B = 0;
    Color = R + (G << 8) + (B << 16);
}

TColorPoint::TColorPoint(double val, unsigned char r, unsigned char g, unsigned char b) {
    Val = val;
    R = r;
    G = g;
    B = b;
    Color = R + (G << 8) + (B << 16);
}

//---------------------------------------------------------------------------

__fastcall TDoubleValuesMapper::TDoubleValuesMapper() {
    Count = 0;
}

__fastcall TDoubleValuesMapper::~TDoubleValuesMapper() {
    V1.clear();
    V2.clear();
}

void __fastcall TDoubleValuesMapper::Add(double v1, double v2) {
    V1.push_back(v1);
    V2.push_back(v2);
    Count++;
}

void __fastcall TDoubleValuesMapper::Clear(void) {
    V1.clear();
    V2.clear();
    Count = 0;
}

double __fastcall TDoubleValuesMapper::Get(double v) {
    if (Count == 0) return v;
    if (Count == 1) return V2[0];
    if (v < V1[0]) return V2[0];
    for (int i = 0; i < Count - 1; i++) {
        if ((v >= V1[i]) && (v <= V1[i + 1])) {
            double k = (v - V1[i]) / (V1[i + 1] - V1[i]);
            return V2[i] + k * (V2[i + 1] - V2[i]);
        }
    }
    return V2[Count - 1];
}

//---------------------------------------------------------------------------

__fastcall TColorsScale::TColorsScale() {
    ColorItems = 0;
    LowColor = 0;
    HighColor = 0xffffff;
    Bright = new class TDoubleValuesMapper;
}

__fastcall TColorsScale::~TColorsScale() {
    Clear();
    delete Bright;
}

void __fastcall TColorsScale::AddPoint(double val, unsigned char r, unsigned char g, unsigned char b) {
    Point.push_back(TColorPoint(val, r, g, b));
    ColorItems++;
}

// Добавить яркость для значения
void __fastcall TColorsScale::AddBright(double val, double new_val) {
    Bright->Add(val, new_val);
}

void __fastcall TColorsScale::Clear(void) {
	ClearColors();
    ClearBright();
}

void __fastcall TColorsScale::ClearColors(void) {
	Point.clear();
    ColorItems = 0;
}

void __fastcall TColorsScale::ClearBright(void) {
	Bright->Clear();
}

void __fastcall TColorsScale::GetColor(double val, unsigned char &r, unsigned char &g, unsigned char &b) {
    unsigned int color = GetColor(val);
    r = color & 255;
    g = (color >> 8) & 255;
    b = (color >> 16) & 255;
}

void __fastcall TColorsScale::GetColor(double val, double bright, unsigned char &r, unsigned char &g, unsigned char &b) {
    unsigned int color = GetColor(val, bright);
    r = color & 255;
    g = (color >> 8) & 255;
    b = (color >> 16) & 255;
}

unsigned int __fastcall TColorsScale::GetColor(double val) {
    // Нет добавленных точек
    if (ColorItems == 0) return LowColor;
    // Значение маленькое
    if (val < Point[0].Val) return LowColor;
    // Только одна добавленная точка
    if (ColorItems == 1) {
        if (val == Point[0].Val) return Point[0].Color;
    }
    else { // Несколько точек
	    for (int i = 0; i < ColorItems - 1; i++) {
    	    if ((val >= Point[i].Val) && (val <= Point[i + 1].Val)) {
                return Calculate(val - Point[i].Val, Point[i + 1].Val - Point[i].Val, i);
            }
        }
    }
	return HighColor;
}

unsigned int __fastcall TColorsScale::GetColor(double val, double bright) {
    if (Bright->Count > 0) bright = Bright->Get(bright);
    unsigned int color = GetColor(val);
    unsigned char r, g, b;
    r = (unsigned char)(bright * (color & 255));
    g = (unsigned char)(bright * ((color >> 8) & 255));
    b = (unsigned char)(bright * ((color >> 16) & 255));
    return r + (g << 8) + (b << 16);
}

unsigned int __fastcall TColorsScale::Calculate(double x, double dx, int i) {
    if (x == 0) return Point[i].Color;
    else if (x >= dx) return Point[i + 1].Color;
    double k = x / dx;
    unsigned char r, g, b;
    r = (unsigned char)(1. * Point[i].R + k * (Point[i + 1].R - Point[i].R));
    g = (unsigned char)(1. * Point[i].G + k * (Point[i + 1].G - Point[i].G));
    b = (unsigned char)(1. * Point[i].B + k * (Point[i + 1].B - Point[i].B));
    return r + (g << 8) + (b << 16);
}


