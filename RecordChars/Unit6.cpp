//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit6.h"
#include "Unit1.h"
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/strings.h"
#include "SpecterWork.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm6 *Form6;
extern UnicodeString CurPath;
extern SpecterWork *SpecterWork;
extern int N2, Scale;
//---------------------------------------------------------------------------
__fastcall TForm6::TForm6(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm6::AnalyzeExecute(TObject *Sender)
{
// Создаём список файлов
UnicodeString s, DirName = CurPath + "\\voices\\" + Form1->TabControl1->Tabs->Strings[Form1->TabControl1->TabIndex];
TStringList *ts = new TStringList;
GetFilesFromDirectory(DirName, "*.wav", false, false, ts);
// Цикл по списку
class SpecterWork *sw = new class SpecterWork;
double dif;
Memo1->Clear();
Chart1->Series[0]->Clear();
for (auto i = 0; i < ts->Count; i++) {
	// Вычисляем спектр
	sw->ReadData(ts->Strings[i]);
	sw->CountSpecters(N2 * 2);
	// Сравниваем спектры
	dif = SpecterWork->Compare(sw, Scale);
	// Выводим результат
	char w[100];
	sprintf(w, "%7.3f", dif);
	s = without_extension(get_file_name(ts->Strings[i]));
	ts->Strings[i] = UnicodeString(w) + "\t" + s;
    s = str_replace("_", "", s);
    Chart1->Series[0]->Add(dif, s, clRed);
}
//ts->Text = Memo1->Lines->Text;
ts->Sorted = true;
Memo1->Text = ts->Text;
delete sw;
delete ts;
}
//---------------------------------------------------------------------------
