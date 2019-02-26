#pragma once
#include "CMD.h"
#include "BlockInfo.h"
#include "Score_list.h"
#include "DRAW.h"
#include "Block.h"
#include <iostream>


#define	WIDTH	10										// la region de jeu est de 10*22 unites
#define	HEIGHT	22	                       
#define	SIZE	20										// chaque unite est de 20 pixels





class Tetris
{
public :
	void NewGame();											// commencer un NewGame
	void GameOver();										// Gameover
	CMD  GetCmd();											// obtenir les commandes de clavier
	CMD  GetCmd2();											// obtenir les commandes de clavier
	void DispatchCmd(CMD _cmd);								// distribuer les commandes
	void NewBlock();										// cr�er un nouveau bloc 
	bool CheckBlock(BlockInfo _block);						// assurer que le bloc peut �tre mis 
	void DrawBlock(BlockInfo _block, DRAW _draw = SHOW);	// dessiner un bloc
	void DrawBlock2(BlockInfo _block, DRAW _draw, int position);
	void OnRotate();										// rotation
	void OnLeft();											// bouger vers le gauche 
	void OnRight();											// bouger vers le droite
	void OnDown();											// bouger vers le bas
	void OnSink();                                          // aller tout droite au dessous 
	void Retour();
	void writefile(Score_list &P);                          // �crire les scores
	void score_list(Score_list &P);                         // lister les scores
	void showScore();                                       // afficher le score en temps r�el
	void showLevel();                                       // afficher le niveau du jeu
	void DisplayPause();                                    // Pause
	void reset_game_area();									// clear game area and put it in black
	void reset_game_area2(Tetris tetris2);									// pas utile
	int	  readfile(Score_list &S);
	void  Quit();
	Tetris();
	Tetris(int xscore, int xlevel, int xposition);
	void setmulti();
	///////////some functions for that will be overloaded////////
	void start();											// afficher la page d'acceuil
	void play_game();										// Comencer ?jouer
	void goto_choice(int flag);								// prendre l'action selon le chois de joueur
	void game_board_init();									// initialisation de game board

	///////////////I just compy them from the main functions///////////
	void WelcomeMenu();
	int   get_choice();
	void  *play_game_multi();
	void  top_score();
	void  game_board_init_multi();

	//////////////AI//////////
	CMD  GetAiCmd();										//obtenire la commande de IA
	void cal_max_in_col();									//calculer la valeur maximum de chaque colonne de AI_dec_area et les mettre dans max_table
	int find_min();											//Trouver index de valeur minimum de max_table
	
private:
	int position;
	int score;												//le score du jeux 
	int level;												//le niveau du jeux
	int ifmulti;											//ifmulti = 1 quand il y a un autre joueur, sinon ifmulti = 0
	BlockInfo g_CurBlock, g_NextBlock;
	
	
};

/*
void start(Tetris tetris1, Tetris tetris2);					// afficher la page d'acceuil
int  get_choice();											// recevoir le choice de joueur par le souris 
void play_game(Tetris tetris1, Tetris tetris2);				// Comencer ?jouer 
void goto_choice(int flag,Tetris tetris1, Tetris tetris2);	// prendre l'action selon le chois de joueur
int  readfile(Score_list &P);                               // lire les scores 
void WelcomeMenu();											// le menu au debut pour commencer le jeu
void game_board_init(Tetris tetris1, Tetris tetris2);	    // initialisation de game board
void Quit();												// quitter
void top_score();											// disposer la liste des scores

*/
////my commentaire/////

