
// it_e.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// Cit_App:
// О реализации данного класса см. it_e.cpp
//

class Cit_App : public CWinApp
{
public:
	Cit_App();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern Cit_App theApp;