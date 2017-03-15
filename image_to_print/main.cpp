#include <vcl.h>
#pragma hdrstop

#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include <float.h>
#define NONE ""

TMainForm *MainForm;
//---------------------------------------------------------------------------

__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
	_control87(MCW_EM, MCW_EM);
	JpegImage = new TJPEGImage;
	ImageToPrint = new TImage(this);
	ImageToPrint->Transparent = true;
	ImageToPrint->Stretch = true;

	ImageFileName = NONE;
	Preview.left = 10;
	Preview.top = 10;
	Preview.right = 320;
	Preview.bottom = 449;
}
//---------------------------------------------------------------------------

__fastcall TMainForm::~TMainForm()
{
    delete this->JpegImage;
	delete this->ImageToPrint;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::OpenImageBtnClick(TObject *Sender)
{
	//  ���� ����� ����� � ���� ��� ������� ����������, �� ���������
	//	��� ����� ���������� ������
	if(OpenImageDialog->Execute())
	ImageFileName = OpenImageDialog->FileName;
	// 	���� ���������� �������� �����, �� ��������������� ��������
	// 	� ���� �����
	if(ImageFileName != NONE)
	{
		// ���� ���������� ����� ".bmp"
		if(	PathFindExtension(ImageFileName.c_str()) == (UnicodeString)".bmp")
		{
			ImageToPrint->Picture->LoadFromFile(ImageFileName);
		}
		else
		{
			JpegImage->LoadFromFile(ImageFileName);
			ImageToPrint->Picture->Bitmap->Assign(JpegImage);
		}

        OnPaint(Sender);
	}
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::PrintImageBtnClick(TObject *Sender)
{
	// ������� ������ ������ �������� (��� �� ������ �������� ���������)
	TPrintDialog *PrinterDlg = new TPrintDialog(this);
	PrinterDlg->MinPage = 1;
	PrinterDlg->MaxPage = 1;

	// ���� ����� ��� ����������, �� ���������� � ������
	if(PrinterDlg->Execute())
	{
		TRect RC;
        // ��������� �� ���������� ����� ��������
		TPrinter *MyPrinter = Printer();
		// �������� ���������� � ��������� ����� ������ ��������
		TPrinterInfo2 *PrinterInfo;
		DWORD Needed, Returned;
		EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 2, NULL, 0, &Needed, &Returned);
		PrinterInfo = new TPrinterInfo2[Needed];
		EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 2, (LPBYTE) PrinterInfo, Needed, &Needed, &Returned);

		// ���������� ������� � ������� ����������� � ���������
		POINT Size = GetSize(ImageToPrint, MyPrinter->PageWidth);
		RC.left = (MyPrinter->PageWidth - Size.x) / 2;
		RC.top = (MyPrinter->PageHeight - Size.y) / 2;
		RC.right = RC.left + Size.x;
		RC.bottom = RC.top + Size.y;

 		// ������ ��������� ���������
		MyPrinter->BeginDoc();
		MyPrinter->Canvas->StretchDraw(RC, ImageToPrint->Picture->Bitmap);

        // ��������� ������ �� ���������� ��������
		MyPrinter->Canvas->Font->Size = 14;
		for(int p = 0; p < 2; p++)
		{
			MyPrinter->Canvas->TextOut(
				p*(MyPrinter->PageWidth - 3000) + 100,
				p*(MyPrinter->PageHeight - 1300) + 100,
				"��������� ������: " + (String) PrinterInfo[MyPrinter->PrinterIndex].pPrinterName
			);
			MyPrinter->Canvas->TextOut(
				p*(MyPrinter->PageWidth - 3000) + 100,
				p*(MyPrinter->PageHeight - 1300) + 300,
				"��������� ������: " + (String) PrinterInfo[MyPrinter->PrinterIndex].pPrintProcessor
			);
			MyPrinter->Canvas->TextOut(
				p*(MyPrinter->PageWidth - 3000) + 100,
				p*(MyPrinter->PageHeight - 1300) + 500,
				"�������� �����: " + (String) PrinterInfo[MyPrinter->PrinterIndex].pPortName
			);
			MyPrinter->Canvas->TextOut(
				p*(MyPrinter->PageWidth - 3000) + 100,
				p*(MyPrinter->PageHeight - 1300) + 700,
				"�������� ��������: " + (String) PrinterInfo[MyPrinter->PrinterIndex].pDriverName
			);
			MyPrinter->Canvas->TextOut(
				p*(MyPrinter->PageWidth - 3000) + 100,
				p*(MyPrinter->PageHeight - 1300) + 900,
				"���������: " + (String) PrinterInfo[MyPrinter->PrinterIndex].pParameters
			);
		}
		MyPrinter->EndDoc();
		delete PrinterDlg;
		delete PrinterInfo;
	}
}
//---------------------------------------------------------------------------

POINT TMainForm::GetSize(TImage *Image, int PrinterWidth)
{
	double	Scale = 1;
	POINT 	Result;

    //���� ������ ������ ������, �� ������ ������� ������������ ������
	if(Image->Picture->Bitmap->Width >= Image->Picture->Bitmap->Height)
		Scale = (double) PrinterWidth / Image->Picture->Bitmap->Width;
	// ����� ������������, ��� �������� - ����������, � ����������
	// ������� �� ������
	else
		Scale = (double) PrinterWidth / Image->Picture->Bitmap->Height;

	Result.x = (double) Image->Picture->Bitmap->Width * Scale;
	Result.y = (double) Image->Picture->Bitmap->Height * Scale;
	return Result;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::OnPaint(TObject *Sender)
{
	// �������������� �������� ������ ���, ����� ��������� ��������
	// ������� OnPaint
	Canvas->Pen->Color = (TColor) RGB(150,150,150);
	Canvas->Brush->Color = (TColor) RGB(255,255,255);
	Canvas->Rectangle(
		Preview.left-1,
		Preview.top-1,
		Preview.right+1,
		Preview.bottom+1
	);

	if(!ImageToPrint->Picture->Bitmap->Empty)
	{
		TRect RC;
		POINT Size = GetSize(ImageToPrint, Preview.right - Preview.left);
		RC.left = Preview.left + (Preview.right - Preview.left - Size.x) / 2;
		RC.top = Preview.top + (Preview.bottom - Preview.top - Size.y) / 2;
		RC.right = RC.left + Size.x;
		RC.bottom = RC.top + Size.y;
		this->Canvas->StretchDraw(RC, ImageToPrint->Picture->Bitmap);
	}
}
//---------------------------------------------------------------------------

