//---------------------------------------------------------------------------

#ifndef commonH
#define commonH
//---------------------------------------------------------------------------

#include <VCLTee.Series.hpp>

#define _VOICE_VERSION 3

#define _FREQ 48000
#define _CHANNELS 1
// _N - кол-во точек для вычисления спектра
//#define _N 512
//#define _N2 (_N / 2)
// _NN - кол-во точек для хранения спектра (уменьшенное по сравнению с _N2)
//#define _NN 256

// _M -кол-во точек в одной точке огибающей
#define _M 480

// Кол-во точек огибающей в секунду
#define _ENV_POINTS 100

// _TIME_BUF - время для храненния аудиоданных и огибающей в секундах
#define _TIME_BUF 100

#define MAX_SPECTR_GRAPH 30

// Параметры счёта
//#define _DELTA 2
//#define _DELTA_COLUMN 5

// _R_SUM - радиус суммирования спектров
#define _R_SUM 2
// _R_SMOOTH - радиус сглаживания спектров
#define _R_SMOOTH 2

// _NR - размер буфера аудиоданных (чуть более _N, добавляем несколько элементов)
#define _NR (8192 + 2 * _R_SUM)

// _HISTORY_TIME
#define _HISTORY_TIME 10

struct SER {
	TFastLineSeries *S;
};

struct SER2 {
	TFastLineSeries *S1;
	TFastLineSeries *S2;
};

/*struct DELTA_RESULT {
public:
	float Value;
	float OutBorder;
	int OutPoints;

	DELTA_RESULT() {
		Value = 0.;
		OutBorder = 0.;
        OutPoints = 0;
	}
	~DELTA_RESULT() { }
};*/

bool LogError(UnicodeString message);
void GetRegion(int &a, int &b, int SpecterType, int ScaleView);

class Common {
    public:

    UnicodeString SOX, FFMPEG, RunDir;

    __fastcall Common();
    __fastcall ~Common();
    __fastcall void SetSOX(UnicodeString sox);
    UnicodeString __fastcall GetSOX(void);
    __fastcall void SetFFMPEG(UnicodeString ffmpeg);
    UnicodeString __fastcall GetFFMPEG(void);
};
#endif

