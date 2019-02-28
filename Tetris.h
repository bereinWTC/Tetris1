#pragma once
#include "CMD.h"
#include "BlockInfo.h"
#include "Score_list.h"
#include "DRAW.h"
#include "Block.h"
#include <iostream>

/* la region de jeu est de 10*22 unites*/
#define	WIDTH	10										
#define	HEIGHT	22	  
/*chaque unite est de 20 pixels*/
#define	SIZE	20										

/*
C'est une classe qui cotient des atributs et m�thodes pour faire marcher le jeu de tetris.
*/
class Tetris
{
public :
	Tetris();
	Tetris(int xscore, int xlevel, int xposition);

	void NewGame();											// commencer un NewGame
	void GameOver();										// Gameover
	CMD  GetCmd();											// obtenir les commandes de clavier
	void DispatchCmd(CMD _cmd);								// distribuer les commandes
	void NewBlock();										// cr�er un nouveau bloc 
	bool CheckBlock(BlockInfo _block);						// assurer que le bloc ne sorte pas la fronti�re du r�gion du jeu 
	void DrawBlock(BlockInfo _block, DRAW _draw);	// dessiner un bloc
	void DrawBlock2(BlockInfo _block, DRAW _draw, int position);
	void OnRotate();										// rotation
	void OnLeft();											// bouger vers le gauche 
	void OnRight();											// bouger vers le droite
	void OnDown();											// bouger vers le bas
	void OnSink();                                          // aller tout droite au dessous 
	void Retour();											//
	void writefile(Score_list &P);                          // �crire les scores
	void score_list(Score_list &P);                         // lister les scores
	void showScore();                                       // afficher le score en temps r�el
	void showLevel();                                       // afficher le niveau du jeu
	void reset_game_area();									// clear game area and put it in black
	void reset_game_area2(Tetris tetris2);					// pas utile
	int	  readfile(Score_list &S);
	void  Quit();


	void start();											// afficher la page d'acceuil
	void play_game();										// Comencer ?jouer
	void goto_choice(int flag);								// prendre l'action selon le chois de joueur
	void game_board_init();									// initialisation de game board
	void WelcomeMenu();										// la page d'acceuil du jeu
	int  get_choice();										// recevoir le choix du jueur � la page d'acceuil
	void top_score();										// afficher le score du meilleur jueur
	
	//////////////MULTI JUEUR///////
	CMD  GetCmd2();											// obtenir les commandes de clavier
	void DisplayPause();                                    // Pause
	void setmulti();										//
	void play_game_multi();								    //
	void game_board_init_multi();							//
	void reset_max_table();								    //
	int  AI_dec_area[WIDTH][HEIGHT] = { -1 };				// C'est un tableau qui stock les trous occup�e par un bloc leur niveau et -1 pour la trou vide 
	int  max_table[WIDTH] = { -100 };						// Tableau qui stoke la valeur maximum de chque colonne de AI_dec_area
	void init_g_block();
	void init_game_area();
	/////AI
	CMD  GetAiCmd();										// obtenire la commande de IA
	void cal_max_in_col();									// calculer la valeur maximum de chaque colonne de AI_dec_area et les mettre dans max_table
	int find_min();											// Trouver index de valeur minimum de max_table
	void reset_AI_dec_area();								// r�initialiser le tableau AI_dec_area

protected:
	int position;											//
	int score;												// le score du jeux 
	int level;												// le niveau du jeux
	int ifmulti;											// ifmulti = 1 quand il y a un autre joueur, sinon ifmulti = 0
	BlockInfo g_CurBlock, g_NextBlock;						// tetromino actuel et prochain
	Block g_blocks[7];
	/*d�finir le score et le niveau de difficult?/*/
	BYTE game_area[WIDTH][HEIGHT];
	DWORD  oldtime;										// contr�le de temp pour charge op�ration 
	Score_list user;										// d�finir user pour stocker le score et nom de joueur
	int flag;
	
	
};


