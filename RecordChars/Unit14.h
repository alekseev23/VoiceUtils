//---------------------------------------------------------------------------

#ifndef Unit14H
#define Unit14H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TForm14 : public TForm
{
__published:	// IDE-managed Components
	TListBox *ListBox1;
	TChart *Chart1;
	TPanel *Panel1;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TActionList *ActionList1;
	TAction *Draw;
	TAction *Calculate;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TChart *Chart2;
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);
	void __fastcall DrawExecute(TObject *Sender);
	void __fastcall CalculateExecute(TObject *Sender);
	void __fastcall ListBox1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm14(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm14 *Form14;
//---------------------------------------------------------------------------
#endif
