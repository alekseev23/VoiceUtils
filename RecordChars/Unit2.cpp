//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;

extern UnicodeString CurPath;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm2::BitBtn2Click(TObject *Sender)
{
Form2->Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::BitBtn1Click(TObject *Sender)
{
UnicodeString s;
s = CurPath + "\\voices\\" + Edit1->Text;
if (DirectoryExists(s)) {
	ShowMessage("Голос уже существует!");
	return;
}
MkDir(s);
if (!DirectoryExists(s)) {
	ShowMessage("Ошибка создания каталога!");
	return;
}
else {
	Form1->UpdateVoicesListExecute(this);
	Form2->Close();
}
}
//---------------------------------------------------------------------------
