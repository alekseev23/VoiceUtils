//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit4.h"
#include "Unit1.h"
#include "../Common/FFT.h"
#include "../Common/FrequencyData.h"
#include "../Common/common.h"
#include "../Common/TSpecters.h"
#include "../Common/SpecterWork.h"
#include "../Common/TAudioMax.h"
#include "../Common/TColorSceme.h"
#include "../Common/TWavFile.h"
#include "../Common/Freq2Note.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define MAX_SER_4 30

TForm4 *Form4;
SpecterWork *SpecterWork4;
SER Ser4[MAX_SER_4];           // Спектры текущего файла
extern UnicodeString CurPath;
extern int N2, Scale;
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
	: TForm(Owner)
{
for (int i = 0; i < MAX_SER_4; i++) {
	// Частоты
	Ser4[i].S = new TFastLineSeries(this);
	Ser4[i].S->Visible = false;
	Chart1->AddSeries(Ser4[i].S);
}
SpecterWork4 = new SpecterWork;
}
//---------------------------------------------------------------------------
void __fastcall TForm4::DrawAllExecute(TObject *Sender)
{
double val;
UnicodeString s;
class Freq2Note *Freq2Note = new class Freq2Note;
// Очищаем все графики
for (auto i = 0; i < MAX_SER_4; i++) {
	Ser4[i].S->Clear();
	Ser4[i].S->Visible = false;
}
TStringList *ts = new TStringList;
// Цикл по всем файлам в выбранном звуке
for (auto i = 0; i < Form1->ListBox2->Items->Count; i++) {
	// Читаем файл
	UnicodeString filename = CurPath + "\\voices\\"
		+ Form1->TabControl1->Tabs->Strings[Form1->TabControl1->TabIndex]
		+ "\\" + Form1->ListBox1->Items->Strings[Form1->ListBox1->ItemIndex] + "_"
		+ Form1->ListBox2->Items->Strings[i] + ".wav";
	// Грузим данные из DAT файла
	if (!FileExists(filename)) {
		ShowMessage("Файл не найден: " + filename);
		continue;
	}
	// Находим спектры
	SpecterWork4->ReadData(filename);
	SpecterWork4->CountSpecters(N2 * 2, Form1->N20->Checked);
	SpecterWork4->GetMiddle(0, Scale);
	// Строим график
	Ser4[i].S->Visible = true;
	Ser4[i].S->Title = UnicodeString(Form1->ListBox2->Items->Strings[i]);
	for (auto j = 0; j < SpecterWork4->N2; j++) {
		val = SpecterWork4->Middle[j];
		Ser4[i].S->AddXY(j, val, "");
	}
    // Находим точки максимума
	for (auto j = 1; j < SpecterWork4->N2 - 1; j++) {
        if ((SpecterWork4->Middle[j] > SpecterWork4->Middle[j - 1]) && (SpecterWork4->Middle[j] > SpecterWork4->Middle[j + 1])) {
            s.printf(L"%4i", j);
            int f = Freq2Note->Get(j, SpecterWork4->N2 * 2);
            ts->Add(s + "\t" + Freq2Note->Comment[f]);
        }
    }
}
ts->Sort();
Memo1->Text = ts->Text;
delete ts;
delete Freq2Note;

Chart1->Title->Text->Clear();
Chart1->Title->Text->Add("Средние спектры звука [" + Form1->ListBox1->Items->Strings[Form1->ListBox1->ItemIndex]
	+ "] голоса [" + Form1->TabControl1->Tabs->Strings[Form1->TabControl1->TabIndex] + "]");

}
//---------------------------------------------------------------------------

void __fastcall TForm4::Memo1DblClick(TObject *Sender)
{
Memo1->SelectAll();
Memo1->CopyToClipboard();
}
//---------------------------------------------------------------------------

