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
#pragma comment(lib, "winmm.lib")

#define FPS 30 //定义FPS Timer作为屏幕刷新率定时器

//全局变量声明
static int InterFaceID = 1;						//当前界面ID号
static bool BACKSPACE = FALSE;					//判断BackSpace按键的布尔型变量
static bool ESCAPE = FALSE;						//判断Esc按键的布尔型变量
static int winwidth, winheight;					//窗口宽度和高度
static int countdown = 0;						//倒计时
static char name[21];							//姓名
bool Key_ctrl = FALSE, Another_Key = FALSE;		//快捷键状态
struct record_node
{
	int score;
	char name[21];
	struct record_node *next;
};


//文件操作模块
struct record_node *record_head;	//带头结点的数据链表 
void ReadRecording(void);			//读取函数
void PrintRanking(void);			//打印排名
void AddRanking(void);				//加入数据
void SaveRecording(void);			//存储数据
void DeleteCharacter(void);			//删除角色
void SaveGame(void);				//保存游戏状态
void DeleteSave(void);				//删除游戏保存存档

//0-进入画面
//1-主界面
//2-角色界面
//3-说明界面-2
//4-说明界面-3
//5-游戏界面
//6-说明界面
//7-排行榜界面
//8-退出游戏警告界面
//9-游戏菜单界面按钮

//界面函数声明
void EventProcess(void);			//界面切换函数
void Tetris_Enter(void);			//开始界面
void Tetris_Character(void);		//创造角色
void Tetris_MainMenu(void);			//主菜单
void Tetris_Instruction1(void);		//游戏说明
void Tetris_Instruction2(void);		//游戏说明
void Tetris_Instruction3(void);		//游戏说明
void Tetris_Ranking(void);			//排行榜系统
void Tetris_QuitAlert(void);		//退出警告系统
void GameMenu(void);				//游戏菜单界面
void ReadRecording(void);			//读取保存记录

//回调函数声明
void KeyboardEventProcess(int event, int key);
void TimerEventProcess(int TimerID);
void MouseEventProcess(int event, int button, int x, int y);
void CharEventProcess(char ch);

//具体游戏实现
void KeyboardEventProcess(int key, int event)
{
	//快捷键
	int Key_Kind = 0;
	if (key == VK_CONTROL && event == KEY_DOWN)
		Key_ctrl = TRUE;
	if (key == 'A' && event == KEY_DOWN) {
		Another_Key = TRUE;
		Key_Kind = 1;
	}
	if (key == 'R' && event == KEY_DOWN) {
		Another_Key = TRUE;
		Key_Kind = 2;
	}
	if (key == 'I' && event == KEY_DOWN) {
		Another_Key = TRUE;
		Key_Kind = 3;
	}
	if (key == VK_CONTROL && event == KEY_UP)
		Key_ctrl = FALSE;

	if (Key_ctrl == TRUE && Another_Key == TRUE) {
		switch (Key_Kind) {
		case 1:
			cancelTimer(1);
			StartGame();
			break;
		case 2:
			InterFaceID = 7;
			EventProcess();
			break;
		case 3:
			InterFaceID = 6;
			EventProcess();
			break;
		}
		Key_ctrl = FALSE;
		Another_Key = FALSE;
	}
	//
	switch (InterFaceID)
	{
	case 5:
		if (State.Pause == FALSE)
		{
			if (event == KEY_DOWN)
			{
				switch (key)
				{
				case VK_LEFT:
					if (LeftAble())
					{
						MoveLeft();
						DrawMap();
						DrawBrick();
						DrawInfo();
					}
					break;
				case VK_RIGHT:
					if (RightAble())
					{
						MoveRight();
						DrawMap();
						DrawBrick();
						DrawInfo();
					}
					break;
				case VK_UP:
					MoveRotate();
					DrawMap();
					DrawBrick();
					DrawInfo();
					break;
				case VK_SPACE:
					MoveBottom();
					DrawMap();
					DrawBrick();
					DrawInfo();
					break;
				case VK_DOWN:
					if (DownAble())
					{
						MoveDown();
						DrawMap();
						DrawBrick();
						DrawInfo();
						State.score += State.level == 4 ? 1 : 0;
					}
					break;
				case VK_ESCAPE:
					if (State.Pause == FALSE)
					{
						InterFaceID = 9;
						EventProcess();
						cancelTimer(1);
					}
					break;
				}
			}
		}
		break;
	case 3:
		if (key == VK_DOWN && event == KEY_DOWN)
			InterFaceID = 4;
		if (key == VK_ESCAPE && event == KEY_DOWN)
			ESCAPE = TRUE;
		EventProcess();
		break;
	case 4:
		if (key == VK_DOWN && event == KEY_DOWN)
			InterFaceID = 6;
		if (key == VK_ESCAPE && event == KEY_DOWN)
			ESCAPE = TRUE;
		EventProcess();
		break;
	case 6:
		if (key == VK_DOWN && event == KEY_DOWN)
			InterFaceID = 3;
		if (key == VK_ESCAPE && event == KEY_DOWN)
			ESCAPE = TRUE;
		EventProcess();
		break;
	default:
		uiGetKeyboard(key, event);
		if (key == VK_ESCAPE && event == KEY_DOWN)
			ESCAPE = TRUE;
		EventProcess();
		break;
	}
}

void TimerEventProcess(int TimerID)
{
	switch (InterFaceID)
	{
	case 5:
		if(TimerID == 22)
			CountDown((countdown++)%3+1);
		if (State.Pause == FALSE)
		{
			switch (TimerID)
			{
			case 1:
				if (DownAble())
				{
					MoveDown();
					DrawMap();
					DrawBrick();
					DrawInfo();
				}
				else
				{
					if (VictoryJudge())
					{
						LoadBrick();
						cancelTimer(1);
						RemoveLine();
						DrawMap();
						DrawInfo();

						startTimer(1, 850 - State.score / (100 - 15 * State.level) * 20);
						CreateBrick();
					}
					else
					{
						cancelTimer(1);
						DisplayLose();
						AddRanking();
						
						Pause(1.5);
						InterFaceID = 1;
					}
				}
				break;
			}
		}
		break;

	default:
		switch (TimerID)
		{
		case FPS:
			switch (InterFaceID)
			{
			case 3:
				DisplayInstuction2();
				break;
			case 4:
				DisplayInstuction3();
				break;
			case 6:
				//PlaySound("bgm03.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
				//在这里放播放会卡顿，而且需要单击才能继续
				DisplayInstuction1();
				break;
			default:
				DisplayClear();
				break;
			}
			EventProcess();
		}
		break;
	}
}

void MouseEventProcess(int x, int y, int button, int event)
{
	switch (InterFaceID)
	{
	case 5:
		break;
	default:
		uiGetMouse(x, y, button, event); 
		EventProcess();
		break;
	}

}

void CharEventProcess(char ch)
{
	switch (InterFaceID)
	{
	case 5:
		switch (ch)
		{
		case 'p':
			if (State.Pause == FALSE)
				StartPause();
			else
				EndPause();
			break;
		}
		break;
	default:
		uiGetChar(ch); EventProcess();
		break;
	}
}

void Main(void)
{
	SetWindowSize(720, 720);
	SetWindowTitle("Deep Dark Tetris");
	InitGraphics();
	PlaySound(NULL, NULL, SND_FILENAME);
	PlaySound("bgm01.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	//在第一次启动游戏时播放bgm01
	

	winwidth = GetWindowWidth();
	winheight = GetWindowHeight();


	registerKeyboardEvent(KeyboardEventProcess);
	registerTimerEvent(TimerEventProcess);
	registerMouseEvent(MouseEventProcess);
	registerCharEvent(CharEventProcess);

	startTimer(FPS, 50);

	ReadRecording();
}

void EventProcess(void)
{
	switch (InterFaceID)
	{
	case 0:
		break;
	case 1:
		//PlaySound("bgm01.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		Tetris_MainMenu();
		//加入在这里让上面的音乐函数生效，则在最开始打开游戏的bgm不会响，从quitalert返回主界面的
		//bgm也不会响
		//在这里我得出一个小的结论就是音乐函数必须在DisplayClear（）之前使用。

		break;
	case 2:
		Tetris_Character();
		break;
	case 3:
		DisplayInstuction2();
		Tetris_Instruction2();
		break;
	case 4:
		DisplayInstuction3();
		Tetris_Instruction3();
		break;
	case 6:
		DisplayInstuction1();
		Tetris_Instruction1();
		break;
	case 7:
		DisplayClear();
		Tetris_Ranking();
		break;
	case 8:
		//PlaySound(NULL, NULL, SND_FILENAME);
		//PlaySound("bgm01.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		//PlaySound(NULL, NULL, SND_FILENAME);
		Tetris_QuitAlert();
		break;
	case 9:
		GameMenu();
		break;
	}
}

void Tetris_Enter(void)
{

}

void Tetris_MainMenu()
{
	double fH = GetFontHeight();
	double h = fH * 2;
	double x = winwidth / 2.5;
	double y = winheight / 1.5 - h;
	double w = winwidth / 4.5;
	if (button(1, x, y - 1.5 * h, w, h, "Start", "Start Game"))
	{
		DisplayClear();
		InterFaceID = 2; 
		EventProcess();
	}

	if (button(GenUIID(0), x, y - 3 * h, w, h, "Continue", "Start The Last Saved Game"))
	{
		FILE * fp;
		fp = fopen("Save.txt", "r");
		char FLAG[10];
		fscanf(fp, "%s", FLAG);
		fclose(fp);
		if (strcmp("Save", FLAG)==0)
		{
			PlaySound("gamebgm01.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			DisplayClear();
			InterFaceID = 5;
			Continue();
		}
	}

	if (button(GenUIID(0), x, y - 4.5 * h, w, h, "Ranking", "Show Ranking"))
	{
		PlaySound("bgm02.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		DisplayClear();
		InterFaceID = 7; 
		EventProcess();
	}

	if (button(GenUIID(0), x, y - 6 * h, w, h, "Instruction", "Show Instruction"))
	{
		
		DisplayInstuction1();
		PlaySound("bgm03.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		DisplayClear();//特别注意这里，这里我进行了优化！！
		//如果这里不加入DisplayClear会大幅度降低画图的速度，占用内存导致音乐延迟播放
		InterFaceID = 6; 
		EventProcess();
	}

	if (button(GenUIID(0), x, y - 7.5 * h, w, h, "Exit", "\0"))
	{
		PlaySound("bgm04.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		DisplayClear();
		//PlaySound("bgm04.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		InterFaceID = 8; 
		EventProcess();
	}

	if (ESCAPE)
	{
		InterFaceID = 8;
		ESCAPE = FALSE;
		EventProcess();
	}
}

void Tetris_Character()
{
	double fH = GetFontHeight();
	double h = fH * 2;
	double x = winwidth / 2.5;
	double y = winheight / 1.5 - h;
	double w = winwidth / 4.5;

	if (textbox(GenUIID(0), x, y - 1.5 * h, w, h, name, 20))
	{
		EventProcess();
	}

	if (button(GenUIID(0), x, y - 3 * h, w, h, "Delete This Character", "Delete The Character Your Entered"))
	{
		DeleteCharacter();
		DisplayClear();
		EventProcess();
	}
	
	if (button(GenUIID(0), x-w+w*0.33, y - 4.5 * h, w/2.5, h, "蔡徐坤", "Difficulty : 蔡徐坤"))
	{
		if (strlen(name) > 0)
		{
			InterFaceID = 5;
			strcpy(State.name, name);
			State.level = 1;
			StartGame();
		}
	}

	if (button(GenUIID(0), x - w/2.5 + w * 0.33, y - 4.5 * h, w / 2.5, h, "Easy", "Difficulty : Easy"))
	{
		if (strlen(name) > 0)
		{
			InterFaceID = 5;
			strcpy(State.name, name);
			State.level = 2;
			StartGame();
		}
	}

	if (button(GenUIID(0), x + w/2.5 + w * 0.33, y - 4.5 * h, w / 2.5, h, "Medium", "Difficulty : Medium"))
	{
		if (strlen(name) > 0)
		{
			InterFaceID = 5;
			strcpy(State.name, name);
			State.level = 3;
			StartGame();
		}
	}

	if (button(GenUIID(0), x + w + w * 0.33, y - 4.5 * h, w / 2.5, h, "Hard", "Difficulty : Hard"))
	{
		if (strlen(name) > 0)
		{
			InterFaceID = 5;
			strcpy(State.name, name);
			State.level = 4;
			StartGame();
		}
	}

	if (button(GenUIID(0), x, y - 6 * h, w, h, "Back", "\0"))
	{
		DisplayClear();
		InterFaceID = 1; 
		EventProcess();
	}

	if (ESCAPE)
	{
		DisplayClear();
		InterFaceID = 1; 
		ESCAPE = FALSE; 
		EventProcess();
	}
}

void Tetris_Instruction1(void)
{
	
	double fH = GetFontHeight();
	double h = fH * 2;
	double x = winwidth / 2.5;
	double y = winheight / 1.5 - h;
	double w = winwidth / 4.5;
	//PlaySound("bgm03.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	if (button(GenUIID(0), x, y - 13 * h, w, h, "Next", "\0"))
	{
		//DisplayClear();
		//PlaySound("bgm03.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		//在此处0延时
		DisplayInstuction2();
		InterFaceID = 3;
		EventProcess();
	}

	if (ESCAPE)
	{
		PlaySound("bgm01.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		DisplayClear();
		InterFaceID = 1; 
		ESCAPE = FALSE; 
		EventProcess();
	}
}

void Tetris_Instruction2(void)
{
	double fH = GetFontHeight();
	double h = fH * 2;
	double x = winwidth / 2.5;
	double y = winheight / 1.5 - h;
	double w = winwidth / 4.5;

	if (button(GenUIID(0), x, y - 13 * h, w, h, "Next", "\0"))
	{
		DisplayInstuction3();
		InterFaceID = 4;
		EventProcess();
	}

	if (ESCAPE)
	{
		PlaySound("bgm01.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		DisplayClear();
		InterFaceID = 1;
		ESCAPE = FALSE;
		EventProcess();
	}
}

void Tetris_Instruction3(void)
{
	double fH = GetFontHeight();
	double h = fH * 2;
	double x = winwidth / 2.5;
	double y = winheight / 1.5 - h;
	double w = winwidth / 4.5;

	if (button(GenUIID(0), x, y - 13 * h, w, h, "Back", "\0"))
	{
		PlaySound("bgm01.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		DisplayClear();
		InterFaceID = 1;
		EventProcess();
	}

	if (ESCAPE)
	{
		PlaySound("bgm01.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		DisplayClear();
		InterFaceID = 1;
		ESCAPE = FALSE;
		EventProcess();
	}
}


void Tetris_Ranking(void)
{

	double fH = GetFontHeight();
	double h = fH * 2;
	double x = winwidth / 2.5;
	double y = winheight / 1.5 - h;
	double w = winwidth / 4.5;

	PrintRanking();

	if (button(GenUIID(0), x, y - 10 * h, w, h, "Back", "\0"))
	{
		PlaySound("bgm01.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		DisplayClear();
		InterFaceID = 1;
		EventProcess();
	}

	if (ESCAPE)
	{
		PlaySound("bgm01.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		DisplayClear();
		InterFaceID = 1; 
		ESCAPE = FALSE; 
		EventProcess();
	}
}

void Tetris_QuitAlert(void)
{
	//PlaySound("bgm04.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	//DisplayClear();
	//PlaySound("bgm01.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	DisplayQuitAlert();
	double fH = GetFontHeight();
	double h = fH * 2;
	double x = winwidth / 2;
	double y = winheight / 1.5 - h;
	double w = winwidth / 5;
	
	if (button(GenUIID(0), x - w / 3-27, y - 7.5 * h, w / 3, h, "No", "\0"))
	{
		//PlaySound("bgm01.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		//点击NO返回主界面时直接播放bgm01
		DisplayClear();
		PlaySound("bgm01.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		//在这里PlaySound无论放在DisplayClear前还是后都不影响音乐的播放
		InterFaceID = 1; 
		EventProcess();
	}

	if (ESCAPE)
	{
		DisplayClear();
		InterFaceID = 1; 
		ESCAPE = FALSE; 
		EventProcess();
	}

	if (button(GenUIID(0), x + w / 3-27, y - 7.5 * h, w / 3, h, "Yes", "\0"))
		exit(1);
}

void GameMenu(void)
{
	DisplayMenu();
	double fH = GetFontHeight();
	double h = fH * 2;
	double x = winwidth / 2.5;
	double y = winheight / 1.5 - h;
	double w = winwidth / 4.5;

	if (button(GenUIID(0), x, y - 4 * h, w, h, "Restart", "\0"))
	{
		InterFaceID = 5;
		StartGame();
	}

	if (button(GenUIID(0), x - w + w * 0.33, y - 6 * h, w / 2.5, h, "蔡徐坤", "Difficulty : 蔡徐坤"))
	{
		InterFaceID = 5;
		State.level = 1;
		EndPause();
	}

	if (button(GenUIID(0), x - w / 2.5 + w * 0.33, y - 6 * h, w / 2.5, h, "Easy", "Difficulty : Easy"))
	{
		InterFaceID = 5;
		State.level = 2;
		EndPause();
	}

	if (button(GenUIID(0), x + w / 2.5 + w * 0.33, y - 6 * h, w / 2.5, h, "Medium", "Difficulty : Medium"))
	{
		InterFaceID = 5;
		State.level = 3;
		EndPause();
	}

	if (button(GenUIID(0), x + w + w * 0.33, y - 6 * h, w / 2.5, h, "Hard", "Difficulty : Hard"))
	{
		InterFaceID = 5;
		State.level = 4;
		EndPause();
	}


	if (button(GenUIID(0), x, y - 8 * h, w, h, "Exit Without Save", "\0"))
	{
		InterFaceID = 1;
		PlaySound("bgm01.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		DisplayClear();
		EventProcess();
	}

	if (button(GenUIID(0), x, y - 10 * h, w, h, "Save And Quit", "\0"))
	{
		InterFaceID = 1;
		PlaySound("bgm01.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		DisplayClear();
		SaveGame();
		EventProcess();
	}

	if (button(GenUIID(0), x, y - 12 * h, w, h, "Back", "Back To Game"))
	{
		InterFaceID = 5;
		EndPause();
	}

	if (ESCAPE)
	{
		InterFaceID = 5;
		ESCAPE = FALSE;
		EndPause();
	}
}


void ReadRecording()
{
	FILE * fp;
	fp = fopen("Record.txt", "r");

	int size = sizeof(struct record_node);
	struct record_node *head = (struct record_node *)malloc(size);
	head->next = NULL;
	struct record_node *p = head;

	while (!feof(fp))
	{
		p->next = (struct record_node *)malloc(size);
		p = p->next;
		fscanf(fp, "%d", &p->score);
		fscanf(fp, "%s", p->name);

		p->next = NULL;
	}
	fclose(fp);
	record_head = head;
}

void PrintRanking()
{
	DisplayClear();
	double fH = GetFontHeight();
	double h = fH * 2;
	double x = winwidth / 9;
	double y = winheight/1.2;
	char tmp[10];
	int count = 1;

	struct record_node *p = record_head;
	while (p->next != NULL && p->next->score >0 && count <=20)
	{
		SetPenColor("White");
		if (count <= 10)
		{
			MovePen(x, y - count * h);
			memset(tmp, NULL, sizeof(char) * 10);
			sprintf(tmp, "%2d", count);
			DrawTextString(tmp);

			MovePen(x + 80, y - count * h);
			DrawTextString(p->next->name);

			MovePen(x + 200, y - count * h);
			memset(tmp, NULL, sizeof(char) * 10);
			sprintf(tmp, "%5d", p->next->score);
			DrawTextString(tmp);
		}
		else
		{
			MovePen(x*5, y - (count-10) * h);
			memset(tmp, NULL, sizeof(char) * 10);
			sprintf(tmp, "%2d", count);
			DrawTextString(tmp);

			MovePen(x*5 +80, y - (count - 10) * h);
			DrawTextString(p->next->name);

			MovePen(x*5 + 200, y - (count - 10) * h);
			memset(tmp, NULL, sizeof(char) * 10);
			sprintf(tmp, "%5d", p->next->score);
			DrawTextString(tmp);
		}

		count++;
		p = p->next;
	}

	if (count <= 10)
	{
		MovePen(x - 10, y);
		DrawTextString("Ranking");
		MovePen(x + 90, y);
		DrawTextString("Name");
		MovePen(x + 200, y);
		DrawTextString("Score");
	}
	else
	{
		MovePen(x - 10, y);
		DrawTextString("Ranking");
		MovePen(x + 90, y);
		DrawTextString("Name");
		MovePen(x + 200, y);
		DrawTextString("Score");
		MovePen(x * 5 - 10, y);
		DrawTextString("Ranking");
		MovePen(x * 5 + 90, y);
		DrawTextString("Name");
		MovePen(x * 5 + 200, y);
		DrawTextString("Score");
	}
}

void AddRanking()
{
	int size = sizeof(struct record_node);
	struct record_node *p = record_head;
	while (p->next != NULL)
	{
		if (p->next->score < State.score)
			break;
		p=p->next;
	}

	struct record_node *q = (struct record_node *)malloc(size);
	q->next = p->next;
	q->score = State.score;
	strcpy(q->name, State.name);
	p->next = q;

	SaveRecording();
	DeleteSave();
}

void SaveRecording()
{
	FILE * fp;
	fp = fopen("Record.txt", "w");

	int size = sizeof(struct record_node);
	struct record_node *p = record_head;

	while (p->next != NULL)
	{
		if (p->next->score >= -100)
			fprintf(fp, "%d %s\n", p->next->score, p->next->name);
		p = p->next;
	}
	fclose(fp);
}

void DeleteCharacter()
{
	int size = sizeof(struct record_node);
	struct record_node *p = record_head, *q;

	while (p->next != NULL)
	{
		if (strcmp(p->next->name, name)==0)
		{
			q = p->next;
			p->next = p->next->next;
			free(q);
			if (p->next == NULL)
				break;
		}
		else
		{
			p = p->next;
		}
	}

	SaveRecording();
}

void SaveGame()
{
	FILE * fp;
	fp = fopen("Save.txt", "w");

	fprintf(fp, "Save\n");
	int i, j;
	for (i = 0; i <= 3; i++)
	{
		for (j = 0; j <= 3; j++)
			fprintf(fp, "%d ", CurrentBrick.Brick[i][j]);
		fprintf(fp, "\n");
	}

	for (i = 0; i <= 3; i++)
	{
		for (j = 0; j <= 3; j++)
			fprintf(fp, "%d ", NextBrick.Brick[i][j]);
		fprintf(fp, "\n");
	}


	fprintf(fp, "%d,%d\n", CurrentBrick.x, CurrentBrick.y);
	fprintf(fp, "%d\n", State.score);
	fprintf(fp, "%s\n", State.name);
	fprintf(fp, "%d\n", State.level);

	for (i = 0; i <= 19; i++)
	{
		for (j = 0; j <= 9; j++)
			fprintf(fp, "%d ", Map[i][j]);
		fprintf(fp, "\n");
	}
	fclose(fp);
}

void DeleteSave()
{
	FILE * fp;
	fp = fopen("Save.txt","w");

	fprintf(fp,"NoSave\n");
	fclose(fp);
}

