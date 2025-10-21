//---------------------------------------------------------------------------

#ifndef TSpecterAnalyzerH
#define TSpecterAnalyzerH
//---------------------------------------------------------------------------

class TSpecterAnalyzer {

public:

    int
    	N,
        MaxPos;
    double
        Sum,
        Max;
    float x[100], y[100];

	__fastcall TSpecterAnalyzer();
	__fastcall ~TSpecterAnalyzer();
   	void __fastcall Analyze(int n, float *data);
   	void __fastcall Analyze(int n, double *data);

};
#endif
