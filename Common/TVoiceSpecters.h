//---------------------------------------------------------------------------

#ifndef TVoiceSpectersH
#define TVoiceSpectersH
//---------------------------------------------------------------------------
struct TVoiceSpecter {
	wchar_t Char;
	char Id;
	double Data[5 * 256];
};

//---------------------------------------------------------------------------
class TVoiceSpecters {

public:

	int
		N2,
		Count,
		MainCount;
	TVoiceSpecter *VoiceSpecters = NULL;

	TVoiceSpecters();
	~TVoiceSpecters();

	void SetSize(int count);
	bool Load(UnicodeString fname);
	bool Save(UnicodeString fname);

	double Search(double *data, wchar_t &c, char &id, int &n);
	double Search1(double *data, wchar_t &c, char &id, int &n, int scale);

    double GetSpecterValue(double val, int scale);

};
#endif
