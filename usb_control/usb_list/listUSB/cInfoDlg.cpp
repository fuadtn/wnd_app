// cInfoDlg.cpp: ���� ����������
//

#include "stdafx.h"
#include "listUSB.h"
#include "cInfoDlg.h"
#include "afxdialogex.h"


// ���������� ���� cInfoDlg

IMPLEMENT_DYNAMIC(cInfoDlg, CDialog)

cInfoDlg::cInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_INFODLG, pParent)
{

}

cInfoDlg::~cInfoDlg()
{
}

void cInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cInfoDlg, CDialog)
END_MESSAGE_MAP()


// ����������� ��������� cInfoDlg
