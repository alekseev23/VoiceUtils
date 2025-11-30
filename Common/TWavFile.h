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
	TWavFile(int n);
	~TWavFile();

    // Загрузка из WAV файла
	bool Load(UnicodeString filename);
    // Загрузка из DAT файла
	bool LoadFromDAT(UnicodeString filename);
    // Загрузка из DAT файла с определённой позиции (pos) определённого куска данных (len)
	bool LoadFromDAT(UnicodeString filename, int begin, int len);


    // Сохранение в WAV файл
	bool Save(UnicodeString filename);
    // Сохранение в DAT файл
	bool SaveToDat(UnicodeString filename);

	// Вычисляем спектры из аудио данных
	void Calculate();
	// Пересчитываем аудио данные из спектров
	void ReCalculate();

	void Cut(int left, int right, int d_left, int d_right, double kf, double kb);
	void Shift(int val);
	void Move();
	void Copy(int left, int right, double amp, int sm);
	void Resize(double ratio);
    int GetLogN(void);

};
#endif
