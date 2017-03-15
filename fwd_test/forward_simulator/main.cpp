#include <windows.h>
#include <CommCtrl.h>
#include "event.h"
#include "draw.h"
#include "resource.h"

EventForward FW;
Draw DR;
HFONT hFont;

ATOM RegisterForwardClass(HINSTANCE hInstance);
HWND CreateForwardWindow(HINSTANCE hInstance);
HWND CreateForwardListBox();
LRESULT CALLBACK fnEventHandler(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK fnHelpHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK fnParamHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
VOID FwTextOut(HDC hDC, INT X, INT Y, STRING S)
{
	TextOut(hDC, X, Y, S.c_str(), S.length());
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	RegisterForwardClass(hInstance);
	if (!(DR.hWND = CreateForwardWindow(hInstance)))
	{
		MessageBox(NULL, "Ошибка создания окна", "Ошибка", MB_ICONERROR);
		return -1;
	}

	FW.hListBox = CreateForwardListBox();
	DR.hDC = GetDC(DR.hWND);
	DR.Prepare(FW.WORKSPACE);
	DR.DrawMap(FW.WORKSPACE);
	DR.DrawObject(FW.CurrentObject, FW.CurrentLine, FW.ReferenceLine, FW.R);

	SelectObject(DR.hDC, hFont);
	SetBkMode(DR.hDC, TRANSPARENT);
	SetTextColor(DR.hDC, RGB(170, 170, 170));
	TextOut(DR.hDC, 85, 535, "© Фуад Нуриев", 13);
	TextOut(DR.hDC, 45, 555, "Forward. Тестовая программа", 27);
	TextOut(DR.hDC, 120, 575, "2017", 4);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (msg.wParam);
}


ATOM RegisterForwardClass(HINSTANCE hInstance)
{
	WNDCLASS	wndclass;
	HBRUSH FillWindow = CreateSolidBrush(RGB(240, 240, 240));

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = fnEventHandler;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FWICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) FillWindow;
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_FORWARDMENU);
	wndclass.lpszClassName = "Forward Class";

	return RegisterClass(&wndclass);
}

HWND CreateForwardWindow(HINSTANCE hInstance)
{
	HDC ScreenDC = GetDC(NULL);
	INT ScreenWidth = GetDeviceCaps(ScreenDC, HORZRES);

	HWND hWnd = CreateWindow(
		"Forward Class",
		"Forward",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		ScreenWidth - 639, 20,	// x, y
		619, 680,				// w, h
		NULL, NULL,
		hInstance,
		NULL
	);

	if(!hWnd) return NULL;

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);
	return hWnd;
}

HWND CreateForwardListBox()
{
	HWND hWndList = CreateWindow(
		WC_LISTBOX,
		NULL, 
		WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL,
		10, 10,
		260, 500,
		DR.hWND,
		NULL, GetModuleHandle(NULL), NULL
	);

	if (!hWndList) return NULL;

	hFont = CreateFont(
		12, 0, 0, 0, FW_NORMAL,
		FALSE, FALSE, FALSE,
		RUSSIAN_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"MS Shell Dlg"
	);

	SendMessage(hWndList, WM_SETFONT, (WPARAM)hFont, (LPARAM)0);
	return hWndList;
}

LRESULT CALLBACK fnEventHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_LBUTTONDOWN:
		SetFocus(DR.hWND);
		break;
	case WM_MOVE:
		DR.Prepare(FW.WORKSPACE);
		DR.DrawMap(FW.WORKSPACE);
		DR.DrawObject(FW.CurrentObject, FW.CurrentLine, FW.ReferenceLine, FW.R);
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_PARAMETERS:
			EnableWindow(hWnd, FALSE);
			CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_PARAMDLG), hWnd, fnParamHandler);
			
			break;
		case ID_HELP:
			EnableWindow(hWnd, FALSE);
			CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_HELPDLG), hWnd, fnHelpHandler);
			break;
		case ID_QUIT:
			PostQuitMessage(0);
			break;
		}
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_UP:
			FW.GoForward();
			FW.EventOptions(DR.MAP);
			DR.DrawMap(FW.WORKSPACE);
			DR.DrawObject(FW.CurrentObject, FW.CurrentLine, FW.ReferenceLine, FW.R);
			break;
		case VK_LEFT:
			FW.RotateLeft();
			DR.DrawMap(FW.WORKSPACE);
			DR.DrawObject(FW.CurrentObject, FW.CurrentLine, FW.ReferenceLine, FW.R);
			break;
		case VK_RIGHT:
			FW.RotateRight();
			DR.DrawMap(FW.WORKSPACE);
			DR.DrawObject(FW.CurrentObject, FW.CurrentLine, FW.ReferenceLine, FW.R);
			break;
		case VK_DOWN:
			FW.GoBackward();
			FW.EventOptions(DR.MAP);
			DR.DrawMap(FW.WORKSPACE);
			DR.DrawObject(FW.CurrentObject, FW.CurrentLine, FW.ReferenceLine, FW.R);
			break;
		}
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return 0;
}

BOOL CALLBACK fnHelpHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_PAINT:
	{
		HBITMAP hBitMap;
		BITMAP BM;
		PAINTSTRUCT PS;

		HDC hDC = GetDC(hWnd);
		HDC hDCC = CreateCompatibleDC(hDC);

		hBitMap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BMP_HELP));
		HWND hPic = GetDlgItem(hWnd, IDC_PICTURE1);
		HGDIOBJ hObj = SelectObject(hDCC, hBitMap);
		GetObject(hBitMap, sizeof(BM), &BM);
		BitBlt(hDC, 10, 7, BM.bmWidth, BM.bmHeight, hDCC, 0, 0, SRCCOPY);
		SelectObject(hDCC, hObj);

		SelectObject(hDC, hFont);
		SetBkMode(hDC, TRANSPARENT);
		SetTextColor(hDC, RGB(100, 100, 100));

		FwTextOut(hDC, 120, 20, "Упрвление объектом осуществляется с помощью клавиш перемещения:");
		FwTextOut(hDC, 120, 35, "вперед, назад, вправо, влево.");

		FwTextOut(hDC, 120, 145, "Направляющая белого цвета показывает текущий угол поворота, серого - предыдущий.");
		FwTextOut(hDC, 120, 160, "Максимальный угол поворота в пределах одного шага составляет 90 градусов");
		FwTextOut(hDC, 120, 175, "относительно последнего направлления.");

		FwTextOut(hDC, 120, 275, "Пункт меню «Параметры» позволяет изменить характер сплошной разделительной линии.");
		FwTextOut(hDC, 120, 290, "Линия представляет собой совокупность отрезков [10 х 60px].");
		FwTextOut(hDC, 120, 305, "Единицей задается линия, нулем - разрыв.");
		FwTextOut(hDC, 120, 320, "Два и более разрывов подряд располагаться не могут.");

		FwTextOut(hDC, 120, 405, "Также «Параметры» позволяют установить длину шага объекта в пикселях.");
		FwTextOut(hDC, 120, 420, "в диапазоне [1,20].");

		FwTextOut(hDC, 120, 530, "Для снятия фокусировки с элемента ListBox щелкните ЛКМ в любой области");
		FwTextOut(hDC, 120, 545, "окна вне этого элемента.");

		DeleteDC(hDCC);
		ReleaseDC(hWnd, hDC);
	}
	case WM_COMMAND:
	{
		if(LOWORD(wParam) == ID_HELPCLOSE)
		{
			EnableWindow(DR.hWND, TRUE);
			DestroyWindow(hWnd);
		}
		break;
	}
	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return FALSE;
}

BOOL CALLBACK fnParamHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	STRING EditString = NONE;

	switch (Msg)
	{
	case WM_INITDIALOG:
	{
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MAP), EM_LIMITTEXT, (WPARAM)10, 0);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_STEP), EM_LIMITTEXT, (WPARAM)2, 0);

		EditString = NONE;
		for (UINT i = 0; i < 10; i++)
		EditString += std::to_string(DR.MAP[i]);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MAP), WM_SETTEXT, 0, (LPARAM)EditString.c_str());

		EditString = std::to_string(FW.STEP);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_STEP), WM_SETTEXT, 0, (LPARAM)EditString.c_str());
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BTN_APPLY:
		{
			EditString = NONE;
			char temp_char[16] = NONE;
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MAP), WM_GETTEXT, (WPARAM)16, (LPARAM)temp_char);

			EditString.append(temp_char);
			BOOL KEY = TRUE;
			if (EditString.length() == 10)
			{
				for (UINT i = 0; i < 10; i++)
					if (EditString[i] != '0' && EditString[i] != '1')
					KEY = FALSE;
				for (UINT i = 0; i < 9; i++)
					if (EditString[i] == '0' && EditString[i + 1] == '0') KEY = FALSE;
				if (KEY)
					for (UINT i = 0; i < 10; i++)
					DR.MAP[i] = (INT)EditString[i] - '0';
				DR.DrawMap(FW.WORKSPACE);
				DR.DrawObject(FW.CurrentObject, FW.CurrentLine, FW.ReferenceLine, FW.R);
			}

			SendMessage(GetDlgItem(hWnd, IDC_EDIT_STEP), WM_GETTEXT, (WPARAM)4, (LPARAM)temp_char);
			INT TEMP = atoi(temp_char);
			if (TEMP > 0 && TEMP < 21) FW.STEP = TEMP;

			EnableWindow(DR.hWND, TRUE);
			DestroyWindow(hWnd);
			break;
		}
		case ID_PARAMCLOSE:
			EnableWindow(DR.hWND, TRUE);
			DestroyWindow(hWnd);
			break;
		}
		break;
	}
	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return FALSE;
}
