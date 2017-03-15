#include "stdafx.h"
#include "it_e.h"
#include "Dlg_InfoPlan.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CDlg_InfoPlan, CDialog)

CDlg_InfoPlan::CDlg_InfoPlan(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_InfoPlan::IDD, pParent)
	, c_counter(1)
	, c_iplan(_T(""))
{

}

void CDlg_InfoPlan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BMP, m_Picture);
	DDX_Text(pDX, IDC_EDIT1, c_iplan);
}

BEGIN_MESSAGE_MAP(CDlg_InfoPlan, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CDlg_InfoPlan::OnBnClickedNext)
END_MESSAGE_MAP()

BOOL CDlg_InfoPlan::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_Picture.SetBitmap(::LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BMP1)));
	c_iplan = L"Вариант планировки №1";
	UpdateData(FALSE);
	return TRUE;
}

void CDlg_InfoPlan::OnBnClickedNext()
{
	if (c_counter == 0)
	{
		m_Picture.SetBitmap(::LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BMP1)));
		c_iplan = L"Вариант планировки №1";
	}
	if (c_counter == 1)
	{
		m_Picture.SetBitmap(::LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BMP2)));
		c_iplan = L"Вариант планировки №2";
	}	
	if (c_counter == 2)
	{
		m_Picture.SetBitmap(::LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BMP3)));
		c_iplan = L"Вариант планировки №3";
	}	
	if (c_counter == 3)
	{
		m_Picture.SetBitmap(::LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BMP4)));
		c_iplan = L"Вариант планировки №4";
	}	
	if (c_counter == 4)
	{
		m_Picture.SetBitmap(::LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BMP5)));
		c_iplan = L"Вариант планировки №5";
	}	
	if (c_counter == 5)
	{
		m_Picture.SetBitmap(::LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BMP6)));
		c_iplan = L"Вариант планировки №6";
	}

	UpdateData(FALSE);
	c_counter++;
	c_counter %= 6;
}
