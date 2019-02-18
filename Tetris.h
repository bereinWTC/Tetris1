#include <graphics.h>                            // 这个库是移植TC的图形库到VC，可以到EasyX的官网下载这个程序包
#include <conio.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>      
#pragma comment(lib,"Winmm.lib")                 // 调用Winmm.lib
#define	WIDTH	10		                         // 游戏区宽度
#define	HEIGHT	22	                             // 游戏区高度
#define	SIZE	20		                         // 每个游戏区单位的实际像素


struct Pai
{
	char name[20];
	double grades;
};
typedef struct Pai Pai;

//定义两种枚举变量，ＣＭＤ获取指令，ＤＲＡＷ获取方块状态
enum CMD
{
	CMD_ROTATE,						// 方块旋转
	CMD_LEFT, CMD_RIGHT, CMD_DOWN,	// 方块左、右、下移动
	CMD_SINK,				        // 方块沉底
	CMD_QUIT,						// 退出游戏
	CMD_STOP                        // 暂停
};
enum DRAW
{
	SHOW,	                       // 显示方块
	HIDE,	                       // 隐藏方块
	FIX		                       // 固定方块
};

//定义方块结构及其颜色
struct Block
{
	WORD dir[4];                //WORD类型在graphics.h中，表示一个16位数据，即一个字,用来表示形状
	COLORREF  color;            //方块颜色
};
// 定义一个数组来表示所有方块及其变形、颜色
Block g_blocks[7] = { { 0x0F00, 0x4444, 0x0F00, 0x4444, RED },    // I
{ 0x0660, 0x0660, 0x0660, 0x0660, BLUE },		// 口
{ 0x4460, 0x02E0, 0x0622, 0x0740, MAGENTA },	// L
{ 0x2260, 0x0E20, 0x0644, 0x0470, YELLOW },	// 反L
{ 0x0C60, 0x2640, 0x0C60, 0x2640, CYAN },		// Z
{ 0x0360, 0x4620, 0x0360, 0x4620, GREEN },	// 反Z
{ 0x4E00, 0x4C40, 0x0E40, 0x4640, BROWN } };	// T
												// 定义方块变换信息结构
struct BlockInfo
{
	byte id;	// 方块 ID
	char x, y;	// 方块在游戏区中的坐标
	byte dir : 2;	// 方向(用两位二进制表示4种方向)
}	g_CurBlock, g_NextBlock;

// 定义游戏区
BYTE game_area[WIDTH][HEIGHT] = { 0 };



void WelcomeMenu();                                     // 游戏操作菜单窗口
void Init();											// 初始化游戏
void Quit();											// 退出游戏
void NewGame();											// 开始新游戏
void GameOver();										// 结束游戏
CMD  GetCmd();											// 获取控制命令
void DispatchCmd(CMD _cmd);								// 分发控制命令
void NewBlock();										// 生成新的方块
bool CheckBlock(BlockInfo _block);						// 检测指定方块是否可以放下
void DrawBlock(BlockInfo _block, DRAW _draw = SHOW);	// 画方块
void OnRotate();										// 旋转方块
void OnLeft();											// 左移方块
void OnRight();											// 右移方块
void OnDown();											// 下移方块
void OnSink();                                          // 下沉方块
void writefile(Pai &P);                                 // 写入用户信息
int  readfile(Pai &P);                                  // 读取保存的排行榜信息
void Paihang(Pai &P);                                   // 对玩家分数进行排行
void ScoreTop();                                        // 得分排行榜
void showScore();                                       // 显示分数
void showLevel();                                       // 显示等级
void DisplayPause();                                    // 暂停

