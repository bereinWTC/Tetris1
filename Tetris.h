#include <conio.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>   
#include "Score_list.h"
#include "CMD.h"
#include "DRAW.h"
#include "Block.h"
#include "BlockInfo.h"
#pragma comment(lib,"Winmm.lib")						// utilisation de Winmm.lib
#define	WIDTH	10										// la region de jeu est de 10*22 unites
#define	HEIGHT	22	                       
#define	SIZE	20										// chaque unite est de 20 pixels
/*d�finir le score et le niveau de difficult�*/
int score = 0;
int level = 1;
BYTE game_area[WIDTH][HEIGHT] = { 0 };
DWORD  oldtime;											// contr�le de temp pour charge op�ration 
Score_list user;										// d�finir user pour stocker le score et nom de joueur
/*
il y a 7 types des bloc
0x0f00 = [0000 1111 0000 0000]:	I horizontale
0x4444=[0100 0100 0100 0100]: I verticale
*/
Block g_blocks[7] = {
{ 0x0F00, 0x4444, 0x0F00, 0x4444, RED },				// I
{ 0x0660, 0x0660, 0x0660, 0x0660, BLUE },				// O
{ 0x4460, 0x02E0, 0x0622, 0x0740, MAGENTA },			// L
{ 0x2260, 0x0E20, 0x0644, 0x0470, YELLOW },				// L oppose
{ 0x0C60, 0x2640, 0x0C60, 0x2640, CYAN },				// Z
{ 0x0360, 0x4620, 0x0360, 0x4620, GREEN },				// Z oppose
{ 0x4E00, 0x4C40, 0x0E40, 0x4640, BROWN } };			// T

class Tetris
{
public :
	void WelcomeMenu();                                     // le menu au debut pour commencer le jeu
	void Init();											// initialisation de jeu
	void Quit();											// quitter
	void NewGame();											// commencer un NewGame
	void GameOver();										// Gameover
	CMD  GetCmd();											// obtenir les commandes de clavier
	void DispatchCmd(CMD _cmd);								// distribuer les commandes
	void NewBlock();										// cr�er un nouveau bloc 
	bool CheckBlock(BlockInfo _block);						// assurer que le bloc peut �tre mis 
	void DrawBlock(BlockInfo _block, DRAW _draw = SHOW);	// dessiner un bloc 
	void OnRotate();										// rotation
	void OnLeft();											// bouger vers le gauche 
	void OnRight();											// bouger vers le droite
	void OnDown();											// bouger vers le bas
	void OnSink();                                          // aller tout droite au dessous 
	void writefile(Score_list &P);                          // �crire les scores
	int  readfile(Score_list &P);                           // lire les scores 
	void score_list(Score_list &P);                         // lister les scores
	void ScoreTop();                                        // disposer la liste des scores
	void showScore();                                       // afficher le score en temps r�el
	void showLevel();                                       // afficher le niveau du jeu
	void DisplayPause();                                    // Pause
	void play();

};


