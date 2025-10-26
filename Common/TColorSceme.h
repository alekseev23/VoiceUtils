//---------------------------------------------------------------------------

#ifndef TColorScemeH
#define TColorScemeH
//---------------------------------------------------------------------------
class TColorSceme {

public:

	int
		ControlPoints,  // Ключевые точки
		Colors;         // Количество результирующих цветов
	TColor
		*Color,         // Результирующие цвета
		LeftColor,      // Цвет за нижней границей
		RightColor;     // Цвет за верхней границей
	double
		*ControlValues, // Значения в ключевых точках
		Min,            // Минимальное значение
		Max;            // Максимальное значение
	unsigned char
		*Red,           // Красная компонента рассчитаных цветов Red[Colors]
		*Green,
		*Blue,
		*ControlRed,
		*ControlGreen,
		*ControlBlue,
		LeftRed,
		LeftGreen,
		LeftBlue,
		RightRed,
		RightGreen,
		RightBlue;

	TColorSceme(TColor left_color, TColor right_color, int control_points, TColor *control_color, double *values, int colors);
	~TColorSceme();

	TColor Calculate(double val);
	void Calculate(double val, unsigned char &r, unsigned char &g, unsigned char &b);

	TColor Get(double val);
	void Get(double val, unsigned char &r, unsigned char &g, unsigned char &b);

};

#endif
