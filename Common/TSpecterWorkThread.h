//---------------------------------------------------------------------------

#ifndef TSpecterWorkThreadH
#define TSpecterWorkThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class TSpecterWorkThread : public TThread
{
private:
protected:
	void __fastcall Execute();
public:
	__fastcall TSpecterWorkThread(bool CreateSuspended);
    int A, B, N, BufSize, Count;
    double *Data, MaxValue = 0.;
    short *Buf;
    bool energy_specters = false;
};
//---------------------------------------------------------------------------
#endif
