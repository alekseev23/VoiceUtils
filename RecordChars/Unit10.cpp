//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit10.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm10 *Form10;
//---------------------------------------------------------------------------
__fastcall TForm10::TForm10(TComponent* Owner)
	: TForm(Owner)
{
ComboBox1->ItemIndex = 2;
ComboBox2->ItemIndex = 2;
}
//---------------------------------------------------------------------------
void __fastcall TForm10::BitBtn1Click(TObject *Sender)
{
ComboBox1->ItemIndex = 2;
ComboBox2->ItemIndex = 2;
ApplyExecute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm10::ApplyExecute(TObject *Sender)
{
switch (ComboBox1->ItemIndex) {
	case 0: Form1->Image3->Width = Form1->Image3->Picture->Bitmap->Width / 3; break;
	case 1: Form1->Image3->Width = Form1->Image3->Picture->Bitmap->Width / 2; break;
	case 2: Form1->Image3->Width = Form1->Image3->Picture->Bitmap->Width; break;
	case 3: Form1->Image3->Width = 3 * Form1->Image3->Picture->Bitmap->Width / 2; break;
	case 4: Form1->Image3->Width = 2 * Form1->Image3->Picture->Bitmap->Width; break;
	case 5: Form1->Image3->Width = 3 * Form1->Image3->Picture->Bitmap->Width; break;
	case 6: Form1->Image3->Width = 4 * Form1->Image3->Picture->Bitmap->Width; break;
	case 7: Form1->Image3->Width = 5 * Form1->Image3->Picture->Bitmap->Width; break;
	case 8: Form1->Image3->Width = 10 * Form1->Image3->Picture->Bitmap->Width; break;
}
switch (ComboBox2->ItemIndex) {
	case 0: Form1->Image3->Height = Form1->Image3->Picture->Bitmap->Height / 3; break;
	case 1: Form1->Image3->Height = Form1->Image3->Picture->Bitmap->Height / 2; break;
	case 2: Form1->Image3->Height = Form1->Image3->Picture->Bitmap->Height; break;
	case 3: Form1->Image3->Height = 3 * Form1->Image3->Picture->Bitmap->Height / 2; break;
	case 4: Form1->Image3->Height = 2 * Form1->Image3->Picture->Bitmap->Height; break;
	case 5: Form1->Image3->Height = 3 * Form1->Image3->Picture->Bitmap->Height; break;
	case 6: Form1->Image3->Height = 4 * Form1->Image3->Picture->Bitmap->Height; break;
	case 7: Form1->Image3->Height = 5 * Form1->Image3->Picture->Bitmap->Height; break;
	case 8: Form1->Image3->Height = 10 * Form1->Image3->Picture->Bitmap->Height; break;
}
}
//---------------------------------------------------------------------------
void __fastcall TForm10::BitBtn3Click(TObject *Sender)
{
Form10->Close();
}
//---------------------------------------------------------------------------
