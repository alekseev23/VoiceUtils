//---------------------------------------------------------------------------

#include <vcl.h>
#include <VCLTee.Series.hpp>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
#include "Unit4.h"
#include "Unit5.h"
#include "Unit6.h"
#include "Unit8.h"
#include "Unit9.h"
#include "Unit10.h"
#include "Unit11.h"
#include "Unit12.h"
#include "bass.h"
#include "../../_Common_/Lib/bitmap.h"
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/RunProcess.h"
#include "../../_Common_/Lib/strings.h"
#include "../Common/Envelope.h"
#include "../Common/FFT.h"
#include "../Common/FrequencyData.h"
#include "../Common/common.h"
#include "../Common/TSpecters.h"
#include "../Common/SpecterWork.h"
#include "../Common/TAudioMax.h"
#include "../Common/TColorSceme.h"
#include "../Common/TWavFile.h"
#include "../Common/TSpecterProps.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
HWND win =  NULL;

#define FREQ 48000
#define CHANNELS 1
#define BUFSTEP 1000000	// memory allocation unit
#define MAX_SER 500
#define CHANGES_N 256
char *recbuf = NULL;		// recording buffer
DWORD reclen;				// recording length
HRECORD rchan = 0;			// recording channel
HSTREAM chan = 0;			// playback channel
HSTREAM fn = 0;
int
	input,					// current input source
	AudioDeviceIndex = -1, 	// Текущее аудиоустройство
	N2 = 256,
	Scale = 1;
UnicodeString CurPath, CurrentFile;
TFastLineSeries *AudioSer;
TFastLineSeries *SpecterSer;
SpecterWork *SpecterWork;
SER Ser[MAX_SER];           // Спектры текущего файла
SER SerChanges[CHANGES_N];           // Спектры текущего файла
SER SerMain[5];             // Основные спектры
TAudioMax *AudioMax;
bool BUSY = false;

//---------------------------------------------------------------------------
void MyLog(UnicodeString message) {
	FILE *fd = _wfopen(UnicodeString(CurPath + "\\my.log").w_str(), L"at");
	fwprintf(fd, L"%ls\n", message.w_str());
	fclose(fd);
}
//---------------------------------------------------------------------------
void ShowError(const char *es) {
	ShowMessage(es);
}
//---------------------------------------------------------------------------
void UpdateInputInfo() {
	char *type;
	float level;
	int it = BASS_RecordGetInput(input, &level); // get info on the input
	if (it == -1 || level < 0) { // failed to get level
		BASS_RecordGetInput(-1, &level); // try master input instead
		if (level < 0) { // that failed too
			level = 1; // just display 100%
		}
	}
	switch (it & BASS_INPUT_TYPE_MASK) {
		case BASS_INPUT_TYPE_DIGITAL: 	type="digital"; break;
		case BASS_INPUT_TYPE_LINE:		type="line-in"; break;
		case BASS_INPUT_TYPE_MIC:       type="microphone"; break;
		case BASS_INPUT_TYPE_SYNTH:		type="midi synth"; break;
		case BASS_INPUT_TYPE_CD:		type="analog cd"; break;
		case BASS_INPUT_TYPE_PHONE:		type="telephone"; break;
		case BASS_INPUT_TYPE_SPEAKER: 	type="pc speaker"; break;
		case BASS_INPUT_TYPE_WAVE:		type="wave/pcm"; break;
		case BASS_INPUT_TYPE_AUX:		type="aux";	break;
		case BASS_INPUT_TYPE_ANALOG:	type="analog"; break;
		default: type="undefined";
	}
}
//---------------------------------------------------------------------------
// buffer the recorded data
BOOL CALLBACK RecordingCallback(HRECORD handle, const void *buffer, DWORD length, void *user) {
	// increase buffer size if needed
	if ((reclen%BUFSTEP)+length>=BUFSTEP) {
		recbuf=(char*)realloc(recbuf,((reclen+length)/BUFSTEP+1)*BUFSTEP);
		if (!recbuf) {
			rchan=0;
			ShowError("Out of memory!");
			return FALSE; // stop recording
		}
	}
	// buffer the data
	memcpy(recbuf+reclen,buffer,length);
	reclen+=length;
	return TRUE; // continue recording
}

//---------------------------------------------------------------------------
void StartRecording()
{
	WAVEFORMATEX *wf;
	if (recbuf) { // free old recording
		BASS_StreamFree(chan);
		chan=0;
		free(recbuf);
		recbuf=NULL;
		// close output device before recording incase of half-duplex device
		BASS_Free();
	}
	// allocate initial buffer and make space for WAVE header
	recbuf=(char*)malloc(BUFSTEP);
	reclen=44;
	// fill the WAVE header
	memcpy(recbuf,"RIFF\0\0\0\0WAVEfmt \20\0\0\0", 20);
	memcpy(recbuf+36, "data\0\0\0\0", 8);
	wf=(WAVEFORMATEX*)(recbuf + 20);
	wf->wFormatTag = 1;
	wf->nChannels = CHANNELS;
	wf->wBitsPerSample = 16;
	wf->nSamplesPerSec = FREQ;
	wf->nBlockAlign = wf->nChannels*wf->wBitsPerSample/8;
	wf->nAvgBytesPerSec = wf->nSamplesPerSec*wf->nBlockAlign;
	// start recording
	rchan=BASS_RecordStart(FREQ,CHANNELS,0,RecordingCallback,0);
	if (!rchan) {
		ShowError("Couldn't start recording");
		free(recbuf);
		recbuf=0;
		return;
	}
}

//---------------------------------------------------------------------------
void StopRecording()
{
	BASS_ChannelStop(rchan);
	rchan = 0;
	// complete the WAVE header
	*(DWORD*)(recbuf + 4) = reclen-8;
	*(DWORD*)(recbuf + 40) = reclen-44;

	// setup output device (using default device)
	if (!BASS_Init(-1,FREQ,0,win,NULL)) {
		ShowError("Can't initialize output device");
		return;
	}
    /*
	// create a stream from the recording
	if (chan=BASS_StreamCreateFile(TRUE,recbuf,0,reclen,0));
	else BASS_Free();
	*/
}

//---------------------------------------------------------------------------
// write the recorded data to disk
void WriteToDisk(UnicodeString filename)
{
	FILE *fp;
	if (!(fp=_wfopen(filename.w_str(),L"wb"))) {
		ShowError("Can't create the file");
		return;
	}
	fwrite(recbuf,reclen,1,fp);
	fclose(fp);
}
//---------------------------------------------------------------------------
BOOL InitDevice(int device) {
	BASS_RecordFree(); // free current device (and recording channel) if there is one
	// initalize new device
	if (!BASS_RecordInit(device)) {
		ShowError("Can't initialize recording device");
		return FALSE;
	}
	// get list of inputs
	int c;
	const char *i;
	for (c = 0; i = BASS_RecordGetInputName(c); c++) {
		if (!(BASS_RecordGetInput(c, NULL) & BASS_INPUT_OFF)) { // this one is currently "on"
			input = c;
			UpdateInputInfo();
		}
	}
	return TRUE;
}

//---------------------------------------------------------------------------
void __fastcall AnalyzeRecord(UnicodeString filename)
{
int i, Items, max, min, max_pos, a, b, val, n;
short *Buf;
Items = (reclen - 44) / 2;
Buf = (short *)&recbuf[44];
// Найти огибающую
auto Envelope = new class Envelope(480, Items / 480);
for (i = 0; i < Items; i++) Envelope->AddAudioPoint(Buf[i]);
// Найти максимум огибающей
max = 0;
min = 32767;
max_pos = 0;
for (i = 0; i < Envelope->Size; i++) {
	if (Envelope->Data[i] > max) {
		max = Envelope->Data[i];
		max_pos = i;
	}
	else if (Envelope->Data[i] < min) {
		min = Envelope->Data[i];
	 }
}
// От максимума ищем начало пакета
for (a = max_pos; a > 0; a--) {
	val = Envelope->Data[a];
	if (100 * (val - min) < 5 * (max - min)) break;
}
// От максимума ищем конец пакета
for (b = max_pos; b < Envelope->Size; b++) {
	val = Envelope->Data[b];
	if (100 * (val - min) < 5 * max) break;
}
// Вырезаем пакет
a *= 480;
b *= 480;
if (b > Items) b = Items;
// Переносим данные в начало массива
n = b - a;
/*ShowMessage("min=" + UnicodeString(min) + " max=" + UnicodeString(max)
	+ " a=" + UnicodeString(a) + " b=" + UnicodeString(b)
	+ " n=" + UnicodeString(n) + " Items=" + UnicodeString(Items));*/
for (i = 0; i < n; i++) {
	// Делаем нормировку
	Buf[i] = (short) (32767 * Buf[i + a] / max);
}
// Сохраняем его в WAV файле
reclen = 44 + n * 2;
*(DWORD*)(recbuf + 4) = reclen - 8;
*(DWORD*)(recbuf + 40) = reclen - 44;
FILE *fp;
if (!(fp = _wfopen(filename.w_str(), L"wb"))) {
	ShowError("Can't create the file");
	return;
}
fwrite(recbuf, reclen, 1, fp);
fclose(fp);
// Удаляем ненужные объекты
delete Envelope;
}
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
//
randomize();
// Текущий каталог
CurPath = ExtractFileDir(Application->ExeName);
CurrentFile = CurPath + "\\current.wav";
//
if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
	ShowMessage("An incorrect version of BASS.DLL was loaded");
	return;
}
// инициализация звука
if (!BASS_Init(-1, FREQ, 0, win, NULL)) {
	// если не удалось инициализировать звук
	BASS_Init(-2, FREQ, 0, win, NULL);
}
// звук на выход, без этого звука не услышите
BASS_Start();
// Рабочие классы
SpecterWork = new class SpecterWork;
AudioMax = new class TAudioMax;
// Графики
AudioSer = new TFastLineSeries(this);
Chart1->AddSeries(AudioSer);
AudioSer->Color = (TColor)(200 << 16);

for (int i = 0; i < MAX_SER; i++) {
	// Частоты
	Ser[i].S = new TFastLineSeries(this);
	Ser[i].S->Visible = false;
	Chart2->AddSeries(Ser[i].S);
}

for (int i = 0; i < CHANGES_N; i++) {
	// Частоты
	SerChanges[i].S = new TFastLineSeries(this);
	//SerChanges[i].S->Color = (TColor)((random(255) << 16) + (random(255) << 8) + random(255));
	Chart3->AddSeries(SerChanges[i].S);
}

for (int i = 0; i < SpecterWork->MainCount; i++) {
	// Частоты
	SerMain[i].S = new TFastLineSeries(this);
	Chart4->AddSeries(SerMain[i].S);
}

SpecterSer = new TFastLineSeries(this);
Chart2->AddSeries(SpecterSer);
SpecterSer->Color = (TColor)(200);
SpecterSer->LinePen->Width = 3;
// get list of recording devices
UpdateAudioInputListExecute(this);
//
UpdateVoicesListExecute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N3Click(TObject *Sender)
{
Form2->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UpdateVoicesListExecute(TObject *Sender)
{
UnicodeString s;
TStringList *ts = new TStringList;
s = CurPath + "\\voices";
if (!DirectoryExists(s)) MkDir(s);
// Получить список подкаталогов в каталоге
GetDirsFromDirectory(s, false, ts);
for (int i = 0; i < ts->Count; i++) ts->Strings[i] = get_file_name(ts->Strings[i]);
TabControl1->Tabs->Text = ts->Text;
delete ts;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N4Click(TObject *Sender)
{ // Удалить голос
if (MessageBox(NULL, L"Желаете удалить текущий голос?", L"Требуется подтверждение", MB_YESNO) == IDYES) {
	UnicodeString dirname = CurPath + "\\voices\\" + TabControl1->Tabs->Strings[TabControl1->TabIndex];
	if (!DirectoryExists(dirname)) {
		ShowMessage("Каталог не найден!");
	}
	else {
		// Сперва удаляем все файлы в каталоге

		// Потом сам каталог
		RemoveDir(dirname);
		UpdateVoicesListExecute(this);
	}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N5Click(TObject *Sender)
{ // Удалить все голоса
if (MessageBox(NULL, L"Желаете удалить все голоса?", L"Требуется подтверждение", MB_YESNO) == IDYES) {
	// Получаем список каталогов

	// Цикл по каталогам

	// Удаляем все файлы

	// Удаляем каталог
	//RemoveDir(dirname);

	// Обновляем список
	UpdateVoicesListExecute(this);
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn1Click(TObject *Sender)
{
if (BitBtn1->Caption == "Запись") {
	StartRecording();
	BitBtn1->Caption = "Стоп";
}
else if (BitBtn1->Caption == "Стоп") {
	if (rchan) {
		StopRecording();
		// Находим имя файла
		if (ListBox1->ItemIndex < 0) ListBox1->ItemIndex = 0;
		UnicodeString filename, dirname, name;
		int max = 0, val;
		for (int i = 0; i < ListBox2->Items->Count; i++) {
			val = ListBox2->Items->Strings[i].ToIntDef(0);
			if (val > max)  max = val;
		}
		max++;
		name = ListBox1->Items->Strings[ListBox1->ItemIndex];
		dirname = CurPath + "\\voices\\" + TabControl1->Tabs->Strings[TabControl1->TabIndex];
		filename = dirname + "\\" + name + "_" + UnicodeString(max) + ".wav";
		AnalyzeRecord(filename);
		UpdateRecordsListExecute(this);
		if (ListBox2->Items->Count > 0) ListBox2->ItemIndex = ListBox2->Items->Count - 1;
		ListBox2Click(this);
	}
	BitBtn1->Caption = "Запись";
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UpdateAudioInputListExecute(TObject *Sender)
{ // Обновляем список аудио устройств
	int c, def;
	BASS_DEVICEINFO di;
	ComboBox1->Clear();
	//ComboBox1->Items->Add("Аудио микшер (системные звуки)");
	for (c = 0; BASS_RecordGetDeviceInfo(c, &di); c++) {
		ComboBox1->Items->Add(di.name);
		if (di.flags & BASS_DEVICE_DEFAULT) { // got the default device
			def = c;
		}
	}
	InitDevice(def); // initialize default recording device
	ComboBox1->ItemIndex = 0;
	ComboBox1Change(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ComboBox1Change(TObject *Sender)
{
// Останавливаем запись если ведётся
if (BitBtn1->Caption == "Стоп") BitBtn1Click(this);
//
AudioDeviceIndex = ComboBox1->ItemIndex;
// initialize the selected device
if (InitDevice(AudioDeviceIndex)) {
	// start recording
//ShowMessage("Point 3");
	/*rchan = BASS_RecordStart(FREQ, CHANNELS, 0, RecordingCallback, 0);
	if (!rchan) {
		ShowError("Couldn't start recording");
		return;
	}*/
}
//ShowMessage("Point 4");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UpdateRecordsListExecute(TObject *Sender)
{
UnicodeString filename, dirname, name;
TStringList *ts = new TStringList;
if (ListBox1->ItemIndex < 0) ListBox1->ItemIndex = 0;
name = ListBox1->Items->Strings[ListBox1->ItemIndex];
dirname = CurPath + "\\voices\\" + TabControl1->Tabs->Strings[TabControl1->TabIndex];
GetFilesFromDirectory(dirname, name + "*.wav", false, false, ts);
ListBox2->Clear();
for (int i = 0; i < ts->Count; i++) {
	name = getFieldFromString(1, without_extension(get_file_name(ts->Strings[i])), "_");
	ListBox2->Items->Add(name);
}
delete ts;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListBox1Click(TObject *Sender)
{ // Читаем список имён файлов
UpdateRecordsListExecute(this);
if (ListBox2->Items->Count > 0) {
	ListBox2->ItemIndex = 0;
	ListBox2Click(this);
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListBox2Click(TObject *Sender)
{
if (ListBox2->Items->Count == 0) return;
if (ListBox1->ItemIndex < 0) ListBox1->ItemIndex = 0;
if (ListBox2->ItemIndex < 0) ListBox2->ItemIndex = 0;
// Определяем имя файла
UnicodeString filename;
filename = CurPath + "\\voices\\" + TabControl1->Tabs->Strings[TabControl1->TabIndex]
	+ "\\" + ListBox1->Items->Strings[ListBox1->ItemIndex] + "_"
	+ ListBox2->Items->Strings[ListBox2->ItemIndex] + ".wav";
// Грузим данные из DAT файла
if (!FileExists(filename)) {
	ShowMessage("Файл не найден: " + filename);
	return;
}
//
if (Form9->Showing) {
	//TWavFile *WavFile = new TWavFile;
    TWavFile *WavFile = new TWavFile(2 * N2);
	WavFile->Load(filename);
	WavFile->Calculate();
	int left, right, d_left, d_right;
	double kf, kb;
	left = Form9->Edit1->Text.ToIntDef(0);
	d_left = Form9->Edit2->Text.ToIntDef(0);
	right = Form9->Edit3->Text.ToIntDef(0);
	d_right = Form9->Edit4->Text.ToIntDef(0);
	kf = str_replace(".", ",", Form9->Edit5->Text).ToDouble();
	kb = str_replace(".", ",", Form9->Edit6->Text).ToDouble();
    // Обрезка
	WavFile->Cut(left, right, d_left, d_right, kf, kb);
    // Сдвиг
	WavFile->Shift(Form9->Edit7->Text.ToIntDef(0));
    // Копирование
	WavFile->Copy(Form9->Edit10->Text.ToIntDef(0), Form9->Edit9->Text.ToIntDef(0),
    	str_replace(".", ",", Form9->Edit8->Text).ToDouble(), Form9->Edit11->Text.ToIntDef(0));
    // Растяжение / Сжатие
	WavFile->Resize(str_replace(".", ",", Form9->Edit12->Text).ToDouble());

	StatusBar1->Panels[0].Items[0]->Text = UnicodeString(left) + " " + UnicodeString(d_left) + " " +  UnicodeString(right) + " " +  UnicodeString(d_right) + " " +  UnicodeString(kf) + " " + UnicodeString(kb);
	WavFile->ReCalculate();
	WavFile->Save(CurrentFile);
	delete WavFile;
}
else CopyFile(filename.w_str(), CurrentFile.w_str(), false);
SpecterWork->ReadData(CurrentFile);
SpecterWork->CountSpecters(N2 * 2, N20->Checked);
// Проиграть
N18Click(this);
// Строим график
AudioSer->Clear();
for (auto i = 0; i < SpecterWork->BufSize; i++) {
	AudioSer->AddXY(i, SpecterWork->Buf[i], "");
}
TabControl2Change(this);
// Строим анаиз максимумов
AudioMax->Set(SpecterWork->Buf, SpecterWork->BufSize);
AudioMax->Draw(Form5->Image1->Picture->Bitmap);
AudioMax->DrawMiddleValues(Form5->Image2->Picture->Bitmap);
Form5->Memo1->Text = AudioMax->Print();
Form5->Memo2->Text = AudioMax->PrintMiddleValues();
//
Chart1->Title->Text->Clear();
Chart1->Title->Text->Add("Аудио данные звука [" + ListBox1->Items->Strings[ListBox1->ItemIndex]
	+ "] голоса [" + TabControl1->Tabs->Strings[TabControl1->TabIndex] + "]"
	+ " версия [" + ListBox2->Items->Strings[ListBox2->ItemIndex] + "]");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BitBtn2Click(TObject *Sender)
{ // Удалить
if (ListBox2->Items->Count == 0) return;
if (ListBox1->ItemIndex < 0) ListBox1->ItemIndex = 0;
if (ListBox2->ItemIndex < 0) ListBox2->ItemIndex = 0;
// Определяем имя файла
UnicodeString filename;
filename = CurPath + "\\voices\\" + TabControl1->Tabs->Strings[TabControl1->TabIndex]
	+ "\\" + ListBox1->Items->Strings[ListBox1->ItemIndex] + "_"
	+ ListBox2->Items->Strings[ListBox2->ItemIndex] + ".wav";
DeleteFile(filename.w_str());
filename = str_replace(".wav", ".dat", filename);
DeleteFile(filename.w_str());
//ListBox1Click(this);
UpdateRecordsListExecute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N18Click(TObject *Sender)
{ // Проиграть повторно
if (!FileExists(CurrentFile)) return;
// инициализация звука
if (!BASS_Init(-1, FREQ, 0, win, NULL)) {
	// если не удалось инициализировать звук
	BASS_Init(-2, FREQ, 0, win, NULL);
}
// звук на выход, без этого звука не услышите
BASS_Start();
// Создаём поток
chan = BASS_StreamCreateFile(false, CurrentFile.w_str(), 0, 0, BASS_UNICODE);
// играем поток
BASS_ChannelPlay(chan, false);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BitBtn3Click(TObject *Sender)
{ // Удалить все
if (ListBox2->Items->Count == 0) return;
if (ListBox1->ItemIndex < 0) ListBox1->ItemIndex = 0;
// Определяем имя файла
UnicodeString filename;
for (auto i = 0; i < ListBox2->Items->Count; i++) {
	filename = CurPath + "\\voices\\" + TabControl1->Tabs->Strings[TabControl1->TabIndex]
		+ "\\" + ListBox1->Items->Strings[ListBox1->ItemIndex] + "_"
		+ ListBox2->Items->Strings[i] + ".wav";
	DeleteFile(filename.w_str());
	filename = str_replace(".wav", ".dat", filename);
	DeleteFile(filename.w_str());
}
//ListBox1Click(this);
UpdateRecordsListExecute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N2561Click(TObject *Sender)
{
N2 = 256;
N2561->Checked = true;
SpecterWork->CountSpecters(N2 * 2);
ListBox2Click(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N2562Click(TObject *Sender)
{
N2 = 512;
N2562->Checked = true;
SpecterWork->CountSpecters(N2 * 2);
ListBox2Click(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N10241Click(TObject *Sender)
{
N2 = 1024;
N10241->Checked = true;
SpecterWork->CountSpecters(N2 * 2);
ListBox2Click(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N10242Click(TObject *Sender)
{
N2 = 2048;
N10242->Checked = true;
SpecterWork->CountSpecters(N2 * 2);
ListBox2Click(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N40961Click(TObject *Sender)
{
N2 = 4096;
N40961->Checked = true;
SpecterWork->CountSpecters(N2 * 2);
ListBox2Click(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TabControl2Change(TObject *Sender)
{ //
BUSY = true;
double val, val2;
int best;
// Графики Q-спектров
if (Form11->Showing) Form11->Action1Execute(this);

// Прячем все спектры
for (auto i = 0; i < MAX_SER; i++) Ser[i].S->Visible = false;
best = SpecterWork->FindBest();
// Рисуем спектры
for (auto j = 0; (j < SpecterWork->Count) && (j < MAX_SER); j++) {
	Ser[j].S->Clear();
	Ser[j].S->Visible = true;
	Ser[j].S->Title = UnicodeString(j);
	if (j == best) {
		Ser[j].S->LinePen->Width = 3;
		Ser[j].S->Color = (TColor)(((255 * j / SpecterWork->Count) << 16) + ((255 - 255 * j / SpecterWork->Count) << 8));
	}
	else {
		Ser[j].S->LinePen->Width = 1;
		Ser[j].S->Color = (TColor)(((255 * j / SpecterWork->Count) << 16) + ((255 - 255 * j / SpecterWork->Count) << 8));
	}
	int jj;
	if (SpecterWork->Count < MAX_SER) jj = j;
	else jj = j * (SpecterWork->Count - 1) / (MAX_SER - 1);
	for (auto i = 0; i < SpecterWork->N2; i++) {
		val = SpecterWork->GetSpecterPointValue(jj, i, Scale);
		Ser[j].S->AddXY(i, val, "");
	}
}
//SpecterWork->GetMiddle(TabControl2->TabIndex, Scale);
SpecterWork->GetMiddle(0, Scale);
// Строим график
SpecterSer->Clear();
for (auto i = 0; i < SpecterWork->N2; i++) {
	//val = SpecterWork->GetSpecterValue(SpecterWork->Middle[i], Scale);
	val = SpecterWork->Middle[i];
	SpecterSer->AddXY(i, val, "");
}
// Центр тяжести усреднённого спектра
TSpecterProps *SpecterProps = new TSpecterProps;
int a = 0, b;
switch (SpecterWork->N2) {
    case  256: b = 20; break;
    case  512: b = 40; break;
    case 1024: b = 80; break;
    case 2048: b = 160; break;
    case 4096: b = 320; break;
}
SpecterProps->Analyze(a, b, SpecterWork->Middle);
StatusBar1->Panels[0].Items[1]->Text = SpecterProps->Middle;
delete SpecterProps;

//SpecterWork->DrawSpectrograms(Image3->Picture->Bitmap, Scale);
SpecterWork->DrawSpectrograms2(Image3->Picture->Bitmap, Scale);
//SpecterWork->DrawSpectrograms3(Image3->Picture->Bitmap, Scale);
/*
// Сохраняем изображение
UnicodeString filename = CurPath + "\\voices\\" + TabControl1->Tabs->Strings[TabControl1->TabIndex]
	+ "\\" + ListBox1->Items->Strings[ListBox1->ItemIndex] + "_" + ListBox2->Items->Strings[ListBox2->ItemIndex] + ".png";
save_bitmap(Image3->Picture->Bitmap, filename, false, 100);
*/
//
//Image3->Width = Image3->Picture->Bitmap->Width;
//Image3->Height = Image3->Picture->Bitmap->Height;
//Image3->Height = 3 * Image3->Picture->Bitmap->Height;
Form10->ApplyExecute(this);
//SpecterWork->DrawSpectrograms3D(Image2->Picture->Bitmap, Scale);
Image2->Width = Image2->Picture->Bitmap->Width;
Image2->Height = Image2->Picture->Bitmap->Height;
if (!Form3) return;
// Строим графики изменений
for (auto j = 0; j < CHANGES_N; j++) {
	SerChanges[j].S->Clear();
	if (Form3->ListBox1->Selected[j]) SerChanges[j].S->Visible = true;
	else {
		SerChanges[j].S->Visible = false;
		continue;
	}
	SerChanges[j].S->Title = UnicodeString(j);
	SerChanges[j].S->LinePen->Width = 1;
	//SerChanges[j].S->Color = (TColor)(((255 * j / SpecterWork->Count) << 16) + ((255 - 255 * j / SpecterWork->Count) << 8));
	for (auto i = 0; i < SpecterWork->Count; i++) {
		val = SpecterWork->GetSpecterPointValue(i, j + 2, Scale);
		if (i > 0) SerChanges[j].S->AddXY(i, (val - val2), "");
		val2 = val;
	}
}
// Строим основные графики
for (auto j = 0; j < SpecterWork->MainCount; j++) {
	SerMain[j].S->Clear();
	SerMain[j].S->Title = UnicodeString(j);
	SerMain[j].S->LinePen->Width = 3;
	//SerMain[j].S->Color = (TColor)(((255 * j / SpecterWork->Count) << 16) + ((255 - 255 * j / SpecterWork->Count) << 8));
	for (auto i = 0; i < SpecterWork->N2; i++) {
		val = SpecterWork->GetSpecterValue(SpecterWork->Main[j * SpecterWork->N2 + i], Scale);
		SerMain[j].S->AddXY(i, val, "");
	}
}
// Подписи к графикам
Chart2->Title->Text->Clear();
Chart2->Title->Text->Add("Все спектры звука [" + ListBox1->Items->Strings[ListBox1->ItemIndex]
	+ "] голоса [" + TabControl1->Tabs->Strings[TabControl1->TabIndex] + "]"
	+ " версия [" + ListBox2->Items->Strings[ListBox2->ItemIndex] + "]");
Chart4->Title->Text->Clear();
Chart4->Title->Text->Add("Основные спектры звука [" + ListBox1->Items->Strings[ListBox1->ItemIndex]
	+ "] голоса [" + TabControl1->Tabs->Strings[TabControl1->TabIndex] + "]"
	+ " версия [" + ListBox2->Items->Strings[ListBox2->ItemIndex] + "]");

if (Form12->Showing) Form12->DrawExecute(this);

BUSY = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N11Click(TObject *Sender)
{ // Выход
Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N9Click(TObject *Sender)
{ // Шкала - Линейная
Scale = 1;
N9->Checked = true;
ListBox2Click(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N10Click(TObject *Sender)
{ // Шкала - Десятичный логарифм
Scale = 2;
N10->Checked = true;
ListBox2Click(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::X1Click(TObject *Sender)
{  // Шкала - Логарифм по основанию X
Scale = 3;
X1->Checked = true;
ListBox2Click(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N12Click(TObject *Sender)
{
Form3->Show();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BitBtn4Click(TObject *Sender)
{ // Построить все
Form4->DrawAllExecute(this);
Form4->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N28Click(TObject *Sender)
{
Form5->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
Timer1->Enabled = false;
ListBox1Click(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn5Click(TObject *Sender)
{
Form6->Show();
Form6->AnalyzeExecute(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N13Click(TObject *Sender)
{ // Запустить копию
ShellExecute(0,L"open",Application->ExeName.w_str(),NULL,NULL,SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N15Click(TObject *Sender)
{ // Перезапустить
ShellExecute(0,L"open",Application->ExeName.w_str(),NULL,NULL,SW_SHOWNORMAL);
Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N16Click(TObject *Sender)
{ // Обход всех вариантов
Form8->Show();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N17Click(TObject *Sender)
{ // Обработка спектров
Form9->Show();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N19Click(TObject *Sender)
{ // Обычные спектры
N19->Checked = true;
ListBox2Click(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N20Click(TObject *Sender)
{ // Энергетические спектры
N20->Checked = true;
ListBox2Click(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N21Click(TObject *Sender)
{ // Скриншоты - Аудиоданные
// Проверка каталога
UnicodeString dir_name = CurPath + "\\screenshots";
if (!DirectoryExists(dir_name)) CreateDir(dir_name);
//
Chart1->Refresh();
TBitmap *bmp = new TBitmap;
bmp->PixelFormat = pf24bit;
bmp->SetSize(Chart1->Width, Chart1->Height);
BitBlt(bmp->Canvas->Handle, 0, 0, Form1->Chart1->Width, Form1->Chart1->Height, GetDC(Form1->Chart1->Handle), 0, 0, SRCCOPY);
UnicodeString file_name = CurPath + "\\screenshots\\audio_data_"
	+ TabControl1->Tabs->Strings[TabControl1->TabIndex] + "_" + ListBox1->Items->Strings[ListBox1->ItemIndex]
	+ "_" + ListBox2->Items->Strings[ListBox2->ItemIndex] + ".png";
save_bitmap(bmp, file_name, false, 9);
delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N243Click(TObject *Sender)
{ // Скриншоты - Все спектры данного звука
// Проверка каталога
UnicodeString dir_name = CurPath + "\\screenshots";
if (!DirectoryExists(dir_name)) CreateDir(dir_name);
//
if (!Form4->Showing) BitBtn4Click(this);
Form4->Chart1->Refresh();
TBitmap *bmp = new TBitmap;
bmp->PixelFormat = pf24bit;
bmp->SetSize(Form4->Chart1->Width, Form4->Chart1->Height);
BitBlt(bmp->Canvas->Handle, 0, 0, Form4->Chart1->Width, Form4->Chart1->Height, GetDC(Form4->Chart1->Handle), 0, 0, SRCCOPY);
UnicodeString file_name = CurPath + "\\screenshots\\all_specters_"
	+ TabControl1->Tabs->Strings[TabControl1->TabIndex] + "_" + ListBox1->Items->Strings[ListBox1->ItemIndex] + ".png";
save_bitmap(bmp, file_name, false, 9);
delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N255Click(TObject *Sender)
{ // Скриншоты - Все спектры
// Проверка каталога
UnicodeString dir_name = CurPath + "\\screenshots";
if (!DirectoryExists(dir_name)) CreateDir(dir_name);
//
PageControl2->ActivePageIndex = 0;
Chart2->Refresh();
TBitmap *bmp = new TBitmap;
bmp->PixelFormat = pf24bit;
bmp->SetSize(Form1->Chart2->Width, Form1->Chart2->Height);
BitBlt(bmp->Canvas->Handle, 0, 0, Form1->Chart2->Width, Form1->Chart2->Height, GetDC(Form1->Chart2->Handle), 0, 0, SRCCOPY);
UnicodeString file_name = CurPath + "\\screenshots\\specters_"
	+ TabControl1->Tabs->Strings[TabControl1->TabIndex] + "_" + ListBox1->Items->Strings[ListBox1->ItemIndex]
	+ "_" + ListBox2->Items->Strings[ListBox2->ItemIndex] + ".png";
save_bitmap(bmp, file_name, false, 9);
delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N30Click(TObject *Sender)
{ // Скриншоты - Средние спектры
// Проверка каталога
UnicodeString dir_name = CurPath + "\\screenshots";
if (!DirectoryExists(dir_name)) CreateDir(dir_name);
//
PageControl2->ActivePageIndex = 1;
Chart4->Refresh();
TBitmap *bmp = new TBitmap;
bmp->PixelFormat = pf24bit;
bmp->SetSize(Form1->Chart4->Width, Form1->Chart4->Height);
BitBlt(bmp->Canvas->Handle, 0, 0, Form1->Chart4->Width, Form1->Chart4->Height, GetDC(Form1->Chart4->Handle), 0, 0, SRCCOPY);
UnicodeString file_name = CurPath + "\\screenshots\\middle_specters_"
	+ TabControl1->Tabs->Strings[TabControl1->TabIndex] + "_" + ListBox1->Items->Strings[ListBox1->ItemIndex]
	+ "_" + ListBox2->Items->Strings[ListBox2->ItemIndex] + ".png";
save_bitmap(bmp, file_name, false, 9);
delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N3D1Click(TObject *Sender)
{ // Скриншоты - 3D
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N31Click(TObject *Sender)
{ // Скриншоты - Спектрограмма
// Проверка каталога
UnicodeString dir_name = CurPath + "\\screenshots";
if (!DirectoryExists(dir_name)) CreateDir(dir_name);
//
UnicodeString file_name = CurPath + "\\screenshots\\spectrogram_"
	+ TabControl1->Tabs->Strings[TabControl1->TabIndex] + "_" + ListBox1->Items->Strings[ListBox1->ItemIndex]
	+ "_" + ListBox2->Items->Strings[ListBox2->ItemIndex] + ".png";
save_bitmap(Image3->Picture->Bitmap, file_name, false, 9);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::N32Click(TObject *Sender)
{ // Размер спектрограммы
Form10->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TabControl1Change(TObject *Sender)
{
ListBox1Click(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Q1Click(TObject *Sender)
{ // Графики Q-спектров
Form11->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N288Click(TObject *Sender)
{ // Значения нижних спектров
Form12->Show();
}
//---------------------------------------------------------------------------

