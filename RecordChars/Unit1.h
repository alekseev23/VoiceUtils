//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
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
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
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
	TMenuItem *N222;
	TMenuItem *N3;
	TMenuItem *N4;
	TMenuItem *N5;
	TActionList *ActionList1;
	TAction *UpdateVoicesList;
	TGroupBox *GroupBox2;
	TListBox *ListBox2;
	TMenuItem *N6;
	TMenuItem *N7;
	TGroupBox *GroupBox3;
	TComboBox *ComboBox1;
	TAction *UpdateAudioInputList;
	TPanel *Panel2;
	TChart *Chart2;
	TSplitter *Splitter1;
	TMenuItem *N8;
	TMenuItem *N2561;
	TMenuItem *N2562;
	TMenuItem *N10241;
	TMenuItem *N10242;
	TMenuItem *N40961;
	TTabControl *TabControl2;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TImage *Image1;
	TPageControl *PageControl2;
	TTabSheet *TabSheet3;
	TTabSheet *TabSheet4;
	TTabSheet *TabSheet5;
	TMenuItem *N200;
	TMenuItem *N9;
	TMenuItem *N10;
	TMenuItem *X1;
	TMenuItem *N11;
	TScrollBox *ScrollBox1;
	TImage *Image2;
	TImage *Image3;
	TTabSheet *TabSheet6;
	TChart *Chart3;
	TMenuItem *N223;
	TMenuItem *N12;
	TMenuItem *N28;
	TTimer *Timer1;
	TTabSheet *TabSheet7;
	TChart *Chart4;
	TBitBtn *BitBtn5;
	TAction *UpdateRecordsList;
	TMenuItem *N25;
	TMenuItem *N13;
	TMenuItem *N14;
	TMenuItem *N15;
	TScrollBox *ScrollBox2;
	TMenuItem *N26;
	TMenuItem *N16;
	TMenuItem *N29;
	TMenuItem *N17;
	TMenuItem *N224;
	TMenuItem *N18;
	TMenuItem *N221;
	TMenuItem *N19;
	TMenuItem *N20;
	void __fastcall N3Click(TObject *Sender);
	void __fastcall UpdateVoicesListExecute(TObject *Sender);
	void __fastcall N4Click(TObject *Sender);
	void __fastcall N5Click(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall UpdateAudioInputListExecute(TObject *Sender);
	void __fastcall ComboBox1Change(TObject *Sender);
	void __fastcall ListBox1Click(TObject *Sender);
	void __fastcall ListBox2Click(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);
	void __fastcall BitBtn3Click(TObject *Sender);
	void __fastcall N2561Click(TObject *Sender);
	void __fastcall N2562Click(TObject *Sender);
	void __fastcall N10241Click(TObject *Sender);
	void __fastcall N10242Click(TObject *Sender);
	void __fastcall N40961Click(TObject *Sender);
	void __fastcall TabControl2Change(TObject *Sender);
	void __fastcall N11Click(TObject *Sender);
	void __fastcall N9Click(TObject *Sender);
	void __fastcall N10Click(TObject *Sender);
	void __fastcall X1Click(TObject *Sender);
	void __fastcall N12Click(TObject *Sender);
	void __fastcall BitBtn4Click(TObject *Sender);
	void __fastcall N28Click(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall BitBtn5Click(TObject *Sender);
	void __fastcall UpdateRecordsListExecute(TObject *Sender);
	void __fastcall N13Click(TObject *Sender);
	void __fastcall N15Click(TObject *Sender);
	void __fastcall N16Click(TObject *Sender);
	void __fastcall N17Click(TObject *Sender);
	void __fastcall N18Click(TObject *Sender);
	void __fastcall N19Click(TObject *Sender);
	void __fastcall N20Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
