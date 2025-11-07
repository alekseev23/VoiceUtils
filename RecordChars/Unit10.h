//---------------------------------------------------------------------------

#ifndef Unit10H
#define Unit10H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
//---------------------------------------------------------------------------
class TForm10 : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TBitBtn *BitBtn1;
	TStaticText *StaticText1;
	TComboBox *ComboBox1;
	TComboBox *ComboBox2;
	TStaticText *StaticText2;
	TActionList *ActionList1;
	TAction *Apply;
	TBitBtn *BitBtn3;
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall ApplyExecute(TObject *Sender);
	void __fastcall BitBtn3Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm10(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm10 *Form10;
//---------------------------------------------------------------------------
#endif
