#pragma once


// ���������� ���� CDlg_InfoRn

class CDlg_InfoRn : public CDialog
{
	DECLARE_DYNAMIC(CDlg_InfoRn)

public:
	CDlg_InfoRn(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~CDlg_InfoRn();

// ������ ����������� ����
	enum { IDD = IDD_DLG_INFORN};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
};
