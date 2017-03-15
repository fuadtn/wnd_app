// Dlg_InfoWall.cpp: файл реализации
//

#include "stdafx.h"
#include "it_e.h"
#include "Dlg_InfoWall.h"
#include "afxdialogex.h"


// диалоговое окно Dlg_InfoWall

IMPLEMENT_DYNAMIC(Dlg_InfoWall, CDialog)

Dlg_InfoWall::Dlg_InfoWall(CWnd* pParent /*=NULL*/)
	: CDialog(Dlg_InfoWall::IDD, pParent)
{

}

Dlg_InfoWall::~Dlg_InfoWall()
{
}

void Dlg_InfoWall::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dlg_InfoWall, CDialog)
END_MESSAGE_MAP()


// обработчики сообщений Dlg_InfoWall
