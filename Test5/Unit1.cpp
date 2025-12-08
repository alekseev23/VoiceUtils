//---------------------------------------------------------------------------

#include <vcl.h>
#include <System.Math.hpp>
#pragma hdrstop

#include "Unit1.h"
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/strings.h"
#include "../Common/common.h"
#include "../Common/SpecterWork.h"
#include "../Common/TVoiceSpectersDB4.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define SER_MAX 100

TForm1 *Form1;
TVoiceSpectersDB4 *DB = NULL;
class SpecterWork *sw;
TVoiceSpectersDB4_Sound Sound;
int
	//N = 8192,
    N = 2048,
    d = 3;
SER SerOne[3];
SER SerAll[SER_MAX * 3];
SER SerOne2[3];
SER SerAll2[SER_MAX * 3];
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	UnicodeString title[] = {"Min", "Max", "Middle"};
	int Colors[] = {120, 120 << 8, 120 << 16};
	int Colors2[] = {250, 250 << 8, 250 << 16};
	DB = new TVoiceSpectersDB4;
	sw = new class SpecterWork;
	// Графики
	for (int i = 0; i < 3; i++) {
		SerOne[i].S = new TFastLineSeries(this);
		SerOne[i].S->Visible = false;
	    SerOne[i].S->Color = (TColor)(Colors[i]);
		SerOne[i].S->Title = title[i];
		Chart1->AddSeries(SerOne[i].S);

		SerOne2[i].S = new TFastLineSeries(this);
		SerOne2[i].S->Visible = false;
    	SerOne2[i].S->Color = (TColor)(Colors[i]);
		SerOne2[i].S->Title = title[i];
		Chart1->AddSeries(SerOne2[i].S);
	}
	for (int i = 0; i < SER_MAX ; i++) {
	    for (int j = 0; j < 3; j++) {
	        int k = i * 3 + j;
			SerAll[k].S = new TFastLineSeries(this);
			SerAll[k].S->Visible = false;
	        SerAll[k].S->Color = (TColor)(Colors[j]);
			Chart1->AddSeries(SerAll[k].S);

			SerAll2[k].S = new TFastLineSeries(this);
			SerAll2[k].S->Visible = false;
	        SerAll2[k].S->Color = (TColor)(Colors[j]);
			Chart1->AddSeries(SerAll2[k].S);
	    }
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N41Click(TObject *Sender)
{
	if (OpenDialog1->Execute()) {
		DB->Load(OpenDialog1->FileName);
		ListBox1->Clear();
		for (auto i = 0; i < DB->Count; i++) {
			ListBox1->Items->Add(UnicodeString(DB->Sounds[i].Sound) + "_" + UnicodeString(DB->Sounds[i].Id));
		}
	    // Обновляем список спектров
	    if (ListBox1->Count > 0) {
		    ListBox1->ItemIndex = 0;
	    	ListBox1Click(this);
	    }
        TitleUpdateExecute(this);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N3Click(TObject *Sender)
{
if (OpenDialog2->Execute()) {
	// Вычисление всех спектров
	sw->ReadDataFromWAV(UnicodeString(OpenDialog2->FileName));
	sw->CountSpectersThread(N, false, sw->BufSize);
	// Кол-во групповых спектров
	int Count = sw->Count / 480;
	// Вычисляем групповые спектры
	double Min[TVoiceSpectersDB4_M], Max[TVoiceSpectersDB4_M], Middle[TVoiceSpectersDB4_M];
	// Цикл по спектрам
    ListBox2->Clear();
	for (auto j = 0; j < Count; j++) {
        ListBox2->Items->Add(UnicodeString(j));
		sw->CountMiddleValues(j * 480, 480, TVoiceSpectersDB4_M, Min, Max, Middle);
		// Добавляем спектр в вектор звука
		Sound.Add(Min, Max, Middle);
	}
        TitleUpdateExecute(this);
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListBox1Click(TObject *Sender)
{
	if (ListBox1->Count == 0) return;
	// Добавляем список спектров
	ListBox3->Clear();
	for (auto i = 0; i < DB->Sounds[ListBox1->ItemIndex].Count; i++) {
	    ListBox3->Items->Add(UnicodeString(i));
	}
	// Строим график
	if (ListBox3->Count > 0) {
		ListBox3->ItemIndex = 0;
		ListBox3Click(this);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListBox3Click(TObject *Sender)
{
if (ListBox3->Count == 0) return;
// Скрываем все
MakeUnvisibleExecute(this);
SerOne[0].S->Visible = true;
SerOne[1].S->Visible = true;
SerOne[2].S->Visible = true;
for (auto i = 0; i < TVoiceSpectersDB4_M; i++) {
	SerOne[0].S->AddXY(i, DB->Sounds[ListBox1->ItemIndex].Specters[ListBox3->ItemIndex].Points[i].Min, "");
	SerOne[1].S->AddXY(i, DB->Sounds[ListBox1->ItemIndex].Specters[ListBox3->ItemIndex].Points[i].Max, "");
	SerOne[2].S->AddXY(i, DB->Sounds[ListBox1->ItemIndex].Specters[ListBox3->ItemIndex].Points[i].Middle, "");
}
StatusBar1->Panels[0].Items[0]->Text = "Sum=" + UnicodeString(DB->Sounds[ListBox1->ItemIndex].Specters[ListBox3->ItemIndex].Sum);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListBox1DblClick(TObject *Sender)
{  // Строим все графики выбранного зука из базы данных
if (ListBox3->Count == 0) return;
for (auto j = 0; j < ListBox3->Count; j++) {
    int k = j * 3;
	SerAll[k].S->Visible = true;
	SerAll[k + 1].S->Visible = true;
	SerAll[k + 2].S->Visible = true;
	for (auto i = 0; i < TVoiceSpectersDB4_M; i++) {
		SerAll[k].S->AddXY(i, DB->Sounds[ListBox1->ItemIndex].Specters[j].Points[i].Min, "");
		SerAll[k + 1].S->AddXY(i, DB->Sounds[ListBox1->ItemIndex].Specters[j].Points[i].Max, "");
		SerAll[k + 2].S->AddXY(i, DB->Sounds[ListBox1->ItemIndex].Specters[j].Points[i].Middle, "");
	}
}
StatusBar1->Panels[0].Items[0]->Text = "";
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MakeUnvisibleExecute(TObject *Sender)
{
	for (int i = 0; i < 3; i++) {
		SerOne[i].S->Visible = false;
		SerOne[i].S->Clear();
	}
	for (int i = 0; i < SER_MAX * 3; i++) {
		SerAll[i].S->Visible = false;
		SerAll[i].S->Clear();
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MakeUnvisible2Execute(TObject *Sender)
{
	for (int i = 0; i < 3; i++) {
		SerOne2[i].S->Visible = false;
		SerOne2[i].S->Clear();
	}
	for (int i = 0; i < SER_MAX * 3; i++) {
		SerAll2[i].S->Visible = false;
		SerAll2[i].S->Clear();
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListBox2Click(TObject *Sender)
{ // Строим график WAV-файла
if (ListBox2->Count == 0) return;
// Скрываем все
MakeUnvisible2Execute(this);
SerOne2[0].S->Visible = true;
SerOne2[1].S->Visible = true;
SerOne2[2].S->Visible = true;
for (auto i = 0; i < TVoiceSpectersDB4_M; i++) {
	SerOne2[0].S->AddXY(i, Sound.Specters[ListBox2->ItemIndex].Points[i].Min, "");
	SerOne2[1].S->AddXY(i, Sound.Specters[ListBox2->ItemIndex].Points[i].Max, "");
	SerOne2[2].S->AddXY(i, Sound.Specters[ListBox2->ItemIndex].Points[i].Middle, "");
}
StatusBar1->Panels[0].Items[1]->Text = "Sum=" + UnicodeString(Sound.Specters[ListBox2->ItemIndex].Sum);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListBox2DblClick(TObject *Sender)
{ // Строим все графики WAV-файла
if (ListBox2->Count == 0) return;
for (auto j = 0; j < ListBox2->Count; j++) {
    int k = j * 3;
	SerAll2[k].S->Visible = true;
	SerAll2[k + 1].S->Visible = true;
	SerAll2[k + 2].S->Visible = true;
	for (auto i = 0; i < TVoiceSpectersDB4_M; i++) {
		SerAll2[k].S->AddXY(i, Sound.Specters[j].Points[i].Min, "");
		SerAll2[k + 1].S->AddXY(i, Sound.Specters[j].Points[i].Max, "");
		SerAll2[k + 2].S->AddXY(i, Sound.Specters[j].Points[i].Middle, "");
	}
}
StatusBar1->Panels[0].Items[1]->Text = "";
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TitleUpdateExecute(TObject *Sender)
{
Form1->Caption = "Тест5 - " + OpenDialog1->FileName + " - " + OpenDialog2->FileName;
}
//---------------------------------------------------------------------------

