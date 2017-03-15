#pragma once
#include <windows.h>

class Draw
{
public:
	HWND hWND;
	HDC hDC;

	UINT MAP[10] = { 0, 1, 1, 0, 1, 1, 1, 0, 1, 0 };

	HBRUSH BBlack;
	HBRUSH BWhite;
	HBRUSH BGrey;
	HBRUSH BLGrey;
	HBRUSH BOrange;

	HPEN PWhite;
	HPEN PGrey;
	HPEN PLGrey;
	HPEN POrange;

	Draw();
	~Draw();

	VOID Prepare(RECT RC);
	VOID DrawObject(POINT CurrentObject, POINT CurrentLine, POINT ReferenceLine, UINT Radius);
	VOID DrawMap(RECT RC);
};
