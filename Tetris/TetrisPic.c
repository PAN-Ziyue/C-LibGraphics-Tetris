#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include "TetrisPic.h"


//»­¿Õ·½¿é
void DrawBrick0(int x, int y)
{
	MovePen(36 * (x + 1), 720 - 36 * (y+1));
	SetPenColor("Light Gray");
	{
		DrawLine(0, PtoI(36));
		DrawLine(PtoI(36), 0);
		DrawLine(0, -PtoI(36));
		DrawLine(-PtoI(36), 0);
	}
}

void DrawBrick1(int x, int y)
{
	MovePen(36 * (x + 1), 720 - 36 * (y + 1));
	SetPenColor("Blue");
	StartFilledRegion(1);
	{
		DrawLine(0, PtoI(36));
		DrawLine(PtoI(36), 0);
		DrawLine(0, -PtoI(36));
		DrawLine(-PtoI(36), 0);
	}
	EndFilledRegion();
	MovePen(36 * (x + 1), 720 - 36 * (y + 1));
	SetPenColor("Light Gray");
	{
		DrawLine(0, PtoI(36));
		DrawLine(PtoI(36), 0);
		DrawLine(0, -PtoI(36));
		DrawLine(-PtoI(36), 0);
	}
}

void DrawBrick2(int x, int y)
{
	MovePen(36 * (x + 1), 720 - 36 * (y + 1));
	SetPenColor("Cyan");
	StartFilledRegion(1);
	{
		DrawLine(0, PtoI(36));
		DrawLine(PtoI(36), 0);
		DrawLine(0, -PtoI(36));
		DrawLine(-PtoI(36), 0);
	}
	EndFilledRegion();
	MovePen(36 * (x + 1), 720 - 36 * (y + 1));
	SetPenColor("Light Gray");
	{
		DrawLine(0, PtoI(36));
		DrawLine(PtoI(36), 0);
		DrawLine(0, -PtoI(36));
		DrawLine(-PtoI(36), 0);
	}
}

void DrawBrick3(int x, int y)
{
	MovePen(36 * (x + 1), 720 - 36 * (y + 1));
	SetPenColor("Red");
	StartFilledRegion(1);
	{
		DrawLine(0, PtoI(36));
		DrawLine(PtoI(36), 0);
		DrawLine(0, -PtoI(36));
		DrawLine(-PtoI(36), 0);
	}
	EndFilledRegion();
	MovePen(36 * (x + 1), 720 - 36 * (y + 1));
	SetPenColor("Light Gray");
	{
		DrawLine(0, PtoI(36));
		DrawLine(PtoI(36), 0);
		DrawLine(0, -PtoI(36));
		DrawLine(-PtoI(36), 0);
	}
}

void DrawBrick4(int x, int y)
{
	MovePen(36 * (x + 1), 720 - 36 * (y + 1));
	SetPenColor("Yellow");
	StartFilledRegion(1);
	{
		DrawLine(0, PtoI(36));
		DrawLine(PtoI(36), 0);
		DrawLine(0, -PtoI(36));
		DrawLine(-PtoI(36), 0);
	}
	EndFilledRegion();
	MovePen(36 * (x + 1), 720 - 36 * (y + 1));
	SetPenColor("Light Gray");
	{
		DrawLine(0, PtoI(36));
		DrawLine(PtoI(36), 0);
		DrawLine(0, -PtoI(36));
		DrawLine(-PtoI(36), 0);
	}
}

void DrawBrick5(int x, int y)
{
	MovePen(36 * (x + 1), 720 - 36 * (y + 1));
	SetPenColor("Brown");
	StartFilledRegion(1);
	{
		DrawLine(0, PtoI(36));
		DrawLine(PtoI(36), 0);
		DrawLine(0, -PtoI(36));
		DrawLine(-PtoI(36), 0);
	}
	EndFilledRegion();
	MovePen(36 * (x + 1), 720 - 36 * (y + 1));
	SetPenColor("Light Gray");
	{
		DrawLine(0, PtoI(36));
		DrawLine(PtoI(36), 0);
		DrawLine(0, -PtoI(36));
		DrawLine(-PtoI(36), 0);
	}
}

void DrawBrick6(int x, int y)
{
	MovePen(36 * (x + 1), 720 - 36 * (y + 1));
	SetPenColor("Orange");
	StartFilledRegion(1);
	{
		DrawLine(0, PtoI(36));
		DrawLine(PtoI(36), 0);
		DrawLine(0, -PtoI(36));
		DrawLine(-PtoI(36), 0);
	}
	EndFilledRegion();
	MovePen(36 * (x + 1), 720 - 36 * (y + 1));
	SetPenColor("Light Gray");
	{
		DrawLine(0, PtoI(36));
		DrawLine(PtoI(36), 0);
		DrawLine(0, -PtoI(36));
		DrawLine(-PtoI(36), 0);
	}
}

void DrawBrick7(int x, int y)
{
	MovePen(36 * (x + 1), 720 - 36 * (y + 1));
	SetPenColor("Green");
	StartFilledRegion(1);
	{
		DrawLine(0, PtoI(36));
		DrawLine(PtoI(36), 0);
		DrawLine(0, -PtoI(36));
		DrawLine(-PtoI(36), 0);
	}
	EndFilledRegion();
	MovePen(36 * (x + 1), 720 - 36 * (y + 1));
	SetPenColor("Light Gray");
	{
		DrawLine(0, PtoI(36));
		DrawLine(PtoI(36), 0);
		DrawLine(0, -PtoI(36));
		DrawLine(-PtoI(36), 0);
	}
}


