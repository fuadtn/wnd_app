#pragma once
#include "afxcmn.h"


// ���������� ���� cSettingsDlg

class cSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(cSettingsDlg)

public:
	cSettingsDlg(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~cSettingsDlg();

// ������ ����������� ����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	int vPort;
	CString vIpAddres;
	afx_msg void OnBnClickedOk();
};
