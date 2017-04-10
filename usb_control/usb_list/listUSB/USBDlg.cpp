#include "stdafx.h"
#include "listUSB.h"
#include "afxdialogex.h"
#include <initguid.h>
#include <Dbt.h>
#include <Usbiodef.h>
#include <fstream>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


cUSBDlg::cUSBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_LISTUSB_DIALOG, pParent)
	, vStatus(_T(""))
	, SETTINGS_PORT(0)
	, SETTINGS_IP(_T(""))
	, isLogin(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_USBICON);
}

void cUSBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTUSB, vListBox);
	DDX_Text(pDX, IDC_STATUS, vStatus);
}

BEGIN_MESSAGE_MAP(cUSBDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_DEVICECHANGE, &cUSBDlg::OnDeviceChange)
	ON_LBN_DBLCLK(IDC_LISTUSB, &cUSBDlg::OnLbnDblclkListUsb)
	ON_BN_CLICKED(IDC_BUTTON1, &cUSBDlg::OnBnClickedSaveToFile)
	ON_COMMAND(ID_QUIT, &cUSBDlg::OnQuit)
	ON_COMMAND(ID_INFO, &cUSBDlg::OnInfo)
	ON_COMMAND(ID_ABOUT, &cUSBDlg::OnAbout)
	ON_COMMAND(ID_SETTINGS, &cUSBDlg::OnSettings)
	ON_COMMAND(ID_LOGIN, &cUSBDlg::OnLogin)
END_MESSAGE_MAP()


BOOL cUSBDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	vStatus = L"Для работы с программой авторизуйтесь";
	UpdateData(FALSE);

	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
	ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
	NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_USB_DEVICE;
	HDEVNOTIFY hDevNotify = RegisterDeviceNotification(this->m_hWnd, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);

	SETTINGS_IP = "192.168.16.128";
	SETTINGS_PORT = 80;

	return TRUE;
}


void cUSBDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


void cUSBDlg::OnPaint()
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


HCURSOR cUSBDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


afx_msg LRESULT cUSBDlg::OnDeviceChange(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case DBT_DEVICEARRIVAL:
	{
		PDEV_BROADCAST_DEVICEINTERFACE pInterface = (PDEV_BROADCAST_DEVICEINTERFACE)lParam;
		if (pInterface->dbcc_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
		{
			CString CurrentDevice = ConvertToFormat(pInterface->dbcc_name);
			if (vListBox.FindStringExact(0, CurrentDevice) == LB_ERR)
			{
				vListBox.AddString(CurrentDevice);
				vStatus = L"Обнаружено USB-устройство";
			}
			else
			{
				vStatus = L"USB-устройство уже содержится в списке";
			}
			UpdateData(FALSE);
		}
		break;
	}	
	case DBT_DEVICEREMOVECOMPLETE:
	{		
		if (isLogin == TRUE)
		vStatus = L"Вставьте USB-устройство и дождитесь реакции программы";
		else
		vStatus = L"Для работы с программой авторизуйтесь";
		UpdateData(FALSE);
		break;
	}
	}
	return 0;
}


void cUSBDlg::OnLbnDblclkListUsb()
{
	int Index = vListBox.GetCurSel();
	vListBox.DeleteString(Index);
}


void cUSBDlg::OnBnClickedSaveToFile()
{
	if (isLogin)
	{
		int count = vListBox.GetCount();
		if (count == 0)
		{
			vStatus.Format(L"Отсутствуют объекты для передачи...");
			UpdateData(FALSE);
			return;
		}

		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		CString PostData = L"devices=";
		CString sCStr;

		for (int i = 0; i < count; i++)
		{
			vListBox.GetText(i, sCStr);
			PostData += sCStr;
			if(i != (count - 1))
			PostData += L"  ";
		}

		int position = 0;
		while ((position = PostData.Find('&')) != -1)
		{
			PostData.Delete(position, 1);
			PostData.Insert(position, L"%26");
		}

		bfStruct bf;
		bf.is_list = FALSE;
		CString Result = cUSBDlg::PostRequest(SETTINGS_IP, SETTINGS_PORT, L"/send-list/", PostData, bf);

		if (Result == HTTP_OK)
			vStatus.Format(L"Отправлено [%d] на %s:%d", count, SETTINGS_IP, SETTINGS_PORT);
		else
			vStatus.Format(L"Передача не удалась...");
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	}
	else
		vStatus.Format(L"Вы не авторизованы. Передача прервана...");
	UpdateData(FALSE);
}


void cUSBDlg::OnQuit()
{
	CDialog::OnCancel();
}


void cUSBDlg::OnInfo()
{
	cInfoDlg idlg;
	idlg.DoModal();
}


void cUSBDlg::OnAbout()
{
	CAboutDlg adlg;
	adlg.DoModal();
}


void cUSBDlg::OnSettings()
{
	cSettingsDlg sdlg(this);
	sdlg.vIpAddres = SETTINGS_IP;
	sdlg.vPort = SETTINGS_PORT;
	
	INT_PTR return_value;
	return_value = sdlg.DoModal();

	if (return_value == IDOK)
	{
		if(sdlg.vIpAddres != "")
		SETTINGS_IP = sdlg.vIpAddres;
		SETTINGS_PORT = sdlg.vPort;
	}
}


void cUSBDlg::OnLogin()
{
	cLoginDlg ldlg;
	ldlg.vLoginIp = SETTINGS_IP;
	ldlg.vLoginPort = SETTINGS_PORT;

	INT_PTR return_value;
	return_value = ldlg.DoModal();

	if (return_value == IDOK)
	{
		isLogin = ldlg.is_login;
		if (isLogin == TRUE)
			vStatus = L"Вставьте USB-устройство и дождитесь реакции программы";
		else
			vStatus = L"Некорректные данные. Авторизация прошла безуспешно...";
		UpdateData(FALSE);
	}
}


CString cUSBDlg::PostRequest(CString IpAddress, int Port, CString HttpObj, CString hSContent, bfStruct &bf)
{
	BOOL result = FALSE;
	DWORD sz;
	wchar_t status_code[16] = { '\0' };
	CString return_result;

	size_t convertedCharsw = 0;
	size_t wsLength = (hSContent.GetLength() + 1) * 2;
	char *hContent = new char[wsLength];
	wcstombs_s(&convertedCharsw, hContent, wsLength, hSContent, _TRUNCATE);

	HINTERNET hInternet = InternetOpen(L"WINUSB.RS UAGENT", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet)
	{
		HINTERNET hConnect = InternetConnect(hInternet, IpAddress, Port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
		if (hConnect)
		{
			const wchar_t *AcceptType[] = { L"*/*", NULL };
			HINTERNET hRequest = HttpOpenRequest(hConnect, L"POST", HttpObj, NULL, NULL, AcceptType, 0, NULL);
			if (hRequest)
			{
				const wchar_t *pHeader = L"Content-Type: application/x-www-form-urlencoded";
				if (hContent)
					result = HttpSendRequest(hRequest, pHeader, wcslen(pHeader), static_cast<LPVOID>(hContent), strlen(hContent));
				else
					result = HttpSendRequest(hRequest, pHeader, wcslen(pHeader), NULL, 0);

				if (result)
				{
					HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE, status_code, &sz, NULL);
					return_result.Format(L"%s", status_code);
				}

				if (bf.is_list && result)
				{
					DWORD dwBytesRead;
					BOOL st = InternetReadFile(hRequest, static_cast<LPVOID>(bf.hBuf), POST_BUFFER_SIZE, &dwBytesRead);
					if (dwBytesRead > 0)
						bf.hBuf[dwBytesRead - 1] = '\0';
					bf.hBuf[dwBytesRead] = '\0';
				}
			}
			InternetCloseHandle(hRequest);
		}
		InternetCloseHandle(hConnect);
	}
	InternetCloseHandle(hInternet);
	delete[] hContent;
	return return_result;
}


CString cUSBDlg::ConvertToFormat(wchar_t *dv_name)
{
	CString Convert(dv_name);
	
	int psStart = 0;
	int psEnd = 0;

	psStart = Convert.Find('{');
	psEnd = Convert.Find('}');
	Convert.Delete(psStart, psEnd - psStart + 1);

	while (((psStart = Convert.Find('\\')) != -1) || ((psStart = Convert.Find('?')) != -1))
	Convert.Delete(psStart, 1);

	while ((psStart = Convert.Find('#')) != -1)
	{
		Convert.Delete(psStart, 1);
		Convert.Insert(psStart, L"\\");
	}
	if(Convert[Convert.GetLength()-1] == L'#')
	Convert.Delete(Convert.GetLength() - 1, 1);

	return Convert;
}
