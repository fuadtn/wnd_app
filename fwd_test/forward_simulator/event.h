#pragma once
#include "forward.h"
#include <string>

#define NONE ""
#define EVENT_HEIGHT 60
typedef std::string STRING;

class EventForward: public Forward
{
public:
	HWND hListBox;

	EventForward();
	~EventForward();

	VOID EventOptions(UINT MAP[10]);

protected:
	BOOL IsIntersection;
	BOOL IsWorkspace;
	BOOL IsUTurn;
	BOOL IsOpposite;
	UINT EventIncrement;
	UINT IntersectionRadius;
	UINT UTurnPosition;
	STRING EventMessage;

	STRING EventWorkspace();
	STRING EventIntersection(UINT MAP[10]);
	STRING EventOppositeLane(UINT MAP[10]);
	STRING EventUTurn(UINT MAP[10]);
	BOOL isLeftLane(UINT X);
};
