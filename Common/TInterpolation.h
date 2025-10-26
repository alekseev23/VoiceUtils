//---------------------------------------------------------------------------

#ifndef TInterpolationH
#define TInterpolationH
//---------------------------------------------------------------------------
class TInterpolation {

public:

	TInterpolation();
	~TInterpolation();

	void InterpolateX2(int n, short *val, short *new_val);
	void InterpolateX4(int n, short *val, short *new_val);

private:
	double Lagrange(double x, int n, double x_arr[], double y_arr[]);
    double Newton(double x, int n, double x_arr[], double y_arr[]);

};
#endif
