//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
extern int Band[20];

//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
StringGrid1->Cells[0][0] = "№";
StringGrid1->Cells[1][0] = "Начало";
StringGrid1->Cells[2][0] = "Конец";
for (auto i = 0; i < 10; i++) StringGrid1->Cells[0][i + 1] = UnicodeString(i);
BitBtn5Click(this);
StringGrid1Exit(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm2::BitBtn4Click(TObject *Sender)
{
for (auto i = 0; i < 10; i++) {
	StringGrid1->Cells[1][i + 1] = "";
	StringGrid1->Cells[2][i + 1] = "";
}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::BitBtn5Click(TObject *Sender)
{
StringGrid1->Cells[1][1] = "0"; 	StringGrid1->Cells[2][1] = "2";
StringGrid1->Cells[1][2] = "3"; 	StringGrid1->Cells[2][2] = "5";
StringGrid1->Cells[1][3] = "6"; 	StringGrid1->Cells[2][3] = "10";
StringGrid1->Cells[1][4] = "11"; 	StringGrid1->Cells[2][4] = "15";
StringGrid1->Cells[1][5] = "16"; 	StringGrid1->Cells[2][5] = "20";
StringGrid1->Cells[1][6] = "21"; 	StringGrid1->Cells[2][6] = "25";
StringGrid1->Cells[1][7] = "26"; 	StringGrid1->Cells[2][7] = "100";
StringGrid1->Cells[1][8] = "101"; 	StringGrid1->Cells[2][8] = "1000";
StringGrid1->Cells[1][9] = "1001"; 	StringGrid1->Cells[2][9] = "2000";
StringGrid1->Cells[1][10] = "2001"; 	StringGrid1->Cells[2][10] = "4096";
}
//---------------------------------------------------------------------------
void __fastcall TForm2::BitBtn1Click(TObject *Sender)
{
Memo1->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::BitBtn2Click(TObject *Sender)
{
Memo1->SelectAll();
Memo1->CopyToClipboard();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::StringGrid1Exit(TObject *Sender)
{
int i = 0;
Band[i++] = StringGrid1->Cells[1][1].ToIntDef(0);   Band[i++] = StringGrid1->Cells[2][1].ToIntDef(0);
Band[i++] = StringGrid1->Cells[1][2].ToIntDef(0);   Band[i++] = StringGrid1->Cells[2][2].ToIntDef(0);
Band[i++] = StringGrid1->Cells[1][3].ToIntDef(0);   Band[i++] = StringGrid1->Cells[2][3].ToIntDef(0);
Band[i++] = StringGrid1->Cells[1][4].ToIntDef(0);   Band[i++] = StringGrid1->Cells[2][4].ToIntDef(0);
Band[i++] = StringGrid1->Cells[1][5].ToIntDef(0);   Band[i++] = StringGrid1->Cells[2][5].ToIntDef(0);
Band[i++] = StringGrid1->Cells[1][6].ToIntDef(0);   Band[i++] = StringGrid1->Cells[2][6].ToIntDef(0);
Band[i++] = StringGrid1->Cells[1][7].ToIntDef(0);   Band[i++] = StringGrid1->Cells[2][7].ToIntDef(0);
Band[i++] = StringGrid1->Cells[1][8].ToIntDef(0);   Band[i++] = StringGrid1->Cells[2][8].ToIntDef(0);
Band[i++] = StringGrid1->Cells[1][9].ToIntDef(0);   Band[i++] = StringGrid1->Cells[2][9].ToIntDef(0);
Band[i++] = StringGrid1->Cells[1][10].ToIntDef(0);   Band[i++] = StringGrid1->Cells[2][10].ToIntDef(0);
}
//---------------------------------------------------------------------------
