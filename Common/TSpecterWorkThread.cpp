//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "TSpecterWorkThread.h"
#include "SpecterWork.h"
#include "FrequencyData.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TSpecterWorkThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TSpecterWorkThread::TSpecterWorkThread(bool CreateSuspended)
	: TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall TSpecterWorkThread::Execute()
{
	//---- Place thread code here ----
    double val, f;
    int N2 = N / 2;
    __int64 t, pos;
    FrequencyData *FrequencyData = new class FrequencyData(N);
    FrequencyData->applyShape(ShapeType);
    if (End == 0) End = BufSize - 1;
    for (int i = A; i < B; i++) {
	    pos = 1LL * i * (End - Begin) / Count + Begin;
        //pos = 1LL * i * (End - Begin) / Count;
	    FrequencyData->calculate(Buf, BufSize, (int)pos, WindowSize);
		// Сохраняем данные в массив
        t = 1LL * i * N2;
		for (int j = 0; j < N2; j++) {
			val = FrequencyData->Data[j];
			if (EnergySpecters) {
				//val = val * 8000 * (j + 1) * N2 / (1364 * 4096);

                //val = val * 8000 * (j + 1) * N2 / (1364 * 4096);
    	        //val = val * val;

                f = 8000 * 4096 * (j + 1) / (1364 * N2);
                val = val * f * f;
			}
			Data[t++] = val;
			if (val > MaxValue) MaxValue = val;
		}
/*if ((i >= 12000) && (i <= 12000)) {
    std::cout << " i=" << i << " pos=" << (int)pos << " FrequencyData->Data[0]=" <<  FrequencyData->Data[0]
        << " Begin=" << Begin << " End=" << End << " A=" << A << " B=" << B
    	<< std::endl;
}*/
    }
    delete FrequencyData;
}
//---------------------------------------------------------------------------
