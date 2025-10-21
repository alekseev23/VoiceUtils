//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit22.h"
#include "../../_Common_/Lib/bitmap.h"
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/RunProcess.h"
#include "../../_Common_/Lib/strings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm22 *Form22;

UnicodeString CurPath;
//---------------------------------------------------------------------------
__fastcall TForm22::TForm22(TComponent* Owner)
	: TForm(Owner)
{
UnicodeString s;
// Текущий каталог
CurPath = ExtractFileDir(Application->ExeName);
//
TStringList *ts = new TStringList;
s = CurPath + "\\voices";
if (!DirectoryExists(s)) MkDir(s);
// Получить список подкаталогов в каталоге
GetDirsFromDirectory(s, false, ts);
for (int i = 0; i < ts->Count; i++) ts->Strings[i] = get_file_name(ts->Strings[i]);
TabControl2->Tabs->Text = ts->Text;
delete ts;
}
//---------------------------------------------------------------------------
