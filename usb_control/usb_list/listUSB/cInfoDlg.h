#pragma once


// диалоговое окно cInfoDlg

class cInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(cInfoDlg)

public:
	cInfoDlg(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~cInfoDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INFODLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
};
