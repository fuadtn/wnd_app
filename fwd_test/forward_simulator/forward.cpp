#include "forward.h"


Forward::Forward()
{
	// размеры рабочей области
	this->WORKSPACE.top = 10;
	this->WORKSPACE.bottom = 610;
	this->WORKSPACE.left = 290;
	this->WORKSPACE.right = 590;

	// начальные значения объекта
	this->R = 15;
	this->CurrentObject.x = this->WORKSPACE.right - (this->WORKSPACE.right - this->WORKSPACE.left) / 4;
	this->CurrentObject.y = this->WORKSPACE.bottom - 50;
	this->PreviousObject.x = this->CurrentObject.x;
	this->PreviousObject.y = this->CurrentObject.y;
	this->ANGLE = -90;
	this->REFERENCE_ANGLE = this->ANGLE;
	this->STEP = 5;
	this->LineCoordinates();
}


Forward::~Forward()
{
}


VOID Forward::GoForward()
{
	UINT NewX = this->CurrentObject.x + round(this->STEP * cos(this->ANGLE * PI / 180));
	UINT NewY = this->CurrentObject.y + round(this->STEP * sin(this->ANGLE * PI / 180));

	this->PreviousObject.x = this->CurrentObject.x;
	this->PreviousObject.y = this->CurrentObject.y;

	if (this->InWorkspace(NewX, NewY))
	{
		this->CurrentObject.x = NewX;
		this->CurrentObject.y = NewY;
		this->REFERENCE_ANGLE = this->ANGLE;
	}
	this->LineCoordinates();
}


VOID Forward::GoBackward()
{
	UINT NewX = this->CurrentObject.x - round(this->STEP * cos(this->ANGLE * PI / 180));
	UINT NewY = this->CurrentObject.y - round(this->STEP * sin(this->ANGLE * PI / 180));

	this->PreviousObject.x = this->CurrentObject.x;
	this->PreviousObject.y = this->CurrentObject.y;

		if (this->InWorkspace(NewX, NewY))
		{
			this->CurrentObject.x = NewX;
			this->CurrentObject.y = NewY;
			this->REFERENCE_ANGLE = this->ANGLE;
		}
		this->LineCoordinates();
}


VOID Forward::RotateLeft()
{
	this->ANGLE -= this->ANGLE <= this->REFERENCE_ANGLE - 90 ? 0 : 15;
	this->LineCoordinates();
}


VOID Forward::RotateRight()
{
	this->ANGLE += this->ANGLE >= this->REFERENCE_ANGLE + 90 ? 0 : 15;
	this->LineCoordinates();
}


VOID Forward::LineCoordinates()
{
	this->ReferenceLine.x = this->CurrentObject.x + round((this->R) * cos(this->REFERENCE_ANGLE * PI / 180));
	this->ReferenceLine.y = this->CurrentObject.y + round((this->R) * sin(this->REFERENCE_ANGLE * PI / 180));

	this->CurrentLine.x = this->CurrentObject.x + round(this->R * cos(this->ANGLE * PI / 180));
	this->CurrentLine.y = this->CurrentObject.y + round(this->R * sin(this->ANGLE * PI / 180));
}


BOOL Forward::InWorkspace(UINT X, UINT Y)
{
	UINT Radius = this->R > this->STEP ? this->R : this->STEP;
	if ((X - Radius) < this->WORKSPACE.left ||
		(X + Radius) > this->WORKSPACE.right ||
		(Y + Radius) > this->WORKSPACE.bottom ||
		(Y - Radius) < this->WORKSPACE.top)
	{
		return FALSE;
	}
	return TRUE;
}
