//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit14.h"
#include "Unit1.h"
#include "../Common/FFT.h"
#include "../Common/FrequencyData.h"
#include "../Common/common.h"
#include "../Common/TSpecters.h"
#include "../Common/SpecterWork.h"
#include "../Common/TAudioMax.h"
#include "../Common/TColorSceme.h"
#include "../../_Common_/Lib/files.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm14 *Form14;

#define MAX_SER_14 310
SpecterWork *SpecterWork14;
SER Ser14[MAX_SER_14], Serr14[MAX_SER_14];
UnicodeString Name14[MAX_SER_14];
int Count14, Colors14[31];
extern UnicodeString CurPath;
extern int N2, Scale;
//---------------------------------------------------------------------------
__fastcall TForm14::TForm14(TComponent* Owner)
	: TForm(Owner)
{
for (int i = 0; i < MAX_SER_14; i++) {
	// Частоты
	Ser14[i].S = new TFastLineSeries(this);
	Ser14[i].S->Visible = false;
	Chart1->AddSeries(Ser14[i].S);
	Serr14[i].S = new TFastLineSeries(this);
	Serr14[i].S->Visible = false;
	Chart2->AddSeries(Serr14[i].S);
}
SpecterWork14 = new SpecterWork;
for (int i = 0; i < 31; i++) {
    Colors14[i] = random(255) + (random(255) << 8) + (random(255) << 16);
}
}
//---------------------------------------------------------------------------
void __fastcall TForm14::BitBtn1Click(TObject *Sender)
{ // Выделить все
for (int i = 0; i < ListBox1->Items->Count; i++) {
    ListBox1->SelectAll();
}
DrawExecute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm14::BitBtn2Click(TObject *Sender)
{ // Отменить выбор
for (int i = 0; i < ListBox1->Items->Count; i++) {
    ListBox1->ClearSelection();
}
DrawExecute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm14::DrawExecute(TObject *Sender)
{ // Вывести графики
for (int i = 0; i < MAX_SER_14; i++) {
    if (i < Count14) {
        for (int j = 0; j < ListBox1->Count; j++) {
            if (ListBox1->Items->Strings[j] == Name14[i]) {
		    	Ser14[i].S->Visible = ListBox1->Selected[j];
		    	Serr14[i].S->Visible = ListBox1->Selected[j];
            	break;
            }
        }
    }
    else {
    	Ser14[i].S->Visible = false;
    	Serr14[i].S->Visible = false;
    }
}
}
//---------------------------------------------------------------------------
void __fastcall TForm14::CalculateExecute(TObject *Sender)
{ // Вычислить графики
double val;
UnicodeString s, filename, dirname, name;
int k = 0, n, b = 131, m;
b = 131 / 3;
n = N2 / b + 1;
TStringList *ts = new TStringList;
double *V = new double [n * MAX_SER_14];
for (auto i = 0; i < n * MAX_SER_14; i++) V[i] = 0.;
// Очищаем все графики
for (auto i = 0; i < MAX_SER_14; i++) {
	Ser14[i].S->Clear();
	Serr14[i].S->Clear();
}
// Цикл по всем звукам
for (auto z = 0; z < Form1->ListBox1->Items->Count; z++) {
//for (auto z = 0; z < 10; z++) {

	name = Form1->ListBox1->Items->Strings[z];
	dirname = CurPath + "\\voices\\" + Form1->TabControl1->Tabs->Strings[Form1->TabControl1->TabIndex];
    ts->Clear();
	GetFilesFromDirectory(dirname, name + "*.wav", false, false, ts);
	// Цикл по всем файлам в выбранном звуке
	//for (auto i = 0; (i < ts->Count) && (i < 6); i++) {
    for (auto i = 0; i < ts->Count; i++) {
		// Читаем файл
        filename = ts->Strings[i];
		// Грузим данные из DAT файла
		if (!FileExists(filename)) {
			ShowMessage("Файл не найден: " + filename);
			continue;
		}

        try {

		// Находим спектры
		SpecterWork14->ReadData(filename);
		SpecterWork14->CountSpecters(N2 * 2, Form1->N20->Checked);
		SpecterWork14->GetMiddle(0, Scale);
		// Строим график
        Name14[k] = Form1->ListBox1->Items->Strings[z];
        Ser14[k].S->Color = (TColor)Colors14[z];
        Ser14[k].S->LinePen->Width = 2;
        Ser14[k].S->Title = without_extension(get_file_name(filename));
        Serr14[k].S->Color = (TColor)Colors14[z];
        Serr14[k].S->LinePen->Width = 3;
        Serr14[k].S->Title = without_extension(get_file_name(filename));
        // Нормировка значений
        double sum = 0.;
        for (auto j = 0; j < SpecterWork14->N2; j++) {
            sum += SpecterWork14->Middle[j];
        }
        sum = 1e3 / sum;
		for (auto j = 0; j < SpecterWork14->N2; j++) {
			val = sum * SpecterWork14->Middle[j];
			Ser14[k].S->AddXY(j, val, "");
            //
            m = j / b;
            V[k * n + m] += val;
		}
        k++;
        if (k >= MAX_SER_14) break;

        } catch (...) { ShowMessage("Exception 1");
        }
	}
   	if (k >= MAX_SER_14) break;
}
Count14 = k;
Chart1->Title->Text->Clear();
Chart1->Title->Text->Add("Спектры всех звуков");
delete ts;

try {

// Находим минимум и максимум V
double *min = new double [n];
double *max = new double [n];
for (auto i = 0; i < n; i++) {
    min[i] = 1e8;
    max[i] = 0.;
}
for (auto i = 0; i < Count14; i++) {
    for (auto j = 0; j < n; j++) {
        if (V[i * n + j] > max[j]) max[j] = V[i * n + j];
        if (V[i * n + j] < min[j]) min[j] = V[i * n + j];
    }
}
for (auto i = 0; i < Count14; i++) {
    for (auto j = 0; j < n; j++) {
    	val = (V[i * n + j] - min[j]) / (max[j] - min[j]);
        Serr14[i].S->AddXY(j, val, "");
    }
}

delete[] V;
delete[] min;
delete[] max;

} catch (...) { ShowMessage("Exception 2");
}

}
//---------------------------------------------------------------------------
void __fastcall TForm14::ListBox1Click(TObject *Sender)
{
DrawExecute(this);
}
//---------------------------------------------------------------------------
