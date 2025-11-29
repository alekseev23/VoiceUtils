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
    std::cout << "Usage: MakeSpectersBitmap.exe " << std::endl
    	<< "\t--time-begin time_begin" << std::endl
        << "\t--time-end time_end" << std::endl
        << "\t-len time_period" << std::endl
    	<< "\t-N specter_size" << std::endl
        << "\t--specters-per-second specters_per_second" << std::endl
        << "\t--specters-width specters_width" << std::endl
    	<< "\t--window-size window_size" << std::endl
        << "\t--shaper-type shaper_type" << std::endl
        << "\t--log-scale" << std::endl
        << "\t--energy-specters" << std::endl
        << "\t--show-grid-lines" << std::endl
        << "\t--audio-file audio_file" << std::endl
        << "\t--picture-file picture_file" << std::endl
    	<< "Example: MakeSpectersBitmap.exe --time-begin 0 --time-end 0 -N 16384 --window-size 512 --specters-per-second 400 --energy-specters -a 1.mp3 -p 1.png" << std::endl
    	<< "Example: MakeSpectersBitmap.exe --time-begin 12.5 --time-len 5.5 -N 1024 -s 48000 --specters-width 200 --log-scale  --shaper-type 4 -a 2.mp3 -p 2.png" << std::endl;
}
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

int _tmain(int argc, _TCHAR* argv[])
{
UnicodeString AudioFile, PictureFile;
int N, N2, specters_width, specters_per_second, time_begin, time_end, scale = 1, shaper_type = 0, window_size = 512;
double t1, t2, tlen;
bool energy_specters = false, log_scale = false, show_grid_lines = false;
// Ищем основные ключи
TCommandLineArgumentsAnalyzer *CommandLineArgumentsAnalyzer = new TCommandLineArgumentsAnalyzer;
CommandLineArgumentsAnalyzer->AddKey("--time-begin", false, 1, 1);
CommandLineArgumentsAnalyzer->AddKey("--time-end", false, 1, 1);
CommandLineArgumentsAnalyzer->AddKey("--time-len", false, 1, 1);
CommandLineArgumentsAnalyzer->AddKey("-N", false, 1, 1);
CommandLineArgumentsAnalyzer->AddKey("--specters-per-second", false, 1, 1);
CommandLineArgumentsAnalyzer->AddKey("--specters-width", false, 1, 1);
CommandLineArgumentsAnalyzer->AddKey("--shaper-type", false, 1, 1);
CommandLineArgumentsAnalyzer->AddKey("--energy-specters", false, 0, 0);
CommandLineArgumentsAnalyzer->AddKey("--show-grid-lines", false, 0, 0);
CommandLineArgumentsAnalyzer->AddKey("--log-scale", false, 0, 0);
CommandLineArgumentsAnalyzer->AddKey("--window-size", false, 1, 1);
CommandLineArgumentsAnalyzer->AddKey("--audio-file", true, 1, 1);
CommandLineArgumentsAnalyzer->AddKey("--picture-file", true, 1, 1);
CommandLineArgumentsAnalyzer->AddArguments(argc, argv, 1, argc);
// Анализ
// Аудио файл
if (CommandLineArgumentsAnalyzer->Check("--audio-file") && CommandLineArgumentsAnalyzer->CheckRequiredParams("--audio-file")) {
    AudioFile = CommandLineArgumentsAnalyzer->GetParameter("--audio-file", 0);
}
else {
    Help();
    return 1;
}
// Выходное изображение
if ((CommandLineArgumentsAnalyzer->Check("--picture-file") && CommandLineArgumentsAnalyzer->CheckRequiredParams("--picture-file"))) {
    PictureFile = CommandLineArgumentsAnalyzer->GetParameter("--picture-file", 0);
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
// specters_per_second
if ((CommandLineArgumentsAnalyzer->Check("--specters-per-second") && CommandLineArgumentsAnalyzer->CheckRequiredParams("--specters-per-second"))) {
    specters_per_second = CommandLineArgumentsAnalyzer->GetParameter("--specters-per-second", 0).ToIntDef(4800);
    if ((specters_per_second < 100) || (specters_per_second > 48000)) {
    	std::cout << "specters_per_second must be between 100 and 48000" << std::endl;
        return 1;
    }
}
else specters_per_second = 4800;
// specters_width
if ((CommandLineArgumentsAnalyzer->Check("--specters-width") && CommandLineArgumentsAnalyzer->CheckRequiredParams("--specters-width"))) {
    specters_width = CommandLineArgumentsAnalyzer->GetParameter("--specters-width", 0).ToIntDef(N2);
    if ((specters_width < 100) || (specters_width > N2)) {
    	std::cout << "specters_width must be between 100 and " << N2 << std::endl;
        return 1;
    }
}
else specters_width = N2;
// shaper_type
if ((CommandLineArgumentsAnalyzer->Check("--shaper-type") && CommandLineArgumentsAnalyzer->CheckRequiredParams("--shaper-type"))) {
    shaper_type = CommandLineArgumentsAnalyzer->GetParameter("--shaper-type", 0).ToIntDef(0);
    if ((shaper_type < 0) || (shaper_type > 4)) {
    	std::cout << "shaper_type must be between 0 and 4" << std::endl;
        return 1;
    }
}
// window_size
if ((CommandLineArgumentsAnalyzer->Check("--window-size") && CommandLineArgumentsAnalyzer->CheckRequiredParams("--window-size"))) {
    window_size = CommandLineArgumentsAnalyzer->GetParameter("--window-size", 0).ToIntDef(512);
    if ((window_size < 100) || (window_size > N2)) {
    	std::cout << "window_size must be between 100 and " << N2 << std::endl;
        return 1;
    }
}
// energy_specters
if (CommandLineArgumentsAnalyzer->Check("--energy-specters")) {
    energy_specters = true;
}
// show_grid_lines
if (CommandLineArgumentsAnalyzer->Check("--show-grid-lines")) {
    show_grid_lines = true;
}
// scale
if (CommandLineArgumentsAnalyzer->Check("--log-scale")) {
    scale = 2;
}
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
if (get_extension(AudioFile).LowerCase() == ".dat") { }
else {
	// Преобразование адио файла (MP3 или WAV) в DAT
	UnicodeString SourceFile = AudioFile;
    AudioFile = AudioFile + ".dat";
    // Конвертируем аудио в DAT
	Common Common;
	UnicodeString SOX = Common.GetSOX();
	Convert(SOX, SourceFile, AudioFile);
    if (!FileExists(AudioFile)) {
        std::cout << "Error convert audio file to DAT file format! Please check SOX.exe location" << std::endl;
        return 2;
    }
}

// Блок вычислений
SpecterWork *SpecterWork = new class SpecterWork;
time_begin = (int)(t1 * 48000);
time_end = (int)(t2 * 48000);
if (time_end == 0) time_end = getFileSize(AudioFile) / 2;
SpecterWork->ReadDataFromDAT(AudioFile, time_begin, time_end - time_begin);
SpecterWork->CountSpectersThread(N, energy_specters, specters_per_second, 0, SpecterWork->BufSize, window_size, shaper_type);
// Создаём изображение
TBitmap *bmp = new TBitmap;
//bmp->PixelFormat = pf24bit;
//bmp->SetSize(SpecterWork->Count, specters_width);
// Рисуем спектрограмму
SpecterWork->DrawSpectrograms2(bmp, scale, specters_width, show_grid_lines);
// Сохраняем изображение
//bmp->SaveToFile("1.bmp");
save_bitmap(bmp, PictureFile, false, 9);
// Удаляем ненужные объекты
delete CommandLineArgumentsAnalyzer;
delete SpecterWork;
return 0;
}
