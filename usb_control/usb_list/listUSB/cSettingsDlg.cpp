// cSettingsDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "listUSB.h"
#include "afxdialogex.h"


// диалоговое окно cSettingsDlg

IMPLEMENT_DYNAMIC(cSettingsDlg, CDialog)

cSettingsDlg::cSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SETTINGS_DIALOG, pParent)
	, vPort()
	, vIpAddres(_T(""))
{

}

cSettingsDlg::~cSettingsDlg()
{
}

void cSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PORT_EDIT, vPort);
	DDX_Text(pDX, IDC_IP_EDIT, vIpAddres);
}


BEGIN_MESSAGE_MAP(cSettingsDlg, CDialog)
	ON_BN_CLICKED(IDOK, &cSettingsDlg::OnBnClickedOk)
END_MESSAGE_MAP()




void cSettingsDlg::OnBnClickedOk()
{
	CDialog::OnOK();
}
