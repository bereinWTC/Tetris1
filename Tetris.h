#include <graphics.h>                            // ���������ֲTC��ͼ�ο⵽VC�����Ե�EasyX�Ĺ���������������
#include <conio.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>      
#pragma comment(lib,"Winmm.lib")                 // ����Winmm.lib
#define	WIDTH	10		                         // ��Ϸ�����
#define	HEIGHT	22	                             // ��Ϸ���߶�
#define	SIZE	20		                         // ÿ����Ϸ����λ��ʵ������


struct Pai
{
	char name[20];
	double grades;
};
typedef struct Pai Pai;

//��������ö�ٱ������ãͣĻ�ȡָ��ģң��׻�ȡ����״̬
enum CMD
{
	CMD_ROTATE,						// ������ת
	CMD_LEFT, CMD_RIGHT, CMD_DOWN,	// �������ҡ����ƶ�
	CMD_SINK,				        // �������
	CMD_QUIT,						// �˳���Ϸ
	CMD_STOP                        // ��ͣ
};
enum DRAW
{
	SHOW,	                       // ��ʾ����
	HIDE,	                       // ���ط���
	FIX		                       // �̶�����
};

//���巽��ṹ������ɫ
struct Block
{
	WORD dir[4];                //WORD������graphics.h�У���ʾһ��16λ���ݣ���һ����,������ʾ��״
	COLORREF  color;            //������ɫ
};
// ����һ����������ʾ���з��鼰����Ρ���ɫ
Block g_blocks[7] = { { 0x0F00, 0x4444, 0x0F00, 0x4444, RED },    // I
{ 0x0660, 0x0660, 0x0660, 0x0660, BLUE },		// ��
{ 0x4460, 0x02E0, 0x0622, 0x0740, MAGENTA },	// L
{ 0x2260, 0x0E20, 0x0644, 0x0470, YELLOW },	// ��L
{ 0x0C60, 0x2640, 0x0C60, 0x2640, CYAN },		// Z
{ 0x0360, 0x4620, 0x0360, 0x4620, GREEN },	// ��Z
{ 0x4E00, 0x4C40, 0x0E40, 0x4640, BROWN } };	// T
												// ���巽��任��Ϣ�ṹ
struct BlockInfo
{
	byte id;	// ���� ID
	char x, y;	// ��������Ϸ���е�����
	byte dir : 2;	// ����(����λ�����Ʊ�ʾ4�ַ���)
}	g_CurBlock, g_NextBlock;

// ������Ϸ��
BYTE game_area[WIDTH][HEIGHT] = { 0 };



void WelcomeMenu();                                     // ��Ϸ�����˵�����
void Init();											// ��ʼ����Ϸ
void Quit();											// �˳���Ϸ
void NewGame();											// ��ʼ����Ϸ
void GameOver();										// ������Ϸ
CMD  GetCmd();											// ��ȡ��������
void DispatchCmd(CMD _cmd);								// �ַ���������
void NewBlock();										// �����µķ���
bool CheckBlock(BlockInfo _block);						// ���ָ�������Ƿ���Է���
void DrawBlock(BlockInfo _block, DRAW _draw = SHOW);	// ������
void OnRotate();										// ��ת����
void OnLeft();											// ���Ʒ���
void OnRight();											// ���Ʒ���
void OnDown();											// ���Ʒ���
void OnSink();                                          // �³�����
void writefile(Pai &P);                                 // д���û���Ϣ
int  readfile(Pai &P);                                  // ��ȡ��������а���Ϣ
void Paihang(Pai &P);                                   // ����ҷ�����������
void ScoreTop();                                        // �÷����а�
void showScore();                                       // ��ʾ����
void showLevel();                                       // ��ʾ�ȼ�
void DisplayPause();                                    // ��ͣ

