//---------------------------------------------------------------------------

#ifndef TWavFileH
#define TWavFileH
//---------------------------------------------------------------------------
class TWavFile {

public:

	int
		N = 512,
		N2 = 256,
		BufSize,
		SpectersCount;
	short *Buf = NULL;
	double
		*Re = NULL,
		*Im = NULL;
	bool Loaded = false;
    UnicodeString FileName;

	TWavFile();
	~TWavFile();

	bool Load(UnicodeString filename);
	bool Save(UnicodeString filename);

	// ¬ычисл€ем спектры из аудио данных
	void Calculate();
	// ѕересчитываем аудио данные из спектров
	void ReCalculate();

	void Cut(int left, int right, int d_left, int d_right, double kf, double kb);
	void Shift(int val);
	void Move();
	void Copy();
	void Resize();

};
#endif
