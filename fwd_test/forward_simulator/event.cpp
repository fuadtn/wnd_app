#include "event.h"


EventForward::EventForward()
{
	this->hListBox = NULL;
	this->EventMessage = "";
	this->IsWorkspace = TRUE;
	this->IsIntersection = FALSE;
	this->IntersectionRadius = 12;
	this->IsUTurn = FALSE;
	this->UTurnPosition = 0;
	this->IsOpposite = FALSE;
	this->EventIncrement = 10001;
}


EventForward::~EventForward()
{
	
}


STRING EventForward::EventWorkspace()
{
	if (this->CurrentObject.x == this->PreviousObject.x && this->CurrentObject.y == this->PreviousObject.y)
	{
		if (this->IsWorkspace)
		{
			this->IsWorkspace = FALSE;
			MessageBeep(MB_ICONERROR);
			return "Вы пытаетесь покинуть рабочую область";
		}
	}
	else this->IsWorkspace = TRUE;
	return NONE;
}


STRING EventForward::EventIntersection(UINT MAP[10])
{
	UINT MIDDLE = this->WORKSPACE.left + (this->WORKSPACE.right - this->WORKSPACE.left) / 2;
	UINT PositionY = ceil(this->CurrentObject.y - this->WORKSPACE.top) / EVENT_HEIGHT;

	if (abs((int)(MIDDLE - this->CurrentObject.x)) < this->IntersectionRadius)
	{
		if (!this->IsIntersection && MAP[PositionY] == 1)
		{
			this->IsIntersection = TRUE;
			MessageBeep(MB_ICONERROR);
			return "Вы пересекли сплошную линию";
		}
	}
	else this->IsIntersection = FALSE;
	return NONE;
}


STRING EventForward::EventUTurn(UINT MAP[10])
{
	if (this->isLeftLane(this->CurrentObject.x) ^ this->isLeftLane(this->PreviousObject.x)) // если полоса сменилась
	{
		this->UTurnPosition = ceil(this->CurrentObject.y - this->WORKSPACE.top) / EVENT_HEIGHT;

		if (MAP[this->UTurnPosition] == 0)
		this->IsUTurn = TRUE;
	}

	UINT TopY = this->WORKSPACE.top + this->UTurnPosition * EVENT_HEIGHT;
	UINT BottomY = TopY + EVENT_HEIGHT;

	if (this->IsUTurn &&
		(this->isLeftLane(this->CurrentObject.x) && this->CurrentObject.y < TopY ||
		!this->isLeftLane(this->CurrentObject.x) && this->CurrentObject.y > BottomY))
	{
		this->IsUTurn = FALSE;
		MessageBeep(MB_ICONERROR);
		return "Вы движетесь по встречной полосе";
	}
	else if (this->IsUTurn && 
		(this->isLeftLane(this->CurrentObject.x) && this->CurrentObject.y > BottomY ||
		!this->isLeftLane(this->CurrentObject.x) && this->CurrentObject.y < TopY))
	{
		this->IsUTurn = FALSE;
		MessageBeep(MB_OK);
		return "Вы совершили разворот";
	}
	return NONE;
}


STRING EventForward::EventOppositeLane(UINT MAP[10])
{
	if (this->isLeftLane(this->CurrentObject.x) ^ this->isLeftLane(this->PreviousObject.x))
	{
		this->UTurnPosition = ceil(this->CurrentObject.y - this->WORKSPACE.top) / EVENT_HEIGHT;

		if (MAP[this->UTurnPosition] == 0)
		{
			this->IsOpposite = TRUE;
			return NONE;
		}
		}
		if (this->isLeftLane(this->CurrentObject.x))
		{
			if (this->CurrentObject.y < this->PreviousObject.y)
			{
				if (!this->IsOpposite)
				{
					this->IsOpposite = TRUE;
					MessageBeep(MB_ICONERROR);
					return "Вы движетесь по встречной полосе";
				}
			}
			else this->IsOpposite = FALSE;
		}
		else
		{
			if (this->CurrentObject.y > this->PreviousObject.y)
			{
				if (!this->IsOpposite)
				{
					this->IsOpposite = TRUE;
					MessageBeep(MB_ICONERROR);
					return "Вы движетесь по встречной полосе";
				}
			}
			else this->IsOpposite = FALSE;
		}
	return NONE;
}

VOID EventForward::EventOptions(UINT MAP[10])
{
	STRING Message = NONE;

	if ((Message = this->EventWorkspace()) != NONE)
		SendMessage(this->hListBox, LB_INSERTSTRING, (WPARAM)0, (LPARAM)(std::to_string(this->EventIncrement++) + ": " + Message).c_str());
	if ((Message = this->EventIntersection(MAP)) != NONE)
		SendMessage(this->hListBox, LB_INSERTSTRING, (WPARAM)0, (LPARAM)(std::to_string(this->EventIncrement++) + ": " + Message).c_str());
	if ((Message = this->EventOppositeLane(MAP)) != NONE)
		SendMessage(this->hListBox, LB_INSERTSTRING, (WPARAM)0, (LPARAM)(std::to_string(this->EventIncrement++) + ": " + Message).c_str());
	if ((Message = this->EventUTurn(MAP)) != NONE)
		SendMessage(this->hListBox, LB_INSERTSTRING, (WPARAM)0, (LPARAM)(std::to_string(this->EventIncrement++) + ": " + Message).c_str());
}


BOOL EventForward::isLeftLane(UINT X)
{
	UINT MIDDLE = this->WORKSPACE.left + (this->WORKSPACE.right - this->WORKSPACE.left) / 2;
	if(X < MIDDLE)
	return TRUE;
	return FALSE;
}
