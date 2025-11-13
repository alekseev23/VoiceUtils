//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit12.h"
#include "Unit1.h"
#include "../Common/SpecterWork.h"
#include "../Common/common.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define MAX12 (20*16)

TForm12 *Form12;
extern SpecterWork *SpecterWork;
SER Ser12[MAX12];
extern int Scale;
//---------------------------------------------------------------------------
__fastcall TForm12::TForm12(TComponent* Owner)
	: TForm(Owner)
{
for (int i = 0; i < MAX12; i++) {
	// Частоты
	Ser12[i].S = new TFastLineSeries(this);
	Ser12[i].S->Visible = false;
	Ser12[i].S->LinePen->Width = 3;
	Chart1->AddSeries(Ser12[i].S);
}
}
//---------------------------------------------------------------------------
void __fastcall TForm12::DrawExecute(TObject *Sender)
{
// Убираем старые графики
for (auto i = 0 ; i < MAX12; i++) {
	Ser12[i].S->Clear();
    Ser12[i].S->Visible = false;
}
int K;
switch (SpecterWork->N) {
    case  512: K = 20; break;
    case 1024: K = 40; break;
    case 2048: K = 80; break;
    case 4096: K = 160; break;
    case 8192: K = 320; break;
}
if (ListBox1->Count != K) {
    ListBox1->Clear();
    for (auto i = 0; i < K; i++) {
        ListBox1->Items->Add(i);
    }
    ListBox1->SelectAll();
}
for (auto i = 0; i < K; i++) {
    if (ListBox1->Selected[i]) {
	    Ser12[i].S->Visible = true;
    	for (auto j = 0; j < SpecterWork->Count; j++) {
        	Ser12[i].S->AddXY(j, SpecterWork->GetSpecterPointValue(j, i, Scale));
	    }
    }
}
}
//---------------------------------------------------------------------------
