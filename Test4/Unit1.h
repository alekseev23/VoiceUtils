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
	TMainMenu *MainMenu1;
	TMenuItem *N1;
	TMenuItem *N41;
	TMenuItem *N3;
	TListBox *ListBox1;
	TListBox *ListBox2;
	TChart *Chart1;
	TOpenDialog *OpenDialog1;
	TOpenDialog *OpenDialog2;
	TListBox *ListBox3;
	TActionList *ActionList1;
	TAction *MakeUnvisible;
	TAction *MakeUnvisible2;
	void __fastcall N41Click(TObject *Sender);
	void __fastcall N3Click(TObject *Sender);
	void __fastcall ListBox1Click(TObject *Sender);
	void __fastcall ListBox2Click(TObject *Sender);
	void __fastcall ListBox3Click(TObject *Sender);
	void __fastcall ListBox1DblClick(TObject *Sender);
	void __fastcall MakeUnvisibleExecute(TObject *Sender);
	void __fastcall ListBox2DblClick(TObject *Sender);
	void __fastcall MakeUnvisible2Execute(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
