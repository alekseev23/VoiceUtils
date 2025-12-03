//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit13.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm13 *Form13;
//---------------------------------------------------------------------------
__fastcall TForm13::TForm13(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm13::Button1Click(TObject *Sender)
{ // Очистить
Memo1->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TForm13::Button2Click(TObject *Sender)
{ // Копировать
Memo1->SelectAll();
Memo1->CopyToClipboard();
}
//---------------------------------------------------------------------------
