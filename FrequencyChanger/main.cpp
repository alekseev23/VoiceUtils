#include <vcl.h>
#include <tchar.h>
#include "../../_Common_/Lib/TCommandLineArgumentsAnalyzer.h"
#include "../../_Common_/Lib/bitmap.h"
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/RunProcess.h"
#include "../../_Common_/Lib/strings.h"
#include "../Common/FFT.h"
#include "../Common/FrequencyData.h"
#include "../Common/common.h"
#include "../Common/TSpecters.h"
#include "../Common/SpecterWork.h"
#include "../Common/TAudioMax.h"
#include "../Common/TColorSceme.h"
#include "../Common/TWavFile.h"

using namespace std;

//---------------------------------------------------------------------------
void Help(void) {
    std::cout << "Usage: FrequencyChanger.exe " << std::endl
    	<< "\t--time-begin time_begin" << std::endl
        << "\t--time-end time_end" << std::endl
        << "\t-len time_period" << std::endl
    	<< "\t-N specter_size" << std::endl
        << "\t--frequency-cut left right foreground_amplitude background_amplitude" << std::endl
        << "\t--frequency-shift shift_value" << std::endl
    	<< "\t--frequency-copy left right shift amplitude" << std::endl
        << "\t--frequency-stretch strech_value" << std::endl
        << "\t--in audio_file" << std::endl
        << "\t--out audio_file" << std::endl
    	<< "Example: FrequencyChanger.exe --time-begin 0 --time-end 0 -N 16384 --frequency-cut 0 20 1.0 0.0 --in 1.mp3 -out 2.wav" << std::endl
    	<< "Example: FrequencyChanger.exe --time-begin 12.5 --time-len 5.5 -N 1024 --frequency-shift -5 --in sample.dat --out result.dat" << std::endl
    	<< "Example: FrequencyChanger.exe --time-begin 12.5 --time-len 5.5 -N 1024 --frequency-copy 0 40 60 0.8 --in sample.dat --out result.wav" << std::endl
    	<< "Example: FrequencyChanger.exe --time-begin 12.5 --time-len 5.5 -N 1024 --frequency-stretch 0.5 --in in.wav --out out.wav" << std::endl;
}
//------------------------------------------------------------------------------
/*
Преобразует из одного аудио формата в другой (версия от 30.11.2025)
*/
bool Convert(UnicodeString SOX, UnicodeString in, UnicodeString out) {
UnicodeString BITS = "16", CHANNELS = "1", FREQ = "48000", command;
// Если in - DAT-файл
if (get_extension(in) == ".dat") {
	command = SOX + " --bits " + BITS
		+ " --channels " + CHANNELS
		+ " --encoding signed-integer --rate " + FREQ
		+ " --type raw \"" + in +"\" ";

}
else {
    command = SOX + " \"" + in + "\" ";
}
if (get_extension(out) == ".dat") {
	command = command + "--bits " + BITS
		+ " --channels " + CHANNELS
		+ " --encoding signed-integer --rate " + FREQ
		+ " --type raw \"" + out +"\"";

}
else {
    command = command + "\"" + out + "\" ";
}
/*command = SOX + " \"" + in + "\" --bits " + BITS
	+ " --channels " + CHANNELS
	+ " --encoding signed-integer --rate " + FREQ
	+ " --type raw \"" + out +"\"";*/
_wsystem(command.w_str());
//wcout << command.w_str() << endl;
return true;
}
//---------------------------------------------------------------------------

int _tmain(int argc, _TCHAR* argv[])
{
UnicodeString InFile, OutFile;
int N, N2, time_begin, time_end;
double t1, t2, tlen;
// Ищем основные ключи
TCommandLineArgumentsAnalyzer *CommandLineArgumentsAnalyzer = new TCommandLineArgumentsAnalyzer;
CommandLineArgumentsAnalyzer->AddKey("--time-begin", false, 1, 1);
CommandLineArgumentsAnalyzer->AddKey("--time-end", false, 1, 1);
CommandLineArgumentsAnalyzer->AddKey("--time-len", false, 1, 1);
CommandLineArgumentsAnalyzer->AddKey("-N", false, 1, 1);
CommandLineArgumentsAnalyzer->AddKey("--frequency-cut", true, 4, 4);
CommandLineArgumentsAnalyzer->AddKey("--frequency-shift", true, 1, 1);
CommandLineArgumentsAnalyzer->AddKey("--frequency-copy", true, 4, 4);
CommandLineArgumentsAnalyzer->AddKey("--frequency-stretch", true, 1, 1);
CommandLineArgumentsAnalyzer->AddKey("--in", true, 1, 1);
CommandLineArgumentsAnalyzer->AddKey("--out", true, 1, 1);
CommandLineArgumentsAnalyzer->AddArguments(argc, argv, 1, argc);

/*for (auto i = 0; i < CommandLineArgumentsAnalyzer->Count; i++) {
    std::wcout << CommandLineArgumentsAnalyzer->Keys[i]->Key
        <<  "\t" << CommandLineArgumentsAnalyzer->Keys[i]->Present
        <<  "\t" << CommandLineArgumentsAnalyzer->Keys[i]->Params->Text
    	<< std::endl;
}*/
// Анализ
// Аудио файл
if (CommandLineArgumentsAnalyzer->Check("--in") && CommandLineArgumentsAnalyzer->CheckRequiredParams("--in")) {
    InFile = CommandLineArgumentsAnalyzer->GetParameter("--in", 0);
}
else {
    Help();
    return 1;
}
// Выходное изображение
if ((CommandLineArgumentsAnalyzer->Check("--out") && CommandLineArgumentsAnalyzer->CheckRequiredParams("--out"))) {
    OutFile = CommandLineArgumentsAnalyzer->GetParameter("--out", 0);
}
else {
    Help();
    return 1;
}
// N
if ((CommandLineArgumentsAnalyzer->Check("-N") && CommandLineArgumentsAnalyzer->CheckRequiredParams("-N"))) {
    N = CommandLineArgumentsAnalyzer->GetParameter("-N", 0).ToIntDef(512);
}
else N = 512;
N2 = N / 2;
// time_begin
if ((CommandLineArgumentsAnalyzer->Check("--time-begin") && CommandLineArgumentsAnalyzer->CheckRequiredParams("--time-begin"))) {
    t1 = str_replace(".", ",", CommandLineArgumentsAnalyzer->GetParameter("--time-begin", 0)).ToDouble();
}
else t1 = 0.;
// time_end
if ((CommandLineArgumentsAnalyzer->Check("--time-end") && CommandLineArgumentsAnalyzer->CheckRequiredParams("--time-end"))) {
    t2 = str_replace(".", ",", CommandLineArgumentsAnalyzer->GetParameter("--time-end", 0)).ToDouble();
}
else t2 = 0.;
// time_len
if ((CommandLineArgumentsAnalyzer->Check("--time-len") && CommandLineArgumentsAnalyzer->CheckRequiredParams("--time-len"))) {
    tlen = str_replace(".", ",", CommandLineArgumentsAnalyzer->GetParameter("--time-len", 0)).ToDouble();
    t2 = t1 + tlen;
}

// Преобразование аудио если надо
if (get_extension(InFile).LowerCase() == ".dat") { }
else {
	// Преобразование адио файла (MP3 или WAV) в DAT
	UnicodeString SourceFile = InFile;
    InFile = InFile + ".dat";
    // Конвертируем аудио в DAT
	Common Common;
	UnicodeString SOX = Common.GetSOX();
	Convert(SOX, SourceFile, InFile);
    if (!FileExists(InFile)) {
        std::cout << "Error convert audio file to DAT file format! Please check SOX.exe location" << std::endl;
        return 2;
    }
}

// Блок вычислений
TWavFile *WavFile = new TWavFile(N);

time_begin = (int)(t1 * 48000);
time_end = (int)(t2 * 48000);
if (time_end == 0) time_end = getFileSize(InFile) / 2;
WavFile->LoadFromDAT(InFile, time_begin, time_end - time_begin);
//WavFile->Load(InFile);
WavFile->Calculate();

// Обрезка
// --frequency-cut
if ((CommandLineArgumentsAnalyzer->Check("--frequency-cut") && CommandLineArgumentsAnalyzer->CheckRequiredParams("--frequency-cut"))) {
	int left, right, d_left, d_right;
	double kf, kb;
	left = CommandLineArgumentsAnalyzer->GetParameter("--frequency-cut", 0).ToIntDef(0);
	d_left = 0;
	right = CommandLineArgumentsAnalyzer->GetParameter("--frequency-cut", 1).ToIntDef(0);
	d_right = 0;
    kf = str_replace(".", ",", CommandLineArgumentsAnalyzer->GetParameter("--frequency-cut", 2)).ToDouble();
    kb = str_replace(".", ",", CommandLineArgumentsAnalyzer->GetParameter("--frequency-cut", 3)).ToDouble();
	WavFile->Cut(left, right, d_left, d_right, kf, kb);
}
// Сдвиг
// --frequency-shift
if ((CommandLineArgumentsAnalyzer->Check("--frequency-shift") && CommandLineArgumentsAnalyzer->CheckRequiredParams("--frequency-shift"))) {
	int shift = CommandLineArgumentsAnalyzer->GetParameter("--frequency-shift", 0).ToIntDef(0);
    //std::cout << "Frequency-shift: " << shift << std::endl;
	WavFile->Shift(shift);
}
// Копирование
// --frequency-copy
if ((CommandLineArgumentsAnalyzer->Check("--frequency-copy") && CommandLineArgumentsAnalyzer->CheckRequiredParams("--frequency-copy"))) {
	int left, right, shift;
	double amp;
	left = CommandLineArgumentsAnalyzer->GetParameter("--frequency-copy", 0).ToIntDef(0);
	right = CommandLineArgumentsAnalyzer->GetParameter("--frequency-copy", 1).ToIntDef(0);
    amp = str_replace(".", ",", CommandLineArgumentsAnalyzer->GetParameter("--frequency-copy", 2)).ToDouble();
	shift = CommandLineArgumentsAnalyzer->GetParameter("--frequency-copy", 3).ToIntDef(0);
	WavFile->Copy(left, right,amp, shift);
}
// Растяжение / Сжатие
if ((CommandLineArgumentsAnalyzer->Check("--frequency-stretch") && CommandLineArgumentsAnalyzer->CheckRequiredParams("--frequency-stretch"))) {
	double val = str_replace(".", ",", CommandLineArgumentsAnalyzer->GetParameter("--frequency-stretch", 0)).ToDouble();
	WavFile->Resize(val);
}

// Обратное преобразование
WavFile->ReCalculate();
// Сохранение
// Преобразование аудио если надо
if (get_extension(OutFile).LowerCase() == ".wav") {
	WavFile->Save(OutFile);
}
else if (get_extension(OutFile).LowerCase() == ".dat") {
	WavFile->SaveToDat(OutFile);
}
else {
	// Преобразование адио файла (MP3 или WAV) в DAT
	UnicodeString DatFile = OutFile + ".dat";
	WavFile->SaveToDat(DatFile);
    // Конвертируем аудио в DAT
	Common Common;
	UnicodeString SOX = Common.GetSOX();
	Convert(SOX, DatFile, OutFile);
    if (!FileExists(OutFile)) {
        std::cout << "Error convert audio file to DAT file format! Please check SOX.exe location" << std::endl;
        return 2;
    }
}
// Удаляем ненужные объекты
delete WavFile;
delete CommandLineArgumentsAnalyzer;
return 0;
}
