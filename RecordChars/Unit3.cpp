//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{
for (auto i = 0; i < 256; i++) {
	ListBox1->Items->Add(UnicodeString(i));
}
ListBox1->Selected[0] = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm3::ListBox1Click(TObject *Sender)
{
Form1->TabControl2Change(this);
}
//---------------------------------------------------------------------------
