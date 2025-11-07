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
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TStatusBar *StatusBar1;
	TMainMenu *MainMenu1;
	TMenuItem *N1;
	TMenuItem *vdb21;
	TMenuItem *vdb31;
	TMenuItem *vdb32;
	TMenuItem *N23;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TChart *Chart1;
	TScrollBox *ScrollBox1;
	TImage *Image1;
	TGroupBox *GroupBox1;
	TListBox *ListBox1;
	TGroupBox *GroupBox2;
	TListBox *ListBox2;
	TOpenDialog *OpenDialog1;
	TOpenDialog *OpenDialog2;
	TMenuItem *N3;
	TMenuItem *N4;
	TMenuItem *N5;
	TMenuItem *N6;
	TMenuItem *N24;
	TMenuItem *N7;
	void __fastcall vdb21Click(TObject *Sender);
	void __fastcall vdb31Click(TObject *Sender);
	void __fastcall ListBox2Click(TObject *Sender);
	void __fastcall N23Click(TObject *Sender);
	void __fastcall N4Click(TObject *Sender);
	void __fastcall N6Click(TObject *Sender);
	void __fastcall ListBox1Click(TObject *Sender);
	void __fastcall N7Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
