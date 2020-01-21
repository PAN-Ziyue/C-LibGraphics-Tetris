#pragma once


//һ����������
//�˶���
void MoveLeft(void);		//����
void MoveRight(void);		//����
void MoveDown(void);		//����
void MoveBottom(void);		//ֱ���ƶ������
void MoveRotate(void);		//��ת����

//�ж���
bool CrashJudge(void);		//�Ƿ���ש���ص�
bool FullLineJudge(void);	//�Ƿ�������
bool DownAble(void);		//�Ƿ�������
bool LeftAble(void);		//�Ƿ�������
bool RightAble(void);		//�Ƿ�������
bool VictoryJudge(void);	//�ж��Ƿ���

//�¼���
void DrawMap(void);			//������ͼ
void DrawBrick(void);		//����ש��
void LoadBrick(void);		//ש��װ������ͼ
void RemoveLine(void);		//ɾ������
void CreateBrick(void);		//������ש��
void DrawInfo(void);		//����������Ϣ
void StartGame(void);		//��ʼ��Ϸ����
void StartPause(void);		//����ͣ���溯��
void EndPause(void);		//������ͣ���溯��
void CountDown(int count);	//����ʱ����
void Continue(void);		//������Ϸ����

//������������
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