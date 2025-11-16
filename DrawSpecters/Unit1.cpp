//---------------------------------------------------------------------------

#include <vcl.h>
#include <VCLTee.Series.hpp>
#pragma hdrstop

#include "Unit1.h"
#include "../Common/SpecterWork.h"
#include "../Common/FFT.h"
#include "../Common/FrequencyData.h"
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/strings.h"
#include "../../_Common_/Lib/RunProcess.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
short *Buf = NULL;
int BufSize, Shaper = 0, Diapazon = 1;
TFastLineSeries *Ser1, *Ser2;;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
Ser1 = new TFastLineSeries(this);
Chart1->AddSeries(Ser1);
Ser2 = new TFastLineSeries(this);
Chart2->AddSeries(Ser2);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DAT1Click(TObject *Sender)
{
if (OpenDialog1->Execute()) {
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
{
    int i, j, n = 512, nn = 512, pos;
    FrequencyData *FrequencyData;
    short *data;
    // Статусная строка
	StatusBar1->Panels[0].Items[1]->Text = "Позиция: " + UnicodeString(TrackBar1->Position);
    // Берём необходимое кол-во данных
    data = new short [n];
    pos = TrackBar1->Position;
    for (i = 0; i < n; i++) {
        j = i - n / 2 + pos;
        if ((j < 0) || (j >= BufSize)) data[i] = 0;
        else data[i] = Buf[j];
    }
    // Делаем анализ
    FrequencyData = new class FrequencyData(n);
    FrequencyData->applyShape(Shaper);
    FrequencyData->calculate(data, 0);
    // Строим гроафик
    Ser1->Clear();
    for (i = 0; i < FrequencyData->N / 2; i++) {
        Ser1->AddXY(i, FrequencyData->Data[i], "");
    }
    // Удаляем данные
    delete[] data;
    delete FrequencyData;
    // Берём необходимое кол-во данных
    n = 8192;
    data = new short [n];
    if (Diapazon == 1) {
	    for (i = 0; i < n; i++) {
    	    j = i - n / 2 + pos;
            // Выход за пределы массива Buf
        	if ((j < 0) || (j >= BufSize)) data[i] = 0;
	        else data[i] = Buf[j];
    	}
    }
    else if (Diapazon == 2) {
	    for (i = 0; i < n; i++) {
    	    j = i - n / 2 + pos;
            // Выход за пределы массива Buf
        	if ((j < 0) || (j >= BufSize)) data[i] = 0;
            // Выход за узкую границу
            else if ((i < n / 2 - nn / 2) || (i > n / 2 + nn / 2)) data[i] = 0;
	        else data[i] = Buf[j];
    	}
    }
    // Делаем анализ
    FrequencyData = new class FrequencyData(n);
    FrequencyData->applyShape(Shaper);
    FrequencyData->calculate(data, 0);
    // Строим гроафик
    Ser2->Clear();
    for (i = 0; i < FrequencyData->N / 2; i++) {
        Ser2->AddXY(i, FrequencyData->Data[i], "");
    }
    // Удаляем данные
    delete[] data;
    delete FrequencyData;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N3Click(TObject *Sender)
{ // Прямоугольный
N3->Checked = true;
Shaper = 0;
Action1Execute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N4Click(TObject *Sender)
{ // Трапецевидный
N4->Checked = true;
Shaper = 1;
Action1Execute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N5Click(TObject *Sender)
{ // Нормальный гауссиан
N5->Checked = true;
Shaper = 2;
Action1Execute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N6Click(TObject *Sender)
{ // Узкий гауссиан
N6->Checked = true;
Shaper = 3;
Action1Execute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N7Click(TObject *Sender)
{ // Широкий гауссиан
N7->Checked = true;
Shaper = 4;
Action1Execute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N9Click(TObject *Sender)
{ // Диапазон - Широкий
N9->Checked = true;
Diapazon = 1;
Action1Execute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N10Click(TObject *Sender)
{ // Диапазон - Узкий
N10->Checked = true;
Diapazon = 2;
Action1Execute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N12Click(TObject *Sender)
{ // Запустить копию
ShellExecute(0,L"open",Application->ExeName.w_str(),NULL,NULL,SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N14Click(TObject *Sender)
{ // Перезапустить
ShellExecute(0,L"open",Application->ExeName.w_str(),NULL,NULL,SW_SHOWNORMAL);
Application->Terminate();
}
//---------------------------------------------------------------------------
