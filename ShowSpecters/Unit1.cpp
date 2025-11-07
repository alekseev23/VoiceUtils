//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "../../_Common_/Lib/bitmap.h"
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/strings.h"
#include "../Common/common.h"
#include "../Common/TVoiceSpectersDB.h"
#include "../Common/TVoiceSpecters2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

int Band[20];

double *Specters = NULL;
TVoiceSpectersDB *VoiceSpectersDB = NULL;
TVoiceSpecters2 *VoiceSpecters2 = NULL;
int
	N2 = 4096,
	SX = 4096,
	SY = 500,
	Scale = 1;
SER SerMain[5];             // Основные спектры
SER SerData;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
VoiceSpectersDB = new class TVoiceSpectersDB;
VoiceSpecters2 = new class TVoiceSpecters2;
for (int i = 0; i < VoiceSpectersDB->MainCount; i++) {
	// Частоты
	SerMain[i].S = new TFastLineSeries(this);
	SerMain[i].S->Visible = true;
	Chart1->AddSeries(SerMain[i].S);
}
SerData.S = new TFastLineSeries(this);
SerData.S->LinePen->Width = 3;
Chart1->AddSeries(SerData.S);
Image1->Width = SX;
Image1->Height = SY;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::vdb21Click(TObject *Sender)
{
if (OpenDialog1->Execute()) {
	PageControl1->ActivePageIndex = 0;
//ShowMessage("Before load");
	VoiceSpecters2->Load(OpenDialog1->FileName);
//ShowMessage("Aftre load");
	ListBox1->Clear();
	for (auto i = 0; i < VoiceSpecters2->Count; i++) {
		ListBox1->Items->Add(UnicodeString(VoiceSpecters2->VoiceSpecters[i].Sound)
			+ "_" + UnicodeString((int)VoiceSpecters2->VoiceSpecters[i].Id));
	}
	ListBox1->ItemIndex = 0;
	ListBox1Click(this);
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::vdb31Click(TObject *Sender)
{
if (OpenDialog2->Execute()) {
	PageControl1->ActivePageIndex = 1;
	VoiceSpectersDB->Load(OpenDialog2->FileName);
	ListBox2->Clear();
	// Добавляем названия звуков и версии
	for (auto i = 0; i < VoiceSpectersDB->Count; i++) {
		ListBox2->Items->Add(UnicodeString(VoiceSpectersDB->VoiceSpecters[i].Sound)
			+ "_" + UnicodeString(VoiceSpectersDB->VoiceSpecters[i].Id));
	}
	ListBox2->ItemIndex = 0;
	ListBox2Click(this);
}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListBox2Click(TObject *Sender)
{
// Создаём изображение
TBitmap *bmp = new TBitmap;
bmp->PixelFormat = pf24bit;
bmp->SetSize(SX, SY);
int h = SY / VoiceSpectersDB->MainCount;
int n = ListBox2->ItemIndex;
// Рисуем максимумы
for (auto i = 0; i < VoiceSpectersDB->MainCount; i++) {
	int y = (i + 1) * h - 1;
//ShowMessage(VoiceSpectersDB->VoiceSpecters[n].Specters[i].Count);
	for (auto j = 0; j < VoiceSpectersDB->VoiceSpecters[n].Specters[i].Count; j++) {
		int x = VoiceSpectersDB->VoiceSpecters[n].Specters[i].Freq[j];
		bmp->Canvas->MoveTo(x, y);
		int dx = (h -5) * VoiceSpectersDB->VoiceSpecters[n].Specters[i].Value[j] / 32767;
        bmp->Canvas->LineTo(x, y - dx);
	}

}
// Присваиваем изображение
Image1->Picture->Bitmap->Assign(bmp);
// Удаляем ненужные объекты
delete bmp;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N23Click(TObject *Sender)
{ // Выход
Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N4Click(TObject *Sender)
{ // Запустить копию
ShellExecute(0,L"open",Application->ExeName.w_str(),NULL,NULL,SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N6Click(TObject *Sender)
{ // Перезапустить
ShellExecute(0,L"open",Application->ExeName.w_str(),NULL,NULL,SW_SHOWNORMAL);
Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListBox1Click(TObject *Sender)
{ //
if (ListBox1->Count == 0) return;
// Строим график
double val;
for (auto j = 0; j < VoiceSpecters2->MainCount; j++) {
	SerMain[j].S->Clear();
	for (auto i = 0; i < N2; i++) {
		val = VoiceSpecters2->VoiceSpecters[ListBox1->ItemIndex].Data[j * N2 + i];
		//val = VoiceSpecters->GetSpecterValue(val, Scale);
		SerMain[j].S->AddXY(i, val, "");
	}
}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::N7Click(TObject *Sender)
{
Form2->Show();
}
//---------------------------------------------------------------------------
