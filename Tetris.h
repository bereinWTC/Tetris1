
#include "CMD.h"
#include "BlockInfo.h"
#include "Score_list.h"
#include "DRAW.h"
#include "Block.h"

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
	void NewBlock();										// créer un nouveau bloc 
	bool CheckBlock(BlockInfo _block);						// assurer que le bloc peut être mis 
	void DrawBlock(BlockInfo _block, DRAW _draw = SHOW);	// dessiner un bloc 
	void OnRotate();										// rotation
	void OnLeft();											// bouger vers le gauche 
	void OnRight();											// bouger vers le droite
	void OnDown();											// bouger vers le bas
	void OnSink();                                          // aller tout droite au dessous 
	void writefile(Score_list &P);                          // écrire les scores
	int  readfile(Score_list &P);                           // lire les scores 
	void score_list(Score_list &P);                         // lister les scores
	void ScoreTop();                                        // disposer la liste des scores
	void showScore();                                       // afficher le score en temps réel
	void showLevel();                                       // afficher le niveau du jeu
	void DisplayPause();                                    // Pause
	void play();

};


