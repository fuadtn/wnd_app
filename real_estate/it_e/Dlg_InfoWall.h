#pragma once


// ���������� ���� Dlg_InfoWall

class Dlg_InfoWall : public CDialog
{
	DECLARE_DYNAMIC(Dlg_InfoWall)

public:
	Dlg_InfoWall(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~Dlg_InfoWall();

// ������ ����������� ����
	enum { IDD = IDD_DLG_INFOWALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
};
