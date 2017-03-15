#pragma once
#include <windows.h>
#include <math.h>

#define PI 3.14159265358979323846

class Forward
{
public:
	POINT CurrentObject;	// текущие координаты центра круга
	POINT PreviousObject;	// предыдущие координаты центра круга
	POINT CurrentLine;		// координаты текущего направления
	POINT ReferenceLine;	// координаты предыдущего направлления

	UINT R;					// радиус круга
	UINT STEP;				// величина шага
	INT ANGLE;				// угол текущего поворота
	INT REFERENCE_ANGLE;	// угол предыдущего направления

	RECT WORKSPACE;			// границы рабочей области

	Forward();
	~Forward();

	VOID GoForward();
	VOID GoBackward();
	VOID RotateLeft();
	VOID RotateRight();

protected:
	BOOL InWorkspace(UINT X, UINT Y);
	VOID LineCoordinates();	// пересчет координат направляющих линий
};
