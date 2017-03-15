#include "draw.h"

Draw::Draw()
{
	this->BBlack = CreateSolidBrush(RGB(0, 0, 0));
	this->BWhite = CreateSolidBrush(RGB(255, 255, 255));
	this->BGrey = CreateSolidBrush(RGB(100, 100, 100));
	this->BLGrey = CreateSolidBrush(RGB(250, 250, 250));
	this->BOrange = CreateSolidBrush(RGB(255, 165, 0));
	this->PWhite = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	this->PGrey = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
	this->PLGrey = CreatePen(PS_SOLID, 1, RGB(240, 240, 240));
	this->POrange = CreatePen(PS_SOLID, 1, RGB(255, 165, 0));
}


Draw::~Draw()
{
	DeleteObject(this->BBlack);
	DeleteObject(this->BWhite);
	DeleteObject(this->BGrey);
	DeleteObject(this->BGrey);
	DeleteObject(this->BOrange);
	DeleteObject(this->PWhite);
	DeleteObject(this->PLGrey);

	if(!this->hDC)
	ReleaseDC(this->hWND, this->hDC);
}


VOID Draw::Prepare(RECT RC)
{
	UINT BDR = 1;
	SelectObject(this->hDC, this->PGrey);
	SelectObject(this->hDC, this->BWhite);
	Rectangle(this->hDC, RC.left-BDR, RC.top-BDR, RC.right+BDR, RC.bottom+BDR);
}


VOID Draw::DrawObject(POINT CurrentObject, POINT CurrentLine, POINT ReferenceLine, UINT Radius)
{
	SelectObject(this->hDC, PWhite);
	SelectObject(this->hDC, BBlack);
	Ellipse(hDC, CurrentObject.x - Radius, CurrentObject.y + Radius, CurrentObject.x + Radius, CurrentObject.y - Radius);

	SelectObject(this->hDC, this->PGrey);
	MoveToEx(this->hDC, CurrentObject.x, CurrentObject.y, NULL);
	LineTo(hDC, ReferenceLine.x, ReferenceLine.y);

	SelectObject(this->hDC, PWhite);
	MoveToEx(this->hDC, CurrentObject.x, CurrentObject.y, NULL);
	LineTo(hDC, CurrentLine.x, CurrentLine.y);
}


VOID Draw::DrawMap(RECT RC)
{
	UINT INC = 0;
	UINT HEIGHT = 60;
	UINT iX = RC.left + (RC.right - RC.left) / 2;

	POINT POINTS[3];

	FillRect(this->hDC, &RC, this->BWhite);

	// стрелка направления левой полосы
	POINTS[0].x = RC.left + (RC.right - RC.left) / 4;
	POINTS[0].y = RC.top + 50;
	POINTS[1].x = RC.left + (RC.right - RC.left) / 4 - 10;
	POINTS[1].y = RC.top + 20;
	POINTS[2].x = RC.left + (RC.right - RC.left) / 4 + 10;
	POINTS[2].y = RC.top + 20;

	SelectObject(this->hDC, this->PWhite);
	SelectObject(this->hDC, this->BOrange);
	Polygon(this->hDC, POINTS, 3);

	// стрелка направления правой полосы
	POINTS[0].x = RC.left + 3 * (RC.right - RC.left) / 4;
	POINTS[0].y = RC.top + 20;
	POINTS[1].x = RC.left + 3 * (RC.right - RC.left) / 4 - 10;
	POINTS[1].y = RC.top + 50;
	POINTS[2].x = RC.left + 3 * (RC.right - RC.left) / 4 + 10;
	POINTS[2].y = RC.top + 50;

	SelectObject(this->hDC, this->PWhite);
	SelectObject(this->hDC, this->BOrange);
	Polygon(this->hDC, POINTS, 3);

	// рисование разделительной полосы
	for (UINT iY = RC.top; iY < RC.bottom; iY = iY + HEIGHT)
	{
		if (this->MAP[INC++] == 1)
		{
			SelectObject(this->hDC, this->POrange);
			MoveToEx(this->hDC, iX, iY, NULL);
			LineTo(hDC, iX, iY + HEIGHT);
		}
		else
		{
			UINT MIDDLE = RC.left + (RC.right - RC.left) / 2;
			SelectObject(this->hDC, this->PLGrey);
			SelectObject(this->hDC, this->BLGrey);
			
			if(INC != 1)
			Rectangle(this->hDC, RC.left, iY, MIDDLE, iY - 10);
			if (INC != 10)
			Rectangle(this->hDC, MIDDLE, iY + HEIGHT, RC.right, iY + HEIGHT + 10);
		}
	}
}
