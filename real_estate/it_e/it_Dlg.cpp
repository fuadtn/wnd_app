#include "afxdialogex.h"
#include "stdafx.h"
#include "it_e.h"
#include "it_Dlg.h"
#include "Dlg_InfoRn.h"
#include "Dlg_InfoWall.h"
#include "Dlg_InfoPlan.h"
#include <afxdb.h>
#include <odbcinst.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Cit_Dlg::Cit_Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cit_Dlg::IDD, pParent)
	, e_rn(0)
	, e_mwall(0)
	, e_floor1(0)
	, e_floor2(0)
	, e_garea1(0)
	, e_garea2(0)
	, e_carea1(0)
	, e_carea2(0)
	, e_price1(0)
	, e_price2(0)
	, e_cost(0)
	, e_alayout(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	nSortColumn = 0;
	bSortAsc = TRUE;
}

void Cit_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, EstateList);
	DDX_Control(pDX, IDC_PROGRESS1, c_progress);
	DDX_Text(pDX, IDC_EDIT14, e_rn);
	DDX_Text(pDX, IDC_EDIT4, e_mwall);
	DDX_Text(pDX, IDC_EDIT6, e_floor1);
	DDX_Text(pDX, IDC_EDIT7, e_floor2);
	DDX_Text(pDX, IDC_EDIT8, e_garea1);
	DDX_Text(pDX, IDC_EDIT9, e_garea2);
	DDX_Text(pDX, IDC_EDIT10, e_carea1);
	DDX_Text(pDX, IDC_EDIT11, e_carea2);
	DDX_Text(pDX, IDC_EDIT12, e_price1);
	DDX_Text(pDX, IDC_EDIT13, e_price2);
	DDX_Text(pDX, IDC_EDIT1, e_cost);
	DDX_Text(pDX, IDC_EDIT5, e_alayout);
}

BEGIN_MESSAGE_MAP(Cit_Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(HDN_ITEMCLICK, 0, &Cit_Dlg::OnHdnItemClickEL)
	ON_BN_CLICKED(IDC_BUTTON6, &Cit_Dlg::OnBnClickedParam)
	ON_BN_CLICKED(IDC_BUTTON7, &Cit_Dlg::OnBnClickedRefresh)
	ON_BN_CLICKED(IDC_BUTTON10, &Cit_Dlg::OnBnClickedInfoRn)
	ON_BN_CLICKED(IDC_BUTTON9, &Cit_Dlg::OnBnClickedInfoWall)
	ON_BN_CLICKED(IDC_BUTTON12, &Cit_Dlg::OnBnClickedResetParam)
	ON_BN_CLICKED(IDC_BUTTON11, &Cit_Dlg::OnBnClickedInfoPlan)
END_MESSAGE_MAP()

BOOL Cit_Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	EstateList.InsertColumn(0, L"Район", LVCFMT_LEFT, 45, -1);
	EstateList.InsertColumn(1, L"Общая площадь", LVCFMT_LEFT, 65, -1);
	EstateList.InsertColumn(2, L"Площадь кухни", LVCFMT_LEFT, 60, -1);
	EstateList.InsertColumn(3, L"Этаж", LVCFMT_LEFT, 40, -1);
	EstateList.InsertColumn(4, L"Материал стен", LVCFMT_LEFT, 50, -1);
	EstateList.InsertColumn(5, L"Планировка", LVCFMT_LEFT, 50, -1);
	EstateList.InsertColumn(6, L"Цена, тыс. руб.", LVCFMT_LEFT, 60, -1);

	c_progress.SetRange(0, 1380);
	c_progress.SetStep(1);

	OpenExcelDb(L"SELECT * FROM estate");
	return TRUE;
}

void Cit_Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR Cit_Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cit_Dlg::OpenExcelDb(CString sSQL)
{
	CDatabase database;
	CString sDsn;
	CString sItem;
	CString sFile = L"restate.xls";
	CString sDriver = L"Driver do Microsoft Excel(*.xls)";

	sDsn.Format(L"ODBC;DRIVER={%s};DSN='';DBQ=%s", sDriver, sFile);
	database.Open(NULL, false, false, sDsn);
	CRecordset recset(&database);

	recset.Open(CRecordset::dynaset, sSQL);

	EstateList.DeleteAllItems();
	c_progress.SetPos(0);

	while (!recset.IsEOF())
	{
		recset.GetFieldValue(L"rn", sItem);
		int nItem = EstateList.InsertItem(EstateList.GetItemCount(), sItem, -1);

		recset.GetFieldValue(L"garea", sItem);
		EstateList.SetItemText(nItem, 1, sItem);

		recset.GetFieldValue(L"carea", sItem);
		EstateList.SetItemText(nItem, 2, sItem);

		recset.GetFieldValue(L"floor", sItem);
		EstateList.SetItemText(nItem, 3, sItem);

		recset.GetFieldValue(L"mwall", sItem);
		EstateList.SetItemText(nItem, 4, sItem);

		recset.GetFieldValue(L"alayout", sItem);
		EstateList.SetItemText(nItem, 5, sItem);

		recset.GetFieldValue(L"price", sItem);
		EstateList.SetItemText(nItem, 6, sItem);

		c_progress.StepIt();
		recset.MoveNext();
	}

	recset.Close();
	database.Close();
	c_progress.SetPos(1380);
	c_progress.SetPos(0);
}

void Cit_Dlg::OnHdnItemClickEL(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	if (nSortColumn != phdr->iItem)
	bSortAsc = TRUE;
	else
	bSortAsc = !bSortAsc;
	nSortColumn = phdr->iItem;

	EstateList.SortItemsEx(CompareFunc, (DWORD_PTR) this);
	*pResult = 0;
}

int CALLBACK Cit_Dlg::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	Cit_Dlg *pThis = (Cit_Dlg*)lParamSort;

	CString sItem1 = pThis->EstateList.GetItemText(static_cast<int>(lParam1), pThis->nSortColumn);
	CString sItem2 = pThis->EstateList.GetItemText(static_cast<int>(lParam2), pThis->nSortColumn);

	int x1 = _tstoi(sItem1.GetBuffer());
	int x2 = _tstoi(sItem2.GetBuffer());
	int result = 0;

	if (x1 < x2)
		result = -1;
	else if (x1 == x2)
		result = 0;
	else
		result = 1;

	if (pThis->bSortAsc == FALSE)
	result = (-1) * result;
	return result;
}

void Cit_Dlg::OnBnClickedParam()
{
	CString sSQL = L"SELECT * FROM estate";
	CString sRequest;
	UpdateData(TRUE);

	int isFirst = 0;

	if (e_rn != 0)
	{
		sSQL += L" WHERE";
		isFirst = 1;

		sRequest.Format(L" rn=%d", e_rn);
		sSQL += sRequest;
	}
	if (e_mwall != 0)
	{
		if (isFirst == 0)
		{
			sSQL += L" WHERE";
			isFirst = 1;
		}
		else
			sSQL += L" AND";

		sRequest.Format(L" mwall=%d", e_mwall);
		sSQL += sRequest;
	}
	if (e_alayout != 0)
	{
		if (isFirst == 0)
		{
			sSQL += L" WHERE";
			isFirst = 1;
		}
		else
			sSQL += L" AND";


		sRequest.Format(L" alayout=%d", e_alayout);
		sSQL += sRequest;
	}
	if (e_floor1 != 0)
	{
		if (isFirst == 0)
		{
			sSQL += L" WHERE";
			isFirst = 1;
		}
		else
			sSQL += L" AND";

		sRequest.Format(L" floor>=%d", e_floor1);
		sSQL += sRequest;
	}
	if (e_floor2 != 0)
	{
		if (isFirst == 0)
		{
			sSQL += L" WHERE";
			isFirst = 1;
		}
		else
			sSQL += L" AND";

		sRequest.Format(L" floor<=%d", e_floor2);
		sSQL += sRequest;
	}
	if (e_garea1 != 0)
	{
		if (isFirst == 0)
		{
			sSQL += L" WHERE";
			isFirst = 1;
		}
		else
			sSQL += L" AND";

		sRequest.Format(L" garea>=%d", e_garea1);
		sSQL += sRequest;
	}
	if (e_garea2 != 0)
	{
		if (isFirst == 0)
		{
			sSQL += L" WHERE";
			isFirst = 1;
		}
		else
			sSQL += L" AND";

		sRequest.Format(L" garea<=%d", e_garea2);
		sSQL += sRequest;
	}
	if (e_carea1 != 0)
	{
		if (isFirst == 0)
		{
			sSQL += L" WHERE";
			isFirst = 1;
		}
		else
			sSQL += L" AND";

		sRequest.Format(L" carea>=%d", e_carea1);
		sSQL += sRequest;
	}
	if (e_carea2 != 0)
	{
		if (isFirst == 0)
		{
			sSQL += L" WHERE";
			isFirst = 1;
		}
		else
			sSQL += L" AND";

		sRequest.Format(L" carea<=%d", e_carea2);
		sSQL += sRequest;
	}
	if (e_price1 != 0)
	{
		if (isFirst == 0)
		{
			sSQL += L" WHERE";
			isFirst = 1;
		}
		else
			sSQL += L" AND";

		sRequest.Format(L" price>=%d", e_price1);
		sSQL += sRequest;
	}
	if (e_price2 != 0)
	{
		if (isFirst == 0)
		{
			sSQL += L" WHERE";
			isFirst = 1;
		}
		else
			sSQL += L" AND";

		sRequest.Format(L" price<=%d", e_price2);
		sSQL += sRequest;
	}

	CalcPrice();
	OpenExcelDb(sSQL);
}

void Cit_Dlg::OnBnClickedRefresh()
{
	OpenExcelDb(L"SELECT * FROM estate");
}

void Cit_Dlg::OnBnClickedInfoRn()
{
	CDlg_InfoRn dlg;
	dlg.DoModal();
}

void Cit_Dlg::OnBnClickedInfoWall()
{
	Dlg_InfoWall dlg;
	dlg.DoModal();
}

void Cit_Dlg::OnBnClickedResetParam()
{
	e_alayout = 0;
	e_rn = 0;
	e_garea1 = 0;
	e_garea2 = 0;
	e_carea1 = 0;
	e_carea2 = 0;
	e_mwall = 0;
	e_floor1 = 0;
	e_floor2 = 0;
	e_price1 = 0;
	e_price2 = 0;

	e_cost = 0;
	UpdateData(FALSE);
}

void Cit_Dlg::OnBnClickedInfoPlan()
{
	CDlg_InfoPlan dlg;
	dlg.DoModal();
}

void Cit_Dlg::CalcPrice()
{
	float cs_rn = 1;
	float cs_layout = 4.2;
	float cs_wall = 2.7;
	float cs_floor = 2.1;
	float cs_garea = 9.4;
	float cs_carea = 6.7;

	float f_price = 0;
	float f_div = 0;
	float f_step = 1;

	CDatabase database;
	CString sDsn;
	CString sItem;
	CString sFile = L"restate.xls";
	CString sDriver = L"Driver do Microsoft Excel(*.xls)";

	sDsn.Format(L"ODBC;DRIVER={%s};DSN='';DBQ=%s", sDriver, sFile);
	database.Open(NULL, false, false, sDsn);
	CRecordset recset(&database);
	recset.Open(CRecordset::dynaset, L"SELECT * FROM estate");

	UpdateData(TRUE);

	while (!recset.IsEOF())
	{
		f_step = 1;
		
		recset.GetFieldValue(L"rn", sItem);
		if (e_rn != 0)
		f_step *= FFunction(e_rn, _tstoi(sItem), cs_rn);								// F1

		recset.GetFieldValue(L"garea", sItem);
		if (e_garea2 != 0)
		f_step *= FFunction(abs((e_garea2 + e_garea1)/2), _tstoi(sItem), cs_garea);		// F2

		recset.GetFieldValue(L"carea", sItem);
		if (e_carea2 != 0)
		f_step *= FFunction(abs((e_carea2 + e_carea1) / 2), _tstoi(sItem), cs_carea);	// F3

		recset.GetFieldValue(L"floor", sItem);
		if (e_floor2 != 0)
		f_step *= FFunction(abs((e_floor2 + e_floor1) / 2), _tstoi(sItem), cs_floor);	// F4

		recset.GetFieldValue(L"mwall", sItem);
		if (e_mwall != 0)
		f_step *= FFunction(e_mwall, _tstoi(sItem), cs_wall);							// F5

		recset.GetFieldValue(L"alayout", sItem);
		if (e_alayout != 0)
		f_step *= FFunction(e_alayout, _tstoi(sItem), cs_layout);						// F6

		f_div += f_step;

		recset.GetFieldValue(L"price", sItem);
		f_step *= _tstoi(sItem);														//  Xi

		f_price += f_step;

		c_progress.StepIt();
		recset.MoveNext();
	}

	if (f_div != 0)
	e_cost = -1;
	e_cost = f_price / f_div;

	UpdateData(FALSE);

	recset.Close();
	database.Close();	
}

float Cit_Dlg::FFunction(int u, int ui, float cs)
{
	if (abs((u - ui) / cs) < 1)
		return 1 - abs((u - ui) / cs);
	else
		return 0;
}
