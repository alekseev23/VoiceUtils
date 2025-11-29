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
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TStatusBar *StatusBar1;
	TTrackBar *TrackBar1;
	TMainMenu *MainMenu1;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *N3;
	TMenuItem *N4;
	TMenuItem *N5;
	TMenuItem *N6;
	TMenuItem *N7;
	TMenuItem *N8;
	TMenuItem *N9;
	TMenuItem *N10;
	TMenuItem *N11;
	TMenuItem *N12;
	TMenuItem *N21;
	TMenuItem *N31;
	TMenuItem *N32;
	TMenuItem *N13;
	TMenuItem *N14;
	TMenuItem *N655361;
	TMenuItem *N15;
	TChart *Chart1;
	TOpenDialog *OpenDialog1;
	TActionList *ActionList1;
	TAction *Action1;
	TMenuItem *N16;
	TMenuItem *N17;
	TMenuItem *N18;
	TMenuItem *N19;
	TMenuItem *N20;
	TPanel *Panel1;
	TButton *Button1;
	TButton *Button2;
	TMenuItem *N41;
	TMenuItem *N51;
	TMenuItem *N22;
	TMenuItem *N23;
	TMenuItem *N24;
	void __fastcall N4Click(TObject *Sender);
	void __fastcall N2Click(TObject *Sender);
	void __fastcall Action1Execute(TObject *Sender);
	void __fastcall N14Click(TObject *Sender);
	void __fastcall N15Click(TObject *Sender);
	void __fastcall N655361Click(TObject *Sender);
	void __fastcall N16Click(TObject *Sender);
	void __fastcall N17Click(TObject *Sender);
	void __fastcall N18Click(TObject *Sender);
	void __fastcall N19Click(TObject *Sender);
	void __fastcall N20Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall N6Click(TObject *Sender);
	void __fastcall N8Click(TObject *Sender);
	void __fastcall N13Click(TObject *Sender);
	void __fastcall N12Click(TObject *Sender);
	void __fastcall N21Click(TObject *Sender);
	void __fastcall N31Click(TObject *Sender);
	void __fastcall N41Click(TObject *Sender);
	void __fastcall N51Click(TObject *Sender);
	void __fastcall N23Click(TObject *Sender);
	void __fastcall N24Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
