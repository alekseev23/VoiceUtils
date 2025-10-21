#include "common.h"
#include <System.IniFiles.hpp>
#include "../../_Common_/Lib/RunProcess.h"

//---------------------------------------------------------------------------
bool LogError(UnicodeString message) {
	FILE *fd = fopen("errors.log", "at");
	if (fd == NULL) return false;
	fwprintf(fd, L"%s\t%s\n", FormatDateTime("yyyy-mm-dd hh:mm:ss", Now()).w_str(), message.w_str());
	fclose(fd);
    return true;
}
//---------------------------------------------------------------------------
void GetRegion(int &a, int &b, int SpecterType, int ScaleView) {
    switch (SpecterType) {
        case 0: // 256
        		switch (ScaleView) {
                    case 0: a = 0; b = 256; break;
                    case 8: a = 3; b = 84; break;
                    case 4: a = 3; b = 43; break;
                    case 2: a = 3; b = 21; break;
                }
                break;
        case 1: // 512
        		switch (ScaleView) {
                    case 0: a = 0; b = 512; break;
                    case 8: a = 6; b = 170; break;
                    case 4: a = 6; b = 85; break;
                    case 2: a = 6; b = 42; break;
                }
                break;
        case 2: // 1024
        		switch (ScaleView) {
                    case 0: a = 0; b = 1024; break;
                    case 8: a = 10; b = 340; break;
                    case 4: a = 10; b = 170; break;
                    case 2: a = 10; b = 85; break;
                }
                break;
        case 3: // 2048
        		switch (ScaleView) {
                    case 0: a = 0; b = 2048; break;
                    case 8: a = 21; b = 682; break;
                    case 4: a = 21; b = 340; break;
                    case 2: a = 21; b = 170; break;
                }
                break;
        case 4: // 4096
        		switch (ScaleView) {
                    case 0: a = 42; b = 4096; break;
                    case 8: a = 42; b = 1364; break;
                    case 4: a = 42; b = 682; break;
                    case 2: a = 42; b = 340; break;
                }
                break;
    }
}
//---------------------------------------------------------------------------
__fastcall Common::Common() {
    SOX = "";
    FFMPEG = "";
    RunDir = ExtractFileDir(Application->ExeName);
}
//---------------------------------------------------------------------------
__fastcall Common::~Common() {

}
//---------------------------------------------------------------------------
void __fastcall Common::SetSOX(UnicodeString sox) {
	TIniFile *Ini = new TIniFile(RunDir + "\\SOX.ini");
	Ini->WriteString("SOX", "Path", sox);
	Ini->UpdateFile(); // Сохраняем
	delete Ini;
    SOX = sox;
}
//---------------------------------------------------------------------------
UnicodeString __fastcall Common::GetSOX(void) {
    if (SOX == "") {
        // Ищем в текущем каталоге ".\\sox.exe"
        UnicodeString path = RunDir + "\\sox.exe";
        if (FileExists(path)) SOX = path;
        else { // Ищем в подкаталоге текущго каталога ".\\SOX\\sox.exe"
            path = RunDir + "\\SOX\\sox.exe";
        	if (FileExists(path)) SOX = path;
	        else { // Ищем в SOX.ini
				TIniFile *Ini = new TIniFile(RunDir + "\\SOX.ini");
				SOX = Ini->ReadString("SOX", "Path", "");
				delete Ini;
	        }
        }
        if (SOX == "") { // Ничего не помогло, пытаемся найти с помощью команды "where "
            UnicodeString command, fname;
            fname = RunDir + "\\command.res";   // Результаты команды
            if (FileExists(fname)) DeleteFile(fname);   // Удаляем файл если остался от предыдущего вызова
            command = "where sox.exe"; // Ищем где sox.exe
			RunProcess(command, RunDir, "", fname, ""); // Запускаем процесс
			if (FileExists(fname)) { // Есть результат
	            TStringList *ts = new TStringList;
            	ts->LoadFromFile(fname, TEncoding::UTF8);
                if (ts->Count > 0) SOX = ts->Strings[0]; // Если что то есть
	            delete ts;
                DeleteFile(fname);
            }
        }
    }
    return SOX;
}
//---------------------------------------------------------------------------
__fastcall void Common::SetFFMPEG(UnicodeString ffmpeg) {
	TIniFile *Ini = new TIniFile(RunDir + "\\FFMPEG.ini");
	Ini->WriteString("FFMPEG", "Path", ffmpeg);
	Ini->UpdateFile(); // Сохраняем
	delete Ini;
    FFMPEG = ffmpeg;
}
//---------------------------------------------------------------------------
UnicodeString __fastcall Common::GetFFMPEG(void) {
    if (FFMPEG == "") {
        // Ищем в текущем каталоге
        UnicodeString path = RunDir + "\\ffmpeg.exe";
        if (FileExists(path)) FFMPEG = path;
        else { // Ищем в текущем каталоге ".\\FFMPEG\\ffmpeg.exe"
            path = RunDir + "\\FFMPEG\\FFMPEG.exe";
        	if (FileExists(path)) FFMPEG = path;
        	else { // Ищем в FFMPEG.ini
				TIniFile *Ini = new TIniFile(RunDir + "\\FFMPEG.ini");
				FFMPEG = Ini->ReadString("FFMPEG", "Path", "");
				delete Ini;
	        }
        }
        if (FFMPEG == "") { // Ничего не помогло, пытаемся найти с помощью команды "where "
            UnicodeString command, fname;
            fname = RunDir + "\\command.res";   // Результаты команды
            if (FileExists(fname)) DeleteFile(fname);   // Удаляем файл если остался от предыдущего вызова
            command = "where ffmpeg.exe"; // Ищем где ffmpeg.exe
			RunProcess(command, RunDir, "", fname, ""); // Запускаем процесс
			if (FileExists(fname)) { // Есть результат
	            TStringList *ts = new TStringList;
            	ts->LoadFromFile(fname, TEncoding::UTF8);
                if (ts->Count > 0) FFMPEG = ts->Strings[0]; // Если что то есть
	            delete ts;
                DeleteFile(fname);
            }
        }
    }
    return FFMPEG;
}
//---------------------------------------------------------------------------

