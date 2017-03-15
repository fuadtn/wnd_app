#pragma once


// диалоговое окно CDlg_InfoRn

class CDlg_InfoRn : public CDialog
{
	DECLARE_DYNAMIC(CDlg_InfoRn)

public:
	CDlg_InfoRn(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CDlg_InfoRn();

// Данные диалогового окна
	enum { IDD = IDD_DLG_INFORN};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
};
