//---------------------------------------------------------------------------

#pragma hdrstop

#include "TColorSceme.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TColorSceme::TColorSceme(TColor left_color, TColor right_color, int control_points, TColor *control_color, double *values, int colors) {
	// Граничные цвета
	LeftColor = left_color;
	RightColor = right_color;
	LeftRed = ((int)LeftColor) & 255;
	LeftGreen = ((int)LeftColor >> 8) & 255;
	LeftBlue = ((int)LeftColor >> 16) & 255;
	RightRed = ((int)RightColor) & 255;
	RightGreen = ((int)RightColor >> 8) & 255;
	RightBlue = ((int)RightColor >> 16) & 255;
	// Контрольные цвета
	ControlPoints = control_points;
	ControlValues = new double [ControlPoints];
	ControlRed = new unsigned char [ControlPoints];
	ControlGreen = new unsigned char [ControlPoints];
	ControlBlue = new unsigned char [ControlPoints];
	for (auto i = 0; i < ControlPoints; i++) {
		ControlValues[i] = values[i];
		ControlRed[i] = ((int)control_color[i]) & 255;
		ControlGreen[i] = ((int)control_color[i] >> 8) & 255;
		ControlBlue[i] = ((int)control_color[i] >> 16) & 255;
	}
	//
	Min = values[0];
	Max = values[ControlPoints - 1];
	Colors = colors;
	Red = new unsigned char [Colors];
	Green = new unsigned char [Colors];
	Blue = new unsigned char [Colors];
	Color = new TColor [Colors];
	// Расчёт
	double val;
	unsigned char r, g, b;
	for (auto i = 0; i < Colors; i++) {
		val = Min + (Max - Min) * i / (Colors - 1);
		Calculate(val, r, g, b);
		Red[i] = r;
		Green[i] = g;
		Blue[i] = b;
		Color[i] = (TColor)(r + (g << 8) + (b << 16));
	}
}

TColorSceme::~TColorSceme() {
	delete[] Red;
	delete[] Green;
	delete[] Blue;
	delete[] Color;
	delete[] ControlValues;
	delete[] ControlRed;
	delete[] ControlGreen;
	delete[] ControlBlue;
}

TColor TColorSceme::Calculate(double val) {
	unsigned char r, g, b;
	Calculate(val, r, g, b);
	return (TColor)(r + (g << 8) + (b << 16));
}

void TColorSceme::Calculate(double val, unsigned char &r, unsigned char &g, unsigned char &b) {
	if (val < Min) {
		r = LeftRed;
		g = LeftGreen;
		b = LeftBlue;
	}
	if (val > Max) {
		r = RightRed;
		g = RightGreen;
		b = RightBlue;
	}
	for (auto i = 0; i < ControlPoints - 1; i++) {
		if ((val >= ControlValues[i]) && (val <= ControlValues[i + 1])) {
			double k = (val - ControlValues[i]) / (ControlValues[i + 1] - ControlValues[i]);
			r = (unsigned char)(k * (ControlRed[i + 1] - ControlRed[i]) + ControlRed[i]);
			g = (unsigned char)(k * (ControlGreen[i + 1] - ControlGreen[i]) + ControlGreen[i]);
			b = (unsigned char)(k * (ControlBlue[i + 1] - ControlBlue[i]) + ControlBlue[i]);
			return;
		}
	}
	return;
}


TColor TColorSceme::Get(double val) {
	if (val < Min) return LeftColor;
	if (val > Max) return RightColor;
	int i = (int)(Colors * (val - Min) / (Max - Min));
	if (i < 0) i = 0;
	if (i >= Colors) i = Colors - 1;
	return Color[i];
}

void TColorSceme::Get(double val, unsigned char &r, unsigned char &g, unsigned char &b) {
	if (val < Min) {
		r = LeftRed;
		g = LeftGreen;
		b = LeftBlue;
//ShowMessage("val=" + UnicodeString(val) + " Min=" + UnicodeString(Min) + " r="  + UnicodeString(r) + " g=" + UnicodeString(g) + " b=" + UnicodeString(b));
		return;
	}
	if (val > Max) {
		r = RightRed;
		g = RightGreen;
		b = RightBlue;
//ShowMessage("val=" + UnicodeString(val) + " Max=" + UnicodeString(Max) + " r="  + UnicodeString(r) + " g=" + UnicodeString(g) + " b=" + UnicodeString(b));
		return;
	}
	int i = (int)(Colors * (val - Min) / (Max - Min));
	if (i < 0) i = 0;
	if (i >= Colors) i = Colors - 1;
	r = Red[i];
	g = Green[i];
	b = Blue[i];
}

