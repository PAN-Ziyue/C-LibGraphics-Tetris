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
#include <time.h>
#include "TetrisPic.h"
#include "TetrisGame.h"
#include "imgui.h"


//三、函数实现
//运动类
void MoveLeft()
{
	CurrentBrick.x--;
}

void MoveRight()
{
	CurrentBrick.x++;
}

void MoveDown()
{
	CurrentBrick.y++;
}

void MoveBottom()
{
	while (DownAble())
	{
		MoveDown();
		State.score += State.level == 4?1:0;
	}
}

void MoveRotate()
{
	TmpBrick = CurrentBrick;
	int tmp[4][4];
	int i, j;
	memcpy(tmp, TmpBrick.Brick, sizeof(int) * 16);

	//让TmpBrick右转
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			TmpBrick.Brick[j][3 - i] = tmp[i][j];

	if (CrashJudge())
		return;

	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
		{
			if (TmpBrick.Brick[i][j] && TmpBrick.x + j < 0)
				return;
		}

	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
		{
			if (TmpBrick.Brick[i][j] && TmpBrick.x + j > 9)
				return;
		}

	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
		{
			if (TmpBrick.Brick[i][j] && TmpBrick.y + j > 19)
				return;
		}

	CurrentBrick = TmpBrick;
}

//判断类
bool CrashJudge()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (TmpBrick.Brick[i][j] && Map[i + TmpBrick.y][j + TmpBrick.x])
				return TRUE;
	return FALSE;
}

bool DownAble()
{
	TmpBrick = CurrentBrick;
	int i, j;
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
		{
			if (TmpBrick.Brick[i][j] && TmpBrick.y + i == 19)
				return FALSE;
		}
	TmpBrick.y++;
	return !CrashJudge();
}

bool LeftAble()
{
	TmpBrick = CurrentBrick;
	int i, j;
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
		{
			if (TmpBrick.Brick[i][j] && TmpBrick.x + j == 0)
				return FALSE;
		}
	TmpBrick.x--;
	return !CrashJudge();
}

bool RightAble()
{
	TmpBrick = CurrentBrick;
	int i, j;
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
		{
			if (TmpBrick.Brick[i][j] && TmpBrick.x + j == 9)
				return FALSE;
		}
	TmpBrick.x++;
	return !CrashJudge();
}

bool VictoryJudge()
{
	int i;
	for (i = 0; i <= 9; i++)
		if (CurrentBrick.Brick[1][i] && Map[0][i + CurrentBrick.x])
			return FALSE;
	return TRUE;
}

bool FullLineJudge()
{
	bool FullAble = TRUE;
	int i;
	for (i = 0; i < 10; i++) {
		if (line[i] == 0) FullAble = FALSE;
	}
	return FullAble;
}

//事件类
void DrawMap(void)
{
	DisplayClear();
	int i, j;
	for (i = 0; i <= 9; i++)
		for (j = 0; j <= 19; j++)
		{

			switch (Map[j][i])
			{
			case 0:
				DrawBrick0(i, j);
				break;
			case 1:
				DrawBrick1(i, j);
				break;
			case 2:
				DrawBrick2(i, j);
				break;
			case 3:
				DrawBrick3(i, j);
				break;
			case 4:
				DrawBrick4(i, j);
				break;
			case 5:
				DrawBrick5(i, j);
				break;
			case 6:
				DrawBrick6(i, j);
				break;
			case 7:
				DrawBrick7(i, j);
				break;
			}
		}
}

void DrawBrick()
{
	int i, j;
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
		{
			switch (CurrentBrick.Brick[j][i])
			{
			case 1:
				DrawBrick1(i + CurrentBrick.x, j + CurrentBrick.y);
				break;
			case 2:
				DrawBrick2(i + CurrentBrick.x, j + CurrentBrick.y);
				break;
			case 3:
				DrawBrick3(i + CurrentBrick.x, j + CurrentBrick.y);
				break;
			case 4:
				DrawBrick4(i + CurrentBrick.x, j + CurrentBrick.y);
				break;
			case 5:
				DrawBrick5(i + CurrentBrick.x, j + CurrentBrick.y);
				break;
			case 6:
				DrawBrick6(i + CurrentBrick.x, j + CurrentBrick.y);
				break;
			case 7:
				DrawBrick7(i + CurrentBrick.x, j + CurrentBrick.y);
				break;
			}
		}
}

void DrawInfo()
{
	int i, j;
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
		{
			switch (NextBrick.Brick[j][i])
			{
			case 1:
				DrawBrick1(i + 12, j + 3);
				break;
			case 2:
				DrawBrick2(i + 12, j + 3);
				break;
			case 3:
				DrawBrick3(i + 12, j + 3);
				break;
			case 4:
				DrawBrick4(i + 12, j + 3);
				break;
			case 5:
				DrawBrick5(i + 12, j + 3);
				break;
			case 6:
				DrawBrick6(i + 12, j + 3);
				break;
			case 7:
				DrawBrick7(i + 12, j + 3);
				break;
			}
		}
	MovePen(36 * 12, 720 - 36 * 9);
	char score[32];
	memset(score, '\0', sizeof(char) * 32);
	sprintf(score, "%d", State.score);
	SetPenColor("White");
	DrawTextString("Score:  ");
	DrawTextString(score);
	MovePen(36 * 12, 720 - 36 * 10.5);
	DrawTextString("Name:  ");
	DrawTextString(State.name);
}

void LoadBrick()
{
	int i, j;
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
			if (CurrentBrick.Brick[j][i])
				Map[j + CurrentBrick.y][i + CurrentBrick.x] = CurrentBrick.Brick[j][i];
}

void RemoveLine()
{
	int i, j, k, count = 0, Number_Of_Line[4] = { -1,-1,-1,-1 };
	for (i = 19, k = 0; i > 0; i--) {
		for (j = 0; j < 10; j++) {
			line[j] = Map[i][j];
		}
		if (FullLineJudge())
		{
			Number_Of_Line[k++] = i;
			count++;
		}
	}
	for (k = 3; k >= 0; k--) {
		if (Number_Of_Line[k] != -1) {
			for (i = Number_Of_Line[k]; i > 0; i--) {
				for (j = 0; j < 10; j++) {
					Map[i][j] = Map[i - 1][j];
				}
				for (j = 0; j < 10; j++) {
					Map[i - 1][j] = 0;
				}
			}
		}
	}
	switch (count)
	{
	case 1:
		State.score += 16 * (State.level + 2) / 5;
		break;
	case 2:
		State.score += 36 * (State.level + 2) / 5;
		break;
	case 3:
		State.score += 60 * (State.level + 2) / 5;
		break;
	case 4:
		State.score += 90 * (State.level + 2) / 5;
		break;
	}
}

void CreateBrick()
{
	CurrentBrick = NextBrick;
	memset(NextBrick.Brick, 0, sizeof(int) * 16);
	NextBrick.x = 3;
	NextBrick.y = -2;
	int RandomNumber = rand() % 7 + 1;//获得7个随机数
	switch (RandomNumber)
	{
	case 1:
		NextBrick.Brick[1][0] = 1; NextBrick.Brick[1][1] = 1;
		NextBrick.Brick[1][2] = 1; NextBrick.Brick[1][3] = 1;
		break;
	case 2:
		NextBrick.Brick[1][0] = 2;
		NextBrick.Brick[2][0] = 2; NextBrick.Brick[2][1] = 2; NextBrick.Brick[2][2] = 2;
		break;
	case 3:
		NextBrick.Brick[1][2] = 3;
		NextBrick.Brick[2][0] = 3; NextBrick.Brick[2][1] = 3; NextBrick.Brick[2][2] = 3;
		break;
	case 4:
		NextBrick.Brick[1][1] = 4; NextBrick.Brick[1][2] = 4;
		NextBrick.Brick[2][1] = 4; NextBrick.Brick[2][2] = 4;
		break;
	case 5:
		NextBrick.Brick[1][1] = 5; NextBrick.Brick[1][2] = 5;
		NextBrick.Brick[2][0] = 5; NextBrick.Brick[2][1] = 5;
		break;
	case 6:
		NextBrick.Brick[1][1] = 6;
		NextBrick.Brick[2][0] = 6; NextBrick.Brick[2][1] = 6; NextBrick.Brick[2][2] = 6;
		break;
	case 7:
		NextBrick.Brick[1][0] = 7; NextBrick.Brick[1][1] = 7;
		NextBrick.Brick[2][1] = 7; NextBrick.Brick[2][2] = 7;
		break;
	}
}

void StartGame()
{
	//游戏正式开始时播放gamebgm01
	PlaySound(NULL, NULL, SND_FILENAME);
	PlaySound("gamebgm02.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	Randomize();
	memset(Map, 0, sizeof(int) * 200);

	memset(NextBrick.Brick, 0, sizeof(int) * 16);
	NextBrick.x = 3;
	NextBrick.y = -2;
	int RandomNumber = rand() % 7 + 1;//获得7个随机数
	switch (RandomNumber)
	{
	case 1:
		NextBrick.Brick[1][0] = 1; NextBrick.Brick[1][1] = 1;
		NextBrick.Brick[1][2] = 1; NextBrick.Brick[1][3] = 1;
		break;
	case 2:
		NextBrick.Brick[1][0] = 2;
		NextBrick.Brick[2][0] = 2; NextBrick.Brick[2][1] = 2; NextBrick.Brick[2][2] = 2;
		break;
	case 3:
		NextBrick.Brick[1][2] = 3;
		NextBrick.Brick[2][0] = 3; NextBrick.Brick[2][1] = 3; NextBrick.Brick[2][2] = 3;
		break;
	case 4:
		NextBrick.Brick[1][1] = 4; NextBrick.Brick[1][2] = 4;
		NextBrick.Brick[2][1] = 4; NextBrick.Brick[2][2] = 4;
		break;
	case 5:
		NextBrick.Brick[1][1] = 5; NextBrick.Brick[1][2] = 5;
		NextBrick.Brick[2][0] = 5; NextBrick.Brick[2][1] = 5;
		break;
	case 6:
		NextBrick.Brick[1][1] = 6;
		NextBrick.Brick[2][0] = 6; NextBrick.Brick[2][1] = 6; NextBrick.Brick[2][2] = 6;
		break;
	case 7:
		NextBrick.Brick[1][0] = 7; NextBrick.Brick[1][1] = 7;
		NextBrick.Brick[2][1] = 7; NextBrick.Brick[2][2] = 7;
		break;
	}

	State.score = 0;
	CreateBrick();
	State.Pause = FALSE;
	State.Live = TRUE;
	startTimer(22, 1000);
}

void StartPause()
{
	DisplayPause();
	State.Pause = TRUE;
	cancelTimer(1);
}

void EndPause()
{
	DisplayClear();
	startTimer(22,1000);
}

void CountDown(int count)
{
	switch (count)
	{
	case 1:
		DisplayPause3();
		
		break;
	case 2:
		DisplayPause2();
		break;
	case 3:
		DisplayPause1();
		cancelTimer(22);
		State.Pause = FALSE;
		startTimer(1, 850 - State.score / (100-15*State.level)* 20);
		break;
	}
}

void Continue()
{
	FILE * fp;
	fp = fopen("Save.txt", "r");

	char tmp[8];
	fscanf(fp, "%s",tmp);
	int i, j;
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
			fscanf(fp, "%d", &CurrentBrick.Brick[i][j]);

	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
			fscanf(fp, "%d", &NextBrick.Brick[i][j]);

	fscanf(fp, "%d,%d", &CurrentBrick.x, &CurrentBrick.y);
	fscanf(fp, "%d", &State.score);
	fscanf(fp, "%s", State.name);
	fscanf(fp, "%d", &State.level);

	for (i = 0; i <= 19; i++)
		for (j = 0; j <= 9; j++)
			fscanf(fp, "%d", &Map[i][j]);

	fclose(fp);

	startTimer(22, 1000);
	State.Live = TRUE;
	State.Pause = TRUE;
	NextBrick.x = 3; NextBrick.y = -2;
}