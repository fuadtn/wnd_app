#pragma once
#include <windows.h>
#include <math.h>

#define PI 3.14159265358979323846

class Forward
{
public:
	POINT CurrentObject;	// ������� ���������� ������ �����
	POINT PreviousObject;	// ���������� ���������� ������ �����
	POINT CurrentLine;		// ���������� �������� �����������
	POINT ReferenceLine;	// ���������� ����������� ������������

	UINT R;					// ������ �����
	UINT STEP;				// �������� ����
	INT ANGLE;				// ���� �������� ��������
	INT REFERENCE_ANGLE;	// ���� ����������� �����������

	RECT WORKSPACE;			// ������� ������� �������

	Forward();
	~Forward();

	VOID GoForward();
	VOID GoBackward();
	VOID RotateLeft();
	VOID RotateRight();

protected:
	BOOL InWorkspace(UINT X, UINT Y);
	VOID LineCoordinates();	// �������� ��������� ������������ �����
};
