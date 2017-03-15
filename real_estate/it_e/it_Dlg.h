#pragma once
#include "afxcmn.h"

// диалоговое окно Cit_Dlg
class Cit_Dlg : public CDialog
{
public:
	Cit_Dlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
	enum { IDD = IDD_IT_E_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl EstateList;
	void OpenExcelDb(CString sSQL);
	afx_msg void OnHdnItemClickEL(NMHDR *pNMHDR, LRESULT *pResult);
	int nSortColumn;
	BOOL bSortAsc;
	afx_msg void OnBnClickedParam();
	afx_msg void OnBnClickedRefresh();
	CProgressCtrl c_progress;
	int e_rn;
	int e_mwall;
	int e_floor1;
	int e_floor2;
	int e_garea1;
	int e_garea2;
	int e_carea1;
	int e_carea2;
	int e_price1;
	int e_price2;
	int e_alayout;
	float e_cost;
	afx_msg void OnBnClickedInfoRn();
	afx_msg void OnBnClickedInfoWall();
	afx_msg void OnBnClickedResetParam();
	afx_msg void OnBnClickedInfoPlan();
	void CalcPrice();
	float FFunction(int u, int ui, float cs);
};