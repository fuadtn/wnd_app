#pragma once
#include "afxwin.h"


class CDlg_InfoPlan : public CDialog
{
	DECLARE_DYNAMIC(CDlg_InfoPlan)

public:
	CDlg_InfoPlan(CWnd* pParent = NULL);
	enum { IDD = IDD_DLG_INFOPLAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic m_Picture;
	int c_counter;
	afx_msg void OnBnClickedNext();
	CString c_iplan;
};
