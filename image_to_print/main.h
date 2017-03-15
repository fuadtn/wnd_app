//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TButton *OpenImageBtn;
	TButton *PrintImageBtn;
	TOpenDialog *OpenImageDialog;
	void __fastcall OpenImageBtnClick(TObject *Sender);
	void __fastcall PrintImageBtnClick(TObject *Sender);
	void __fastcall OnPaint(TObject *Sender);
private:
	RECT Preview;
	TImage *ImageToPrint;
	TJPEGImage * JpegImage;
    UnicodeString ImageFileName;
	POINT GetSize(TImage *Image, int PrinterWidth);
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
	virtual __fastcall ~TMainForm();
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
