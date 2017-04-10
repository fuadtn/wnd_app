#include "stdafx.h"
#include "listUSB.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(cLoginDlg, CDialog)

static size_t write_data(char *ptr, size_t size, size_t nmemb, std::string* data)
{
	if (data)
	{
		data->append(ptr, size*nmemb);
		return size*nmemb;
	}
	else return 0;  // будет ошибка
}

cLoginDlg::cLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_LOGIN_DIALOG, pParent)
	, vLogin(_T(""))
	, vPassword(_T(""))
	, vLoginPort(0)
	, vLoginIp(_T(""))
	, is_login(false)
{

}

cLoginDlg::~cLoginDlg()
{
}

void cLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOGIN, vLogin);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, vPassword);
}


BEGIN_MESSAGE_MAP(cLoginDlg, CDialog)
	ON_BN_CLICKED(IDOK, &cLoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


void cLoginDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	CString PostData;
	PostData = L"username=" + vLogin;
	PostData += L"&password=" + vPassword;

	bfStruct bf;
	bf.is_list = FALSE;

	CString Result = cUSBDlg::PostRequest(vLoginIp, vLoginPort, L"/autorization/", PostData, bf);

	if (Result == HTTP_OK)
	is_login = TRUE;
	else
	is_login = FALSE;

	GetDlgItem(IDOK)->EnableWindow(TRUE);
	CDialog::OnOK();
}
