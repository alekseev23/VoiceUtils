#include <FFT.h>
//_________________________________________________________________________________________
//_________________________________________________________________________________________
//
// NAME:          FFT.
// PURPOSE:       Быстрое преобразование Фурье: Комплексный сигнал в комплексный спектр и обратно.
//                В случае действительного сигнала в мнимую часть (Idat) записываются нули.
//                Количество отсчетов - кратно 2**К - т.е. 2, 4, 8, 16, ... (см. комментарии ниже).
//
//
// PARAMETERS:
//
//    double *Rdat    [in, out] - Real part of Input and Output Data (Signal or Spectrum)
//    double *Idat    [in, out] - Imaginary part of Input and Output Data (Signal or Spectrum)
//    int    N       [in]      - Input and Output Data length (Number of samples in arrays)
//    int    LogN    [in]      - Logarithm2(N)
//    int    Ft_Flag [in]      - Ft_Flag = FT_ERR_DIRECT  (i.e. -1) - Direct  FFT  (Signal to Spectrum)
//		                 Ft_Flag = FT_ERR_INVERSE (i.e.  1) - Inverse FFT  (Spectrum to Signal)
//
// RETURN VALUE:  false on parameter error, true on success.
//_________________________________________________________________________________________
//
// NOTE: In this algorithm N and LogN can be only:
//       N    = 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384;
//       LogN = 2, 3,  4,  5,  6,   7,   8,   9,   10,   11,   12,   13,    14;
//_________________________________________________________________________________________
//_________________________________________________________________________________________


bool  FFT(double *Rdat, double *Idat, int N, int LogN, int Ft_Flag)
{
  // parameters error check:
  if((Rdat == NULL) || (Idat == NULL))                  return false;
  if((N > 16384) || (N < 1))                            return false;
  if(!NUMBER_IS_2_POW_K(N))                             return false;
  if((LogN < 2) || (LogN > 14))                         return false;
  if((Ft_Flag != FT_DIRECT) && (Ft_Flag != FT_INVERSE)) return false;

  //register int  i, j, n, k, io, ie, in, nn;
  int  i, j, n, k, io, ie, in, nn;
  double         ru, iu, rtp, itp, rtq, itq, rw, iw, sr;

  static const double Rcoef[14] =
  {  -1.0000000000000000F,  0.0000000000000000F,  0.7071067811865475F,
      0.9238795325112867F,  0.9807852804032304F,  0.9951847266721969F,
      0.9987954562051724F,  0.9996988186962042F,  0.9999247018391445F,
      0.9999811752826011F,  0.9999952938095761F,  0.9999988234517018F,
      0.9999997058628822F,  0.9999999264657178F
  };
  static const double Icoef[14] =
  {   0.0000000000000000F, -1.0000000000000000F, -0.7071067811865474F,
     -0.3826834323650897F, -0.1950903220161282F, -0.0980171403295606F,
     -0.0490676743274180F, -0.0245412285229122F, -0.0122715382857199F,
     -0.0061358846491544F, -0.0030679567629659F, -0.0015339801862847F,
     -0.0007669903187427F, -0.0003834951875714F
  };

  nn = N >> 1;
  ie = N;
  try {
  for(n=1; n<=LogN; n++)
  {
	rw = Rcoef[LogN - n];
	iw = Icoef[LogN - n];
	if(Ft_Flag == FT_INVERSE) iw = -iw;
	in = ie >> 1;
	ru = 1.0F;
	iu = 0.0F;
	for(j=0; j<in; j++)
	{
	  for(i=j; i<N; i+=ie)
	  {
		io       = i + in;
		rtp      = Rdat[i]  + Rdat[io];
		itp      = Idat[i]  + Idat[io];
		rtq      = Rdat[i]  - Rdat[io];
		itq      = Idat[i]  - Idat[io];
		Rdat[io] = rtq * ru - itq * iu;
		Idat[io] = itq * ru + rtq * iu;
		Rdat[i]  = rtp;
		Idat[i]  = itp;
	  }

	  sr = ru;
	  ru = ru * rw - iu * iw;
	  iu = iu * rw + sr * iw;
	}

	ie >>= 1;
  }

  } catch (...) {
	ShowMessage("Error 1");
  }


  try {

  for(j=i=1; i<N; i++)
  {
	if(i < j)
	{
	  io       = i - 1;
	  in       = j - 1;
	  rtp      = Rdat[in];
	  itp      = Idat[in];
	  Rdat[in] = Rdat[io];
	  Idat[in] = Idat[io];
	  Rdat[io] = rtp;
	  Idat[io] = itp;
	}

	k = nn;

	while(k < j)
	{
	  j   = j - k;
	  k >>= 1;
	}

	j = j + k;
  }

  } catch (...) {
	ShowMessage("Error 2");
  }


  if(Ft_Flag == FT_DIRECT) return true;

  rw = 1.0F / N;

try {
  for(i=0; i<N; i++)
  {
	Rdat[i] *= rw;
	Idat[i] *= rw;
  }

} catch (...) {
	ShowMessage("Error 3");
}

  return true;
}
//---------------------------------------------------------------------------
// Пример вычисления БПФ от одного периода косинусного
// действительного сигнала

void Test_FFT()
{
  static double Re[KK];
  static double Im[KK];
  double  p = 2. * 3.141592653589 / KK; // будет 8 отсчетов на период

  int i;
  // формируем сигнал
  for(i=0; i<KK; i++)
  {
	Re[i] = cos(3.9* p * i);  // заполняем действительную часть сигнала
	Im[i] = 0.0;         // заполняем мнимую часть сигнала
  }

  FFT(Re, Im, KK, 10, FT_DIRECT); // вычисляем прямое БПФ

  // выводим действительную и мнимую части спектра и спектр мощности
  FILE *f = fopen("e:\\spectrum.txt", "w");
  for(i=0; i<KK; i++)
  {
	//fprintf(f, "%14.10f  %14.10f  %14.10f\n", Re[i], Im[i], Re[i]*Re[i]+Im[i]*Im[i]);
	//
	Re[i]=Re[i]/KK;
	Im[i]=Im[i]/KK;
	fprintf(f, "%14.10f  %14.10f  %14.10f\n", Re[i], Im[i], Re[i]*Re[i]+Im[i]*Im[i]);
  }
  fclose(f);
}
//---------------------------------------------------------------------------
// AVal - массив анализируемых данных, Nvl - длина массива должна быть кратна степени 2.
// FTvl - массив полученных значений, Nft - длина массива должна быть равна Nvl.

void FFTAnalysis(double *AVal, double *FTvl, int Nvl, int Nft) {
  	int i, j, n, m, Mmax, Istp;
  	double Tmpr, Tmpi, Wtmp, Theta;
  	double Wpr, Wpi, Wr, Wi;
  	double *Tmvl;
  	n = Nvl * 2;
  	Tmvl = new double[n];
  	for (i = 0; i < n; i+=2) {
    	Tmvl[i] = 0;
        Tmvl[i+1] = AVal[i/2];
    }
  	i = 1;
    j = 1;
  	while (i < n) {
		if (j > i) {
	  		Tmpr = Tmvl[i];
            Tmvl[i] = Tmvl[j];
            Tmvl[j] = Tmpr;
	  		Tmpr = Tmvl[i+1];
            Tmvl[i+1] = Tmvl[j+1];
            Tmvl[j+1] = Tmpr;
		}
		i = i + 2; m = Nvl;
		while ((m >= 2) && (j > m)) {
        	j = j - m;
            m = m >> 1;
        }
		j = j + m;
  	}
  	Mmax = 2;
  	while (n > Mmax) {
		Theta = -TwoPi / Mmax;
        Wpi = sin(Theta);
		Wtmp = sin(Theta / 2);
        Wpr = Wtmp * Wtmp * 2;
		Istp = Mmax * 2;
        Wr = 1;
        Wi = 0;
        m = 1;
		while (m < Mmax) {
	  		i = m;
            m = m + 2;
            Tmpr = Wr;
            Tmpi = Wi;
	  		Wr = Wr - Tmpr * Wpr - Tmpi * Wpi;
	  		Wi = Wi + Tmpr * Wpi - Tmpi * Wpr;
	  		while (i < n) {
				j = i + Mmax;
				Tmpr = Wr * Tmvl[j] - Wi * Tmvl[j-1];
				Tmpi = Wi * Tmvl[j] + Wr * Tmvl[j-1];
				Tmvl[j] = Tmvl[i] - Tmpr;
                Tmvl[j-1] = Tmvl[i-1] - Tmpi;
				Tmvl[i] = Tmvl[i] + Tmpr;
                Tmvl[i-1] = Tmvl[i-1] + Tmpi;
				i = i + Istp;
	  		}
		}
		Mmax = Istp;
  	}
  	for (i = 0; i < Nft; i++) {
    	j = i * 2;
        FTvl[i] = 2*sqrt(pow(Tmvl[j],2) + pow(Tmvl[j+1],2))/Nvl;
    }
  	delete []Tmvl;
}
