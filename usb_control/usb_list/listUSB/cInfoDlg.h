#pragma once


// ���������� ���� cInfoDlg

class cInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(cInfoDlg)

public:
	cInfoDlg(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~cInfoDlg();

// ������ ����������� ����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INFODLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
};
