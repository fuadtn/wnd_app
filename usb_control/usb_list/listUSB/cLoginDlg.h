#pragma once


// ���������� ���� cLoginDlg

class cLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(cLoginDlg)

public:
	cLoginDlg(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~cLoginDlg();

// ������ ����������� ����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CString vLogin;
	CString vPassword;
	afx_msg void OnBnClickedOk();
	int vLoginPort;
	CString vLoginIp;
	bool is_login;
};
