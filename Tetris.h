#include <graphics.h>                            // Fonctionne sur windows. L'installation de EasyX est obligatoire.
#include <conio.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>      
#pragma comment(lib,"Winmm.lib")                 // utilisation de Winmm.lib
#define	WIDTH	10		                         // la region de jeu est de 10*22 unites
#define	HEIGHT	22	                             // 
#define	SIZE	20		                         // chaque unite est de 20 pixels


struct Pai
{
	char name[20];
	double grades;
};
typedef struct Pai Pai;


enum CMD
{
	CMD_ROTATE,						// notation 
	CMD_LEFT, CMD_RIGHT, CMD_DOWN,	// bouger 
	CMD_SINK,				        // aller tous droite a la base 
	CMD_QUIT,						// quitter 
	CMD_STOP                        // pause
};
enum DRAW
{
	SHOW,	                       
	HIDE,	                       
	FIX		                       
};


struct Block
{
	WORD dir[4];                //structure des blocs
	COLORREF  color;            //couleurs des bloc 
};
// il y a 7 types des bloc 
Block g_blocks[7] = { { 0x0F00, 0x4444, 0x0F00, 0x4444, RED },    // I
//ici 0x0f00 = [0000 1111 0000 0000], 0x4444=[0100 0100 0100 0100] qui signifient le I horizontale et verticale
{ 0x0660, 0x0660, 0x0660, 0x0660, BLUE },		// O
{ 0x4460, 0x02E0, 0x0622, 0x0740, MAGENTA },	// L
{ 0x2260, 0x0E20, 0x0644, 0x0470, YELLOW },	// L oppose
{ 0x0C60, 0x2640, 0x0C60, 0x2640, CYAN },		// Z
{ 0x0360, 0x4620, 0x0360, 0x4620, GREEN },	// Z oppose
{ 0x4E00, 0x4C40, 0x0E40, 0x4640, BROWN } };	// T
												
struct BlockInfo
{
	byte id;	// ID
	char x, y;	// coordonnee
	byte dir : 2;	// direction
}	g_CurBlock, g_NextBlock;


BYTE game_area[WIDTH][HEIGHT] = { 0 };



void WelcomeMenu();                                     // le menu au debut pour commencer le jeu
void Init();											// initialisation de jeu
void Quit();											// quitter
void NewGame();											// commencer un NewGame
void GameOver();										// Gameover
CMD  GetCmd();											// obtenir les commandes de clavier
void DispatchCmd(CMD _cmd);								// distribuer les commandes
void NewBlock();										// cr¨¦er un nouveau bloc 
bool CheckBlock(BlockInfo _block);						// assurer que le bloc peut ¨ºtre mis 
void DrawBlock(BlockInfo _block, DRAW _draw = SHOW);	// dessiner un bloc 
void OnRotate();										// rotation
void OnLeft();											// bouger
void OnRight();											// bouger 
void OnDown();											// bouger 
void OnSink();                                          // aller tous droite au dessous 
void writefile(Pai &P);                                 // ¨¦crire les scores
int  readfile(Pai &P);                                  // lire les scores 
void Paihang(Pai &P);                                   // lister les scores
void ScoreTop();                                        // disposer la liste des scores
void showScore();                                       // 
void showLevel();                                       // 
void DisplayPause();                                    // Pause

