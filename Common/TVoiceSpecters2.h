//---------------------------------------------------------------------------

#ifndef TVoiceSpecters2H
#define TVoiceSpecters2H
//---------------------------------------------------------------------------
struct TVoiceSpecter {
	UnicodeString Sound;
	unsigned char Id;
	//double Data[5 * 256];
    double Data[5 * 4096];
};

//---------------------------------------------------------------------------
class TVoiceSpecters2 {

public:

	int
		N2,
		Count,
		MainCount;
	TVoiceSpecter *VoiceSpecters = NULL;

	TVoiceSpecters2();
	~TVoiceSpecters2();

	void SetSize(int count);
	bool Load(UnicodeString fname);
	bool Save(UnicodeString fname);

	double Search(double *data, UnicodeString &c, char &id, int &n);
	double Search1(double *data, UnicodeString &c, char &id, int &n, int scale);

    double GetSpecterValue(double val, int scale);

};
#endif
