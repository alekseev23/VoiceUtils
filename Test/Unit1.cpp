//---------------------------------------------------------------------------

#include <vcl.h>
#include <System.Math.hpp>
#pragma hdrstop

#include "Unit1.h"
#include "../RecordChars/SpecterWork.h"
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/strings.h"
#include "../Common/common.h"
#include "../AnalyzeRecord/TVoiceSpecters.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm7 *Form7;
double *Specters = NULL;
TVoiceSpecters *VoiceSpecters = NULL;
int
	N2 = 256,
	Scale = 1;
SER SerMain[5];             // Основные спектры
SER SerData;
//---------------------------------------------------------------------------
__fastcall TForm7::TForm7(TComponent* Owner)
	: TForm(Owner)
{
VoiceSpecters = new TVoiceSpecters;
for (int i = 0; i < 5; i++) {
	// Частоты
	SerMain[i].S = new TFastLineSeries(this);
	SerMain[i].S->Visible = true;
	Chart1->AddSeries(SerMain[i].S);
}
SerData.S = new TFastLineSeries(this);
SerData.S->LinePen->Width = 3;
Chart1->AddSeries(SerData.S);
}
//---------------------------------------------------------------------------
void __fastcall TForm7::N41Click(TObject *Sender)
{
if (OpenDialog1->Execute()) {
	VoiceSpecters->Load(OpenDialog1->FileName);
	ListBox1->Clear();
	for (auto i = 0; i < VoiceSpecters->Count; i++) {
		ListBox1->Items->Add(UnicodeString(VoiceSpecters->VoiceSpecters[i].Char)
			+ "_" + UnicodeString((int)VoiceSpecters->VoiceSpecters[i].Id));
	}
}
}
//---------------------------------------------------------------------------
void __fastcall TForm7::N3Click(TObject *Sender)
{
if (OpenDialog2->Execute()) {
	if (Specters != NULL) delete[] Specters;
	int fsize = getFileSize(OpenDialog2->FileName);
	int count = fsize / (8 * N2);
	Specters = new double [count * N2];
	FILE *fd = _wfopen(OpenDialog2->FileName.w_str(), L"rb");
	fread(Specters, 8, count * N2, fd);
	fclose(fd);
	ListBox2->Clear();
	for (auto i = 0; i < count; i++) {
		ListBox2->Items->Add(UnicodeString(i));
	}
}
}
//---------------------------------------------------------------------------
void __fastcall TForm7::ListBox1Click(TObject *Sender)
{
if (ListBox1->Count == 0) return;
// Строим график
double val;
for (auto j = 0; j < 5; j++) {
	SerMain[j].S->Clear();
	for (auto i = 0; i < N2; i++) {
		val = VoiceSpecters->VoiceSpecters[ListBox1->ItemIndex].Data[j * N2 + i];
		val = VoiceSpecters->GetSpecterValue(val, Scale);
		SerMain[j].S->AddXY(i, val, "");
	}
}
}
//---------------------------------------------------------------------------
void __fastcall TForm7::ListBox2Click(TObject *Sender)
{
if (ListBox2->Count == 0) return;
// Строим график
double val;
SerData.S->Clear();
for (auto i = 0; i < N2; i++) {
	val = Specters[ListBox2->ItemIndex * N2 + i];
	SerData.S->AddXY(i, val, "");
}
}
//---------------------------------------------------------------------------
void __fastcall TForm7::N5Click(TObject *Sender)
{
N5->Checked = true;
Scale = 1;
ListBox1Click(this);
ListBox2Click(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm7::N6Click(TObject *Sender)
{
N6->Checked = true;
Scale = 2;
ListBox1Click(this);
ListBox2Click(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm7::X1Click(TObject *Sender)
{
X1->Checked = true;
Scale = 3;
ListBox1Click(this);
ListBox2Click(this);
}
//---------------------------------------------------------------------------
