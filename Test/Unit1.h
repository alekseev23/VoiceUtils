//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <VCLTee.Chart.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TForm7 : public TForm
{
__published:	// IDE-managed Components
	TStatusBar *StatusBar1;
	TMainMenu *MainMenu1;
	TMenuItem *N1;
	TMenuItem *N41;
	TMenuItem *N3;
	TListBox *ListBox1;
	TListBox *ListBox2;
	TChart *Chart1;
	TOpenDialog *OpenDialog1;
	TOpenDialog *OpenDialog2;
	TMenuItem *N7;
	TMenuItem *N5;
	TMenuItem *N6;
	TMenuItem *X1;
	void __fastcall N41Click(TObject *Sender);
	void __fastcall N3Click(TObject *Sender);
	void __fastcall ListBox1Click(TObject *Sender);
	void __fastcall ListBox2Click(TObject *Sender);
	void __fastcall N5Click(TObject *Sender);
	void __fastcall N6Click(TObject *Sender);
	void __fastcall X1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm7(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm7 *Form7;
//---------------------------------------------------------------------------
#endif
