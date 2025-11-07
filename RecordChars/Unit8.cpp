//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit8.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm8 *Form8;
extern bool BUSY;

namespace AutoWork {
	int Voice, Char, Variant;
}
//---------------------------------------------------------------------------
__fastcall TForm8::TForm8(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm8::BitBtn1Click(TObject *Sender)
{ // Старт
if (Timer1->Enabled) {
	ShowMessage("Процесс уже запущен!");
	return;
}
if (CheckBox1->Checked) AutoWork::Voice = 0;
else AutoWork::Voice = Form1->TabControl1->TabIndex;
AutoWork::Char = 0;
AutoWork::Variant = 0;
Timer1->Enabled = true;
ProgressBar1->Position = 0;
ProgressBar1->Max = 1;
ProgressBar2->Position = 0;
ProgressBar2->Max = 1;
ProgressBar3->Position = 0;
ProgressBar3->Max = 1;
}
//---------------------------------------------------------------------------
void __fastcall TForm8::BitBtn2Click(TObject *Sender)
{ // Стоп
Timer1->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm8::BitBtn3Click(TObject *Sender)
{ // Закрыть
BitBtn2Click(this);
Form8->Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm8::FormCloseQuery(TObject *Sender, bool &CanClose)
{
BitBtn2Click(this);
CanClose = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm8::Timer1Timer(TObject *Sender)
{
// Если заняты, то пока отдыхаем
if (BUSY) return;
// Выбираем нужный голос
if (Form1->TabControl1->TabIndex != AutoWork::Voice) {
	Form1->TabControl1->TabIndex = AutoWork::Voice;
}
// Выбираем нужный звук
if (Form1->ListBox1->ItemIndex != AutoWork::Char) {
	Form1->ListBox1->ItemIndex = AutoWork::Char;
	Form1->ListBox1Click(this);
}
else {
	// Выбираем нужный вариант
	if (Form1->ListBox2->ItemIndex != AutoWork::Variant) {
		Form1->ListBox2->ItemIndex = AutoWork::Variant;
		Form1->ListBox2Click(this);
		// Делаем скриншоты
		if (CheckBox2->Checked) Form1->N21Click(this); // Скриншоты - Аудиоданные
		if (CheckBox3->Checked) Form1->N243Click(this); // Скриншоты - Все спектры данного звука
		if (CheckBox4->Checked) Form1->N255Click(this); // Скриншоты - Все спектры
		if (CheckBox5->Checked) Form1->N30Click(this); // Скриншоты - Средние спектры
		if (CheckBox6->Checked) Form1->N3D1Click(this); // Скриншоты - 3D
		if (CheckBox7->Checked) Form1->N31Click(this); // Скриншоты - Спектрограмма
	}
}
//Form1->TabControl2Change(this);
// Проверяем, что всё сделали
if (((AutoWork::Voice == Form1->TabControl1->Tabs->Count - 1) && CheckBox1->Checked) || !CheckBox1->Checked) {
	if (AutoWork::Char == Form1->ListBox1->Items->Count - 1) {
		if (AutoWork::Variant == Form1->ListBox2->Items->Count - 1) {
			Timer1->Enabled = false;
			ShowMessage("Задание выполнено!");
			return;
		}
	}
}
// Рисуем статистику
if (CheckBox1->Checked) {
	ProgressBar1->Max = Form1->TabControl1->Tabs->Count;
	ProgressBar1->Position = AutoWork::Voice;
}
else {
	ProgressBar1->Max = 1;
	ProgressBar1->Position = AutoWork::Voice;
}
ProgressBar2->Max = Form1->ListBox1->Items->Count;
ProgressBar2->Position = AutoWork::Char + 1;
ProgressBar3->Max = Form1->ListBox2->Items->Count;
ProgressBar3->Position = AutoWork::Variant + 1;
StatusBar1->Panels[0].Items[0]->Text = "Голос: " + UnicodeString(AutoWork::Voice);
StatusBar1->Panels[0].Items[1]->Text = "Звук: " + UnicodeString(AutoWork::Char);
StatusBar1->Panels[0].Items[2]->Text = "Вариант: " + UnicodeString(AutoWork::Variant);
// Двигаемся вперёд
AutoWork::Variant++;
if (AutoWork::Variant >= Form1->ListBox2->Items->Count) {
	AutoWork::Variant = 0;
	AutoWork::Char++;
}
if (AutoWork::Char >= Form1->ListBox1->Items->Count) {
	AutoWork::Char = 0;
	if (CheckBox1->Checked) {
		AutoWork::Voice++;
		if (AutoWork::Voice >= Form1->TabControl1->Tabs->Count) {
			ShowMessage("Ошибка");
			return;
		}
	}
}
}
//---------------------------------------------------------------------------
