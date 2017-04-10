#pragma once
#include "afxwin.h"

#define POST_BUFFER_SIZE 16384

struct bfStruct
{
	BOOL is_list;
	char hBuf[POST_BUFFER_SIZE];
};

// ���������� ���� cUSBDlg
class cUSBDlg : public CDialog
{
// ��������
public:
	cUSBDlg(CWnd* pParent = NULL);	// ����������� �����������

// ������ ����������� ����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LISTUSB_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV


// ����������
protected:
	HICON m_hIcon;

	// ��������� ������� ����� ���������
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
protected:
	afx_msg LRESULT OnDeviceChange(WPARAM wParam, LPARAM lParam);
public:
	CListBox vListBox;
	afx_msg void OnLbnDblclkListUsb();
	CString vStatus;
	afx_msg void OnBnClickedSaveToFile();
	afx_msg void OnBnClickedInfo();
	afx_msg void OnQuit();
	afx_msg void OnInfo();
	afx_msg void OnAbout();
	afx_msg void OnSettings();
	afx_msg void OnLogin();
	int SETTINGS_PORT;
	CString SETTINGS_IP;
	bool isLogin = FALSE;
	static CString PostRequest(CString IpAddress, int Port, CString HttpObj, CString hSContent, bfStruct &bf);
	static CString ConvertToFormat(wchar_t *dv_name);
};
