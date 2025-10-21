//---------------------------------------------------------------------------

#ifndef FFTH
#define FFTH
//---------------------------------------------------------------------------
#include <vcl.h>


#define  NUMBER_IS_2_POW_K(x)   ((!((x)&((x)-1)))&&((x)>1))  // x is pow(2, k), k=1,2, ...
#define  FT_DIRECT        -1    // Direct transform.
#define  FT_INVERSE        1    // Inverse transform.

bool  FFT(double *Rdat, double *Idat, int N, int LogN, int Ft_Flag);

#define KK 1024

void Test_FFT();

const double TwoPi = 6.283185307179586;

void FFTAnalysis(double *AVal, double *FTvl, int Nvl, int Nft);

#endif
