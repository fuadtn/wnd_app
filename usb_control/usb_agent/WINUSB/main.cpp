#include <windows.h>
#include <Dbt.h>
#include <initguid.h>
#include <Usbiodef.h>
#include <wininet.h>
#include <string>
#include <atlstr.h>
#include <stdio.h>
#include <fstream>
#include "resource.h"

#pragma comment(lib,"wininet")

using namespace std;

#define POST_BUFFER_SIZE 16384
#define WM_TRAY_NOTIFY WM_APP+10
#define HTTP_OK L"200"

static const GUID GUID_DEVINTERFACE_USBSTOR =
{ 0xA5DCBF10L, 0x6530, 0x11D2,{ 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } };

DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
NOTIFYICONDATA	notIconData;
CString			DeviceList[4096];
int				DeviceCount = 0;
CString			ServerIp = "192.168.16.128";
int				ServerPort = 80;
string			DeviceLetter;

struct bfStruct
{
	BOOL is_list;
	char hBuf[POST_BUFFER_SIZE];
};

BOOL fnReadConfig(CString &server_ip, int &server_port);
BOOL fnGetList();
LRESULT CALLBACK fnEventHandler(HWND, UINT, WPARAM, LPARAM);
BOOL fnEventArrival(LPARAM lParam);
BOOL fnEventRemoveC(LPARAM lParam);
CString fnGetDeviceID(LPARAM dType);
BOOL fnVerifyID(CString DeviceId);
BOOL fnSendEvent(BOOL is_registered, CString DeviceID, CString Description);
BOOL fnVolumeLetter(DWORD unitmask, string &dev_letter);
CString ConvertToFormat(wchar_t *dv_name);
void TrayMessage(LPCWSTR StatusString, DWORD IconFlag);
CString PostRequest(CString IpAddress, int Port, CString HttpObj, CString hSContent, bfStruct &bf);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	HWND		hWnd;
	MSG			msg;
	WNDCLASS	wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = fnEventHandler;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = L"Class of USB Monitoring App";

	if (RegisterClass(&wndclass) == 0) exit(1);

	hWnd = CreateWindow(
		L"Class of USB Monitoring App",
		L"Режим коммерческой тайны", 
		WS_OVERLAPPEDWINDOW,
		GetSystemMetrics(SM_CXSCREEN) - 300,
		GetSystemMetrics(SM_CYSCREEN) - 150,
		250, 100,
		NULL, NULL, 
		hInstance, 
		NULL
	);

	notIconData.cbSize = sizeof(NOTIFYICONDATA);
	notIconData.hWnd = hWnd;
	notIconData.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRAYICON));
	notIconData.uID = 15;
	notIconData.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_INFO;
	notIconData.uCallbackMessage = WM_TRAY_NOTIFY;
	notIconData.uTimeout = 3000;
	wcscpy_s(notIconData.szTip, L"Контроль USB портов\0");
	Shell_NotifyIcon(NIM_ADD, &notIconData);

	ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
	NotificationFilter.dbcc_size = 255;// sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_USB_DEVICE;
	HDEVNOTIFY hDevNotify = RegisterDeviceNotification(hWnd, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);

	setlocale(LC_ALL, "Russian");

	fnReadConfig(ServerIp, ServerPort);
	fnGetList();

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (msg.wParam);
}


BOOL fnReadConfig(CString &server_ip, int &server_port)
{
	ifstream fconfig("config");
	if (fconfig.is_open())
	{
		char temp_ip[128];
		fconfig >> temp_ip;
		fconfig >> server_port;
		fconfig.close();
		
		CString convert(temp_ip);
		server_ip = convert;
		return TRUE;
	}
	return FALSE;
}

BOOL fnGetList()
{
	bfStruct bf;
	bf.is_list = TRUE;
	CString http_status = "";
	http_status = PostRequest(ServerIp, ServerPort, L"/get-list/", "", bf);

	if (http_status == HTTP_OK)
	{
		if (strlen(bf.hBuf) == 0)
		return TRUE;

		int i = 0;
		char *pNext = NULL;
		char *pIs = strtok_s(bf.hBuf, " ", &pNext);
		while (pIs != NULL)
		{
			DeviceList[i] = CString(pIs);
			pIs = strtok_s(NULL, " ", &pNext);
			i++;
		}
		DeviceCount = i;
		return TRUE;
	}
	return FALSE;
}

LRESULT CALLBACK fnEventHandler(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
		case WM_DEVICECHANGE:
		{
			switch (wParam)
			{
				case DBT_DEVICEARRIVAL:
				{
					fnEventArrival(lParam);
					break;
				}
				case DBT_DEVICEREMOVECOMPLETE:
				{
					fnEventRemoveC(lParam);
					break;
				}
			}
			break;
		}
		case WM_DESTROY:
			Shell_NotifyIcon(NIM_DELETE, &notIconData);
			PostQuitMessage(0);
			return 0;
		default:
			return DefWindowProc(hWnd, iMsg, wParam, lParam);
	}
}

BOOL fnEventArrival(LPARAM lParam)
{
	CString DeviceID = fnGetDeviceID(lParam);
	if (DeviceID != "")
	{
		if (fnVerifyID(DeviceID))
		{
			TrayMessage(L"Установлено зарегистрированное устройство", NIIF_INFO);
			fnSendEvent(TRUE, DeviceID, L"Установлено USB");
		}
		else
		{
			TrayMessage(L"Установлено незарегистрированное устройство", NIIF_ERROR);
			fnSendEvent(FALSE, DeviceID, L"Установлено USB");			

			CString Query = L"removedrive.exe \"" + DeviceID + L"\" -L";
			size_t convertedCharsw = 0;
			size_t wsLength = (Query.GetLength() + 1) * 2;
			char *charQuery = new char[wsLength];
			wcstombs_s(&convertedCharsw, charQuery, wsLength, Query, _TRUNCATE);
			WinExec(charQuery, SW_HIDE);
			delete[] charQuery;
		}
	}
	return TRUE;
}

BOOL fnEventRemoveC(LPARAM lParam)
{
	CString DeviceID = fnGetDeviceID(lParam);
	if (DeviceID != "")
	{
		if (fnVerifyID(DeviceID))
		{
			TrayMessage(L"Извлечено зарегистрированное устройство", NIIF_INFO);
			fnSendEvent(TRUE, DeviceID, L"Извлечено USB");
		}
		else
		{
			TrayMessage(L"Извлечено незарегистрированное устройство", NIIF_ERROR);
			fnSendEvent(FALSE, DeviceID, L"Извлечено USB");
		}
	}
	return TRUE;
}

CString fnGetDeviceID(LPARAM dType)
{
	PDEV_BROADCAST_DEVICEINTERFACE pInterface = (PDEV_BROADCAST_DEVICEINTERFACE)dType;
	if (pInterface->dbcc_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
	{
		return CString(ConvertToFormat(pInterface->dbcc_name));
	}
	return CString("");
}

BOOL fnVerifyID(CString DeviceId)
{
	if (DeviceCount == 0)
	return TRUE;
	
	for (int i = 0; i < DeviceCount; i++)
		if (DeviceId == DeviceList[i])
		return TRUE;
	return FALSE;
}

void TrayMessage(LPCWSTR StatusString, DWORD IconFlag)
{
	notIconData.dwInfoFlags = IconFlag;
	wcscpy_s(notIconData.szInfoTitle, L"Контроль USB");
	wcscpy_s(notIconData.szInfo, StatusString);
	Shell_NotifyIcon(NIM_MODIFY, &notIconData);
}

BOOL fnSendEvent(BOOL is_registered, CString DeviceID, CString Description)
{
	CString EventContent;
	CString UserName;
	CString ComputerName;
	DWORD uLength = 512;
	DWORD cLength = 512;
	wchar_t wUserName[512] = { '\0' };
	wchar_t wComputerName[512] = { '\0' };

	GetUserNameW(wUserName, &uLength);
	GetComputerNameW(wComputerName, &cLength);

	EventContent = L"username=" + CString(wUserName);
	EventContent = EventContent + L"&computername=" + CString(wComputerName);
	if (is_registered)
	EventContent += L"&isreg=Да";
	else
	EventContent += L"&isreg=Нет";

	int position = 0;
	while ((position = DeviceID.Find('&')) != -1)
	{
		DeviceID.Delete(position, 1);
		DeviceID.Insert(position, L"%26");
	}

	EventContent = EventContent + L"&device_id=" + DeviceID;
	EventContent = EventContent + L"&description=" + Description;
	
	bfStruct bf;
	bf.is_list = FALSE;
	PostRequest(ServerIp, ServerPort, L"/send-event/",EventContent, bf);
	return TRUE;
}

CString PostRequest(CString IpAddress, int Port, CString HttpObj, CString hSContent, bfStruct &bf)
{
	BOOL result = FALSE;
	DWORD sz;
	wchar_t status_code[16] = { '\0' };

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
				if (strlen(hContent) != 0)
					result = HttpSendRequest(hRequest, pHeader, wcslen(pHeader), static_cast<LPVOID>(hContent), strlen(hContent));
				else
					result = HttpSendRequest(hRequest, pHeader, wcslen(pHeader), NULL, 0);

				if (result)
				{
					HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE, static_cast<LPVOID>(status_code), &sz, NULL);
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
	return CString(status_code);
}

CString ConvertToFormat(wchar_t *dv_name)
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
	if (Convert[Convert.GetLength() - 1] == L'#')
		Convert.Delete(Convert.GetLength() - 1, 1);

	return Convert;
}
