//---------------------------------------------------------------------------

#ifndef Unit22H
#define Unit22H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <VCLTee.Chart.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
//---------------------------------------------------------------------------
class TForm22 : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *N1;
	TStatusBar *StatusBar1;
	TTabControl *TabControl1;
	TPanel *Panel1;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TBitBtn *BitBtn3;
	TBitBtn *BitBtn4;
	TGroupBox *GroupBox1;
	TListBox *ListBox1;
	TChart *Chart1;
	TTabControl *TabControl2;
private:	// User declarations
public:		// User declarations
	__fastcall TForm22(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm22 *Form22;
//---------------------------------------------------------------------------
#endif
