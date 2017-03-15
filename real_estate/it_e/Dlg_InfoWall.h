#pragma once


// диалоговое окно Dlg_InfoWall

class Dlg_InfoWall : public CDialog
{
	DECLARE_DYNAMIC(Dlg_InfoWall)

public:
	Dlg_InfoWall(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~Dlg_InfoWall();

// Данные диалогового окна
	enum { IDD = IDD_DLG_INFOWALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
};
