//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "../Common/Common.h"
#include "../Common/SpecterWork.h"
#include "../Common/FFT.h"
#include "../Common/FrequencyData.h"
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/strings.h"
#include "../../_Common_/Lib/RunProcess.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define KKK 11
#define N 65536

TForm1 *Form1;
short *Buf = NULL;
int BufSize, Shaper = 0, NN = 65536, SpecterType = 0;
SER Ser[KKK];

//------------------------------------------------------------------------------
bool Convert(UnicodeString SOX, UnicodeString in, UnicodeString out) {
UnicodeString BITS = "16", CHANNELS = "1", FREQ = "48000", command;
command = SOX + " \"" + in + "\" --bits " + BITS
	+ " --channels " + CHANNELS
	+ " --encoding signed-integer --rate " + FREQ
	+ " --type raw \"" + out +"\"";
_wsystem(command.w_str());
//wcout << command.w_str() << endl;
return true;
}

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
for (int i = 0; i < KKK; i++) {
	// Частоты
	Ser[i].S = new TFastLineSeries(this);
	Ser[i].S->Visible = false;
    Ser[i].S->LinePen->Width = 3;
	Chart1->AddSeries(Ser[i].S);
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N4Click(TObject *Sender)
{ // Выход
Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N2Click(TObject *Sender)
{ // Открыть
if (OpenDialog1->Execute()) {
    if (get_extension(OpenDialog1->FileName) == ".dat") {

    }
    else {
	    // Преобразование адио файла (MP3 или WAV) в DAT
		UnicodeString source_fname = OpenDialog1->FileName;
        OpenDialog1->FileName = OpenDialog1->FileName + ".dat";
		// Конвертируем аудио в DAT
		Common Common;
		UnicodeString SOX = Common.GetSOX();
		Convert(SOX, source_fname, OpenDialog1->FileName);
        if (!FileExists(OpenDialog1->FileName)) {
            ShowMessage("Ошибка преобразования в DAT формат!");
            return;
        }
    }
    // Пересоздаём буфер с данными
    if (Buf != NULL) delete[] Buf;
    int fsize = getFileSize(OpenDialog1->FileName);
    BufSize = fsize / 2;
    Buf = new short [BufSize];
    // Читаем данные с диска
    FILE *fd = _wfopen(OpenDialog1->FileName.w_str(), L"rb");
    fread(Buf, 2, BufSize, fd);
    fclose(fd);
    //
    TrackBar1->Position = 0;
    TrackBar1->Max = BufSize - 1;
	StatusBar1->Panels[0].Items[0]->Text = "Элементов: " + UnicodeString(BufSize);
    Action1Execute(this);
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Action1Execute(TObject *Sender)
{ //
    int i, j, k, pos;
    double val;
    // Статусная строка
	StatusBar1->Panels[0].Items[1]->Text = "Позиция: " + UnicodeString(TrackBar1->Position);
    pos = TrackBar1->Position;
    // Берём необходимое кол-во данных
    short *data = new short [N];
    for (i = 0; i < N; i++) {
   	    j = pos + i - N / 2;
        // Выход за пределы массива Buf
       	if ((j < 0) || (j >= BufSize)) data[i] = 0;
        else {
	        k = abs(i - N / 2);
            if (k > NN / 2) data[i] = 0;
            else data[i] = Buf[j];
        }
   	}
    // Делаем анализ
    FrequencyData *FrequencyData = new class FrequencyData(N);
    FrequencyData->applyShape(Shaper);
    FrequencyData->calculate(data, 0);
    // Строим гроафик
    Ser[0].S->Clear();
    Ser[0].S->Visible = true;
    for (i = 0; i < FrequencyData->N2; i++) {
        if (SpecterType == 0) val = FrequencyData->Data[i];
        else {
			//val = val * 8000 * (i + 1) * N2 / (1364 * 4096);
			val = FrequencyData->Data[i] * 8000 * (i + 1) / 10920;
            val = val * val;
        }
        Ser[0].S->AddXY(i, val, "");
    }
    // Удаляем данные
    delete[] data;
    delete FrequencyData;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N14Click(TObject *Sender)
{ // Узкий (16384)
    N14->Checked = true;
    NN = 16384;
    Action1Execute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N15Click(TObject *Sender)
{ // Средний (32768)
    N15->Checked = true;
    NN = 32768;
    Action1Execute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N655361Click(TObject *Sender)
{ // Широкий (65536)
    N655361->Checked = true;
    NN = 65536;
    Action1Execute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N16Click(TObject *Sender)
{ // Прямоугольный
N16->Checked = true;
Shaper = 0;
Action1Execute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N17Click(TObject *Sender)
{ // Трапецевидный
N17->Checked = true;
Shaper = 1;
Action1Execute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N18Click(TObject *Sender)
{ // Нормальный гауссиан
N18->Checked = true;
Shaper = 2;
Action1Execute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N19Click(TObject *Sender)
{ // Узкий гауссиан
N19->Checked = true;
Shaper = 3;
Action1Execute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N20Click(TObject *Sender)
{ // Широкий гауссиан
N20->Checked = true;
Shaper = 4;
Action1Execute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{ // <<
int i = TrackBar1->Position - 32768;
if (i < 0) i = 0;
TrackBar1->Position = i;
Action1Execute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{ // >>
int i = TrackBar1->Position + 32768;
if (i >= TrackBar1->Max) i = TrackBar1->Max - 1;
TrackBar1->Position = i;
Action1Execute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N6Click(TObject *Sender)
{ // Запустить копию
ShellExecute(0, L"open", Application->ExeName.w_str(), NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N8Click(TObject *Sender)
{ // Перезапустить
ShellExecute(0, L"open", Application->ExeName.w_str(), NULL, NULL, SW_SHOWNORMAL);
Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N13Click(TObject *Sender)
{ // Очистить все
for (auto i = 1; i < KKK; i++) {
    Ser[i].S->Clear();
    Ser[i].S->Visible = false;
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N12Click(TObject *Sender)
{ // Добавить 1
int q = 1;
Ser[q].S->Clear();
Ser[q].S->Visible = true;
TChartValueList *y = Ser[0].S->YValues; // получаешь значения по OY
for (auto i = 0; i < y->Count; i++) {
    Ser[q].S->AddXY(i, y->Value[i], "");
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N21Click(TObject *Sender)
{ // Добавить 2
int q = 2;
Ser[q].S->Clear();
Ser[q].S->Visible = true;
TChartValueList *y = Ser[0].S->YValues; // получаешь значения по OY
for (auto i = 0; i < y->Count; i++) {
    Ser[q].S->AddXY(i, y->Value[i], "");
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N31Click(TObject *Sender)
{ // Добавить 3
int q = 3;
Ser[q].S->Clear();
Ser[q].S->Visible = true;
TChartValueList *y = Ser[0].S->YValues; // получаешь значения по OY
for (auto i = 0; i < y->Count; i++) {
    Ser[q].S->AddXY(i, y->Value[i], "");
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N41Click(TObject *Sender)
{ // Добавить 4
int q = 4;
Ser[q].S->Clear();
Ser[q].S->Visible = true;
TChartValueList *y = Ser[0].S->YValues; // получаешь значения по OY
for (auto i = 0; i < y->Count; i++) {
    Ser[q].S->AddXY(i, y->Value[i], "");
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N51Click(TObject *Sender)
{ // Добавить 5
int q = 5;
Ser[q].S->Clear();
Ser[q].S->Visible = true;
TChartValueList *y = Ser[0].S->YValues; // получаешь значения по OY
for (auto i = 0; i < y->Count; i++) {
    Ser[q].S->AddXY(i, y->Value[i], "");
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N23Click(TObject *Sender)
{ // Обычные
N23->Checked = true;
SpecterType = 0;
Action1Execute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N24Click(TObject *Sender)
{ // Энергитические
N24->Checked = true;
SpecterType = 1;
Action1Execute(this);
}
//---------------------------------------------------------------------------
