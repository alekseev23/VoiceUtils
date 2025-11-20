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
    double val;
    int pos, N2 = N / 2;
    FrequencyData *FrequencyData = new class FrequencyData(N);
    for (int i = A; i < B; i++) {
	    pos = i * BufSize / Count;
	    FrequencyData->calculate(Buf, BufSize, pos, 512);
		// Сохраняем данные в массив
		for (int j = 0; j < N2; j++) {
			val = FrequencyData->Data[j];
			if (energy_specters) {
				val = val * 8000 * (j + 1) * N2 / (1364 * 4096);
    	        val = val * val;
			}
			Data[i * N2 + j] = val;
			if (val > MaxValue) MaxValue = val;
		}
    }
    delete FrequencyData;
}
//---------------------------------------------------------------------------
