//---------------------------------------------------------------------------

#ifndef TAudioMaxH
#define TAudioMaxH
//---------------------------------------------------------------------------

#define TAudioMax_N 100

struct TAudioMaxData {
	unsigned char
		dx_p[TAudioMax_N],
		dx_m[TAudioMax_N],
		dy_p[TAudioMax_N],
		dy_m[TAudioMax_N];

	TAudioMaxData();
	~TAudioMaxData();
};

//---------------------------------------------------------------------------

class TAudioMax {

public:

	struct TAudioMaxData *Data = NULL;
	int
		Count,
		Area = 480;
		float
			DX_P[TAudioMax_N],
			DY_P[TAudioMax_N],
			DX_M[TAudioMax_N],
			DY_M[TAudioMax_N];

	TAudioMax();
	~TAudioMax();

	void Set(short *data, int n);
	int Compare();
	bool Save(UnicodeString fname);
	bool Load(UnicodeString fname);
	void Draw(TBitmap *bmp);
	void DrawMiddleValues(TBitmap *bmp);

	UnicodeString Print(void);
    UnicodeString PrintMiddleValues(void);

};
#endif
