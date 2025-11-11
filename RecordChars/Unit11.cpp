//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "../Common/common.h"
#include "../Common/TPushAnalyzer.h"
#include "../Common/SpecterWork.h"
#include "Unit11.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm11 *Form11;
SER Gr1[_Q_SIZE], Gr2[_Q_SIZE], Gr3[_Q_SIZE], Gr4[_Q_SIZE];
extern SpecterWork *SpecterWork;
//---------------------------------------------------------------------------
__fastcall TForm11::TForm11(TComponent* Owner)
	: TForm(Owner)
{
int color[] = {
	255 + (0 << 8) + (0 << 16),
	128 + (0 << 8) + (0 << 16),
	128 + (128 << 8) + (0 << 16),
	0 + (255 << 8) + (0 << 16),
	255 + (255 << 8) + (0 << 16),
	0 + (128 << 8) + (128 << 16),
	0 + (255 << 8) + (128 << 16),
	0 + (128 << 8) + (128 << 16),
	128 + (0 << 8) + (255 << 16),
	128 + (0 << 8) + (128 << 16) };
int width[] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
for (int i = 0; i < _Q_SIZE; i++) {
	//
	Gr1[i].S = new TFastLineSeries(this);
	//Gr1[i].S->Visible = false;
	Gr1[i].S->Title = UnicodeString(i);
	Gr1[i].S->Color = (TColor)(color[i]);
	Gr1[i].S->LinePen->Width = width[i];
	Chart1->AddSeries(Gr1[i].S);
	//
	Gr2[i].S = new TFastLineSeries(this);
	//Gr2[i].S->Visible = false;
	Gr2[i].S->Title = UnicodeString(i);
	Gr2[i].S->Color = (TColor)(color[i]);
	Gr2[i].S->LinePen->Width = width[i];
	Chart2->AddSeries(Gr2[i].S);
	//
	Gr3[i].S = new TFastLineSeries(this);
	//Gr3[i].S->Visible = false;
	Gr3[i].S->Title = UnicodeString(i);
	Gr3[i].S->Color = (TColor)(color[i]);
	Gr3[i].S->LinePen->Width = width[i];
	Chart3->AddSeries(Gr3[i].S);
	//
	Gr4[i].S = new TFastLineSeries(this);
	//Gr4[i].S->Visible = false;
	Gr4[i].S->Title = UnicodeString(i);
	Gr4[i].S->Color = (TColor)(color[i]);
	Gr4[i].S->LinePen->Width = width[i];
	Chart4->AddSeries(Gr4[i].S);
}

}
//---------------------------------------------------------------------------
void __fastcall TForm11::FormResize(TObject *Sender)
{
Chart1->Height = Form11->ClientHeight / 4;
Chart2->Height = Chart1->Height;
Chart3->Height = Chart1->Height;
}
//---------------------------------------------------------------------------
void __fastcall TForm11::Action1Execute(TObject *Sender)
{
int range = 480, step = 48;
TPushAnalyzer *PushAnalyzer = new TPushAnalyzer;
PushAnalyzer->Analyze(SpecterWork->Buf, SpecterWork->BufSize, 0, SpecterWork->BufSize, range, step);

TStringList *ts = new TStringList;

for (auto i = 0; i < _Q_SIZE; i++) {
	Gr1[i].S->Clear();
	Gr2[i].S->Clear();
	Gr3[i].S->Clear();
	Gr4[i].S->Clear();

//	ts->Clear();
	UnicodeString s;
	for (auto j = 0; j < PushAnalyzer->Count; j++) {
	s = s + UnicodeString(PushAnalyzer->Data[j].VP.V[i]) + " ";
		Gr1[i].S->AddXY(j, PushAnalyzer->Data[j].VP.V[i], "");
		Gr2[i].S->AddXY(j, PushAnalyzer->Data[j].VM.V[i], "");
		Gr3[i].S->AddXY(j, PushAnalyzer->Data[j].HP.V[i], "");
		Gr4[i].S->AddXY(j, PushAnalyzer->Data[j].HM.V[i], "");
	}
	ts->Add(s);
}
//ShowMessage(ts->Text);
delete ts;

delete PushAnalyzer;
}
//---------------------------------------------------------------------------
