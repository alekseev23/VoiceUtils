//---------------------------------------------------------------------------

#ifndef Unit9H
#define Unit9H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm9 : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TBitBtn *BitBtn1;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TGroupBox *GroupBox3;
	TGroupBox *GroupBox4;
	TLabel *Label1;
	TEdit *Edit1;
	TLabel *Label2;
	TEdit *Edit2;
	TEdit *Edit3;
	TLabel *Label3;
	TEdit *Edit4;
	TLabel *Label4;
	TEdit *Edit5;
	TLabel *Label5;
	TEdit *Edit6;
	TLabel *Label6;
	TBitBtn *BitBtn2;
	void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm9(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm9 *Form9;
//---------------------------------------------------------------------------
#endif
