#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "USBDlg.h"
#include "cInfoDlg.h"
#include "cLoginDlg.h"
#include "cSettingsDlg.h"
#include "resource.h"
#include <string>
#include <windows.h>
#include <wininet.h>


#pragma comment(lib,"wininet")

#define HTTP_OK L"200"

class cUSBApp : public CWinApp
{
public:
	cUSBApp();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern cUSBApp theApp;