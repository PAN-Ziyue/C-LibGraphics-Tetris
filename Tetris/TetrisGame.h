#pragma once


//一、函数声明
//运动类
void MoveLeft(void);		//左移
void MoveRight(void);		//右移
void MoveDown(void);		//下移
void MoveBottom(void);		//直接移动至最底
void MoveRotate(void);		//旋转方块

//判断类
bool CrashJudge(void);		//是否有砖块重叠
bool FullLineJudge(void);	//是否有满行
bool DownAble(void);		//是否能下移
bool LeftAble(void);		//是否能左移
bool RightAble(void);		//是否能右移
bool VictoryJudge(void);	//判断是否存活

//事件类
void DrawMap(void);			//画出地图
void DrawBrick(void);		//画出砖块
void LoadBrick(void);		//砖块装载至地图
void RemoveLine(void);		//删除满行
void CreateBrick(void);		//创建新砖块
void DrawInfo(void);		//画出额外信息
void StartGame(void);		//开始游戏函数
void StartPause(void);		//打开暂停界面函数
void EndPause(void);		//结束暂停界面函数
void CountDown(int count);	//倒计时函数
void Continue(void);		//继续游戏函数

//二、变量声明
typedef struct
{
	int Brick[4][4];
	int x, y;
} TetrisBrick;

typedef struct
{
	bool Live;
	bool Pause;
	int score;
	char name[21];
	int level;
} GameInfo;

TetrisBrick CurrentBrick, NextBrick, TmpBrick;
//static int Map[20][10];
int Map[20][10];
static int line[10];

GameInfo State;