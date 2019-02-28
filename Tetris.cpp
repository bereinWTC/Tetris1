#include "Tetris.h"
#include <conio.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>  
#include <thread>
#include "windows.h"
#include <process.h>


using namespace std;


#pragma comment(lib,"Winmm.lib")						// utilisation de Winmm.lib


 unsigned int WINAPI ThreadProFunc(void *pParam)
 {
//action pour autre joueurs
	 return 0;
 }
 					
 void Tetris::reset_AI_dec_area()
 {

	 for (int i = 0; i < WIDTH; i++)
		 for (int j=0;j<HEIGHT;j++)
		 {
			 AI_dec_area[i][j] = -1;
		 }
}
 void Tetris::reset_max_table()
 {
	 for (int i = 0; i < WIDTH; i++)
	 {
		 max_table[i] = -100;
	 }
 }
void Tetris::reset_game_area()
{
	
	setfillcolor(BLACK);
	bar(position+(WIDTH + 1)*SIZE, SIZE, position+(2 * WIDTH + 1)*SIZE - 1, (HEIGHT + 1)*SIZE - 1);
	ZeroMemory(game_area, WIDTH*HEIGHT);
	if (flag == 2)
	{
		reset_max_table();
		reset_AI_dec_area();
	}

	
}

void Tetris::reset_game_area2(Tetris tetris2)
{

	setfillcolor(BLACK);
	bar(tetris2.position + (WIDTH + 1)*SIZE, SIZE, tetris2.position + (2 * WIDTH + 1)*SIZE - 1, (HEIGHT + 1)*SIZE - 1);
	ZeroMemory(game_area, WIDTH*HEIGHT);
} 

void Tetris::NewGame()
{
	reset_game_area();
	NewBlock();
}



void Tetris::GameOver()
{
	HWND wnd = GetHWnd();
	writefile(user);
	score_list(user);
	if (MessageBox(wnd, "GAME OVER \nJouer encore une fois?", "GAME OVER", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{

		NewGame();
		score = 0;
		showScore();
	}
	else
	{
		closegraph();
		start();
	}

}
void Tetris::NewBlock()
{
	/* obtenir le bloc prochain */
	g_NextBlock.id = rand() % 7;
	g_NextBlock.dir = rand() % 4;
	g_NextBlock.x = 11;
	g_NextBlock.y = 21;

	/*passer la configuration de nouveau block au block current*/
	g_CurBlock.id = g_NextBlock.id, g_NextBlock.id = rand() % 7;
	g_CurBlock.dir = g_NextBlock.dir, g_NextBlock.dir = rand() % 4;
	g_CurBlock.x = 3;
	g_CurBlock.y = 24;

	WORD c = g_blocks[g_CurBlock.id].dir[g_CurBlock.dir];

	/*pour assurer que  le nouveau tetromino est exactement au niveau de l'ecran en haut*/
	while ((c & 0X000F) == 0)
	{
		--g_CurBlock.y;					
		c >>= 4;						
	}

	/* créer un bloc */
	DrawBlock(g_CurBlock,SHOW);

	/* créer le bloc prochain */
	setfillcolor(BLACK);
	bar(440, 20, 519, 99);
	DrawBlock(g_NextBlock, SHOW);

	/* les blocs tombent automatiquement */
	oldtime = GetTickCount();

}
void Tetris::DrawBlock(BlockInfo _block, DRAW _draw)
{
	WORD b = g_blocks[_block.id].dir[_block.dir];
	int x, y;

	int color = BLACK;
	switch (_draw)
	{
		case SHOW: color = g_blocks[_block.id].color; break;			// donne le couleur du tetromino
		case HIDE: color = BLACK;	break;								// donne tetromino au noir
		case FIX: color = g_blocks[_block.id].color / 3; break;			// donne couleur du tetromino/3 quand il est fix dans la région du jeu 
	}
	setfillcolor(color);                                                // met le couleur du tetromino
	 /*scanner une région de £´*£´ et dessiner le bloc */
	for (int i = 0; i < 16; i++)                                       
	{
		if (b & 0x8000)
		{
			x = _block.x + i % 4;
			y = _block.y - i / 4;
			if (y < HEIGHT)
			{
				if (_draw != HIDE)
				{
					bar3d(position+x*SIZE + 2 + 220, (HEIGHT - y - 1)*SIZE + 2 + 20, position + (x + 1)*SIZE - 4 + 220, (HEIGHT - y)*SIZE - 4 + 20, 3, true);
				}
				else
					bar(position + x*SIZE + 220, (HEIGHT - y - 1)*SIZE + 20, position + (x + 1)*SIZE - 1 + 220, (HEIGHT - y)*SIZE - 1 + 20);

			}
		}
		b <<= 1;
	}
}

void Tetris::DrawBlock2(BlockInfo _block, DRAW _draw,int position)
{
	WORD b = g_blocks[_block.id].dir[_block.dir];
	int x, y;

	int color = BLACK;
	switch (_draw)
	{
	case SHOW: color = g_blocks[_block.id].color; break;
	case HIDE: color = BLACK;	break;
	case FIX: color = g_blocks[_block.id].color / 3; break;      
	}
	setfillcolor(color);

	for (int i = 0; i < 16; i++)                                     
	{
		/* pour assurer que le bloc est situé au gauche et en haut de ce region et après on peut le bien mettre */
		if (b & 0x8000)
		{
			x = _block.x + i % 4;
			y = _block.y - i / 4;
			if (y < HEIGHT)
			{
				if (_draw != HIDE)
				{
					bar3d(x*SIZE + 2 + 220, (HEIGHT - y - 1)*SIZE + 2 + 20, (x + 1)*SIZE - 4 + 220, (HEIGHT - y)*SIZE - 4 + 20, 3, true);
				}
				else
					bar(x*SIZE + 220, (HEIGHT - y - 1)*SIZE + 20, (x + 1)*SIZE - 1 + 220, (HEIGHT - y)*SIZE - 1 + 20);

			}
		}
		b <<= 1;
	}

}
bool Tetris::CheckBlock(BlockInfo _block)
{
	WORD b = g_blocks[_block.id].dir[_block.dir];
	int x, y;
	for (int i = 0; i < 16; i++)
	{
		if (b & 0x8000)
		{
			x = _block.x + i % 4;
			y = _block.y - i / 4;
			if ((x < 0) || (x >= WIDTH) || (y < 0))						//trop gauche ou trop droite
				return false;
			if ((y < HEIGHT) && (game_area[x][y]))						//trop haut
				return false;

		}
		b <<= 1;
	}
	return true;
}
CMD  Tetris::GetCmd()
{
	while (1)
	{
		// le bloc va tomber chaque 0.5s si il y a aucune opération 
		Sleep(20);
		DWORD newtime = GetTickCount();
		/*
		faire tombé automatiquement un tetromino pour un intervale de temps 
		qui dépend du niveau du jeu. Plus le nuiveau est grand, plus vide
		le tetromino tombe.
		*/
		if (newtime - oldtime >= 600 - level * 50)
		{
			oldtime = newtime;
			return CMD_DOWN;
		}
		if (_kbhit())								// check the command
		{
			switch (_getch())
			{

			case 'C':
			case 'c':  return CMD_STOP;			//pause le jeu 
			case 'R':
			case 'r':  return CMD_RETOUR;
			case 27:   return CMD_QUIT;
			case 13:  return CMD_SINK;			//faire tombé le tetromino
			case 72:	return CMD_ROTATE;      //up
			case 75:	return CMD_LEFT;        //left
			case 77:	return CMD_RIGHT;       //right
			case 80:	return CMD_DOWN;        //down
			
			}

		}


	}
}

CMD  Tetris::GetCmd2()
{
	while (1)
	{
		// le bloc va tomber chaque 0.5s si il y a aucune opération 
		//Sleep(200);
		DWORD newtime = GetTickCount();
		if (newtime - oldtime >= 600 - level * 50)
		{
			oldtime = newtime;
			return CMD_DOWN;
		}
		if (_kbhit())								// check the command
		{
			switch (_getch())
			{
			case 'W':
			case 'w':  return CMD_ROTATE;
			case 'A':
			case 'a':  return CMD_LEFT;
			case 'S':
			case 's':  return CMD_DOWN;
			case 'D':
			case 'd':  return CMD_RIGHT;
			case 'C':
			case 'c':  return CMD_STOP;
			case 'R':
			case 'r':  return CMD_RETOUR;
			case 27:   return CMD_QUIT;
			case ' ':  return CMD_SINK;
				
			}

		}


	}
}


void Tetris::DispatchCmd(CMD _cmd)
{
	switch (_cmd)
	{
	case CMD_ROTATE:	OnRotate();		break;
	case CMD_LEFT:		OnLeft();		break;
	case CMD_RIGHT:		OnRight();		break;
	case CMD_DOWN:		OnDown();		break;
	case CMD_SINK:      OnSink();       break;
	case CMD_STOP:      DisplayPause(); break;
	case CMD_QUIT:		break;
	case CMD_RETOUR:	Retour();	break;
	}
}
void Tetris::OnRotate()
{

	int distance=0;
	BlockInfo temp = g_CurBlock;
	temp.dir++;					 if (CheckBlock(temp)) { distance = 0; goto rotate; }		// rotation sans bouger
	temp.x = g_CurBlock.x - 1;   if (CheckBlock(temp)) { distance = -1; goto rotate; }		// bouger à gauche pour un unité et faire la rotation  
	temp.x = g_CurBlock.x + 1;   if (CheckBlock(temp)) { distance = 1; goto rotate; }		// bouger à droite pour un unité et faire la rotation  
	temp.x = g_CurBlock.x - 2;   if (CheckBlock(temp)) { distance = -2; goto rotate; }		// bouger à gauche pour 2 unités et faire la rotation  
	temp.x = g_CurBlock.x + 2;   if (CheckBlock(temp)) { distance = 2; goto rotate; }		// bouger à droite pour 2 unités et faire la rotation  

	//rotation:tuer le bloc à l'instant et créer un nouveau bloc(qui est tourné) 
rotate:
	DrawBlock(g_CurBlock, HIDE);	//cacher le tetromino courrant 
	++g_CurBlock.dir;				//changer la direction du tetromino 
	g_CurBlock.x += distance;		//ajouter la distance pour pivoter
	DrawBlock(g_CurBlock, SHOW);	//afficher le tetromino tourné

}
void Tetris::OnLeft()
{
	BlockInfo temp = g_CurBlock;
	--temp.x;

	if (CheckBlock(temp))
	{
		DrawBlock(g_CurBlock, HIDE); //cacher le tetromino courrant 
		--g_CurBlock.x;				 //aller vers le gauche 
		DrawBlock(g_CurBlock, SHOW); //afficher le tetromino translaté
	}
}
void Tetris::OnRight()
{
	BlockInfo temp = g_CurBlock;
	++temp.x;

	if (CheckBlock(temp))
	{
		DrawBlock(g_CurBlock, HIDE);
		++g_CurBlock.x;
		DrawBlock(g_CurBlock, SHOW);
	}
}
void Tetris::OnDown()
{
	BlockInfo temp = g_CurBlock;
	--temp.y;

	if (CheckBlock(temp))
	{
		DrawBlock(g_CurBlock, HIDE);
		--g_CurBlock.y;
		DrawBlock(g_CurBlock, SHOW);
	}
	else OnSink();
}
void Tetris::OnSink()
{
	int i, x, y;
	int count = 0;
	// continuer à bouger au dessous 
	DrawBlock(g_CurBlock, HIDE);								//cacher le tetromino courrant 
	BlockInfo temp = g_CurBlock;
	--temp.y;
	while (CheckBlock(temp))									//assurer que le tetromino peut toujour tombé et en meme temps trouver le plus bas position pissble du tetromino
	{
		--g_CurBlock.y;
		--temp.y;
	}
	DrawBlock(g_CurBlock, FIX);									//changer le couleur du tetromino une fois arrêtré

	// mettre le bloc sur la place finale 
	WORD b = g_blocks[g_CurBlock.id].dir[g_CurBlock.dir];
	for (i = 0; i < 16; i++)
	{
		if (b & 0x8000)
		{
			if (g_CurBlock.y - i / 4 >= HEIGHT)
			{
				GameOver();										// si la place finale est trop haut - GAMEOVER
				return;
			}
			else
			{
				game_area[g_CurBlock.x + i % 4][g_CurBlock.y - i / 4] = 1;
				if (flag == 2)
				{
					AI_dec_area[g_CurBlock.x + i % 4][g_CurBlock.y - i / 4] = g_CurBlock.y - i / 4; //mettre le niveau du chaque element de tetromino dans AI_dec_area
					
				}

			}

		}

		b <<= 1;
	}
	if (flag==2) cal_max_in_col();

	// confirmer est-ce que on doit supprimer un ou plusieurs lignes
	int row[4] = { 0 };
	bool bRow = false;																				//pour tester si il'y a des lignes à suprimmer
	for (y = g_CurBlock.y; y >= max(g_CurBlock.y - 3, 0); y--)										//peut supprimmer jusqu'à 3 lignes en meme temps
	{
		/*Compter le nombre d'élément dans une ligne*/
		i = 0;
		for (x = 0; x < WIDTH; x++)
			if (game_area[x][y] == 1)
				i++;
		if (i == WIDTH)
		{
			bRow = true;
			row[g_CurBlock.y - y] = 1;																// marquer le ligne à supprimer
			setfillcolor(WHITE);
			bar(220, (HEIGHT - y - 1)*SIZE + SIZE / 2 - 2 + 20, WIDTH * SIZE - 1 + 220, (HEIGHT - y - 1) * SIZE + SIZE / 2 + 2 + 20);
		}
	}

	if (bRow)																					  //supprimer des lignes marqué
	{
		Sleep(20); 
		IMAGE img;
		for (i = 0; i < 4; i++)
		{
			if (row[i])
			{
				count++;
				for (y = g_CurBlock.y - i + 1; y < HEIGHT; y++)
					for (x = 0; x < WIDTH; x++)
					{
						game_area[x][y - 1] = game_area[x][y];
						game_area[x][y] = 0;

						//pour le AI_dec_area
						if (flag == 2)
						{
							if (AI_dec_area[x][y]==-1) AI_dec_area[x][y - 1] =- 1;
							else AI_dec_area[x][y - 1] = AI_dec_area[x][y] - 1;
							AI_dec_area[x][y] = -1;
							cal_max_in_col();
							
							
						}
					}
				getimage(&img, 220, 20, WIDTH * SIZE, (HEIGHT - (g_CurBlock.y - i + 1)) * SIZE);
				putimage(220, SIZE + 20, &img);
			} 
		}
		
		// calculer le score et le niveaux de difficult?
		score += count * 10;
		showScore();
		level = score / 100 + 1;					          //le niveau est incrément?tanque tout les 100 points de score 
		showLevel();
	}
	NewBlock();												  // créer un nouveau bloc
}
void Tetris::writefile(Score_list &S)
{


	FILE *fp;
	fopen_s(&fp, "data_storage\\ScoreTop.rec", "ab");
	if (!fp)
	{
		printf("file cannot be opened");
		exit(1);
	}
	do
	{
		InputBox(S.name, 10, "Votre prénom");					// ajouter le prénom de joueur 
		S.grades = score;
		fwrite(&S, sizeof(S), 1, fp);


		break;
	} while (1);
	fclose(fp);

}
void Tetris::score_list(Score_list &S)
{
	Score_list a[12];
	FILE *fp;
	int length;


	int i, j;
	fopen_s(&fp, "data_storage\\ScoreTop.rec", "rb");

	for (i = 1; fread(&S, sizeof(S), 1, fp) == 1 && i <= 11; i++)
	{

		a[i] = S;
	}

	length = i - 1;

	for (i = 2; i <= length; i++)
		if (a[i].grades < a[i - 1].grades)
		{
			a[0] = a[i];
			a[i] = a[i - 1];
			for (j = i - 2; a[0].grades < a[j].grades; j--)
				a[j + 1] = a[j];
			a[j + 1] = a[0];
		}


	fclose(fp);

	fopen_s(&fp, "data_storage\\ScoreTop.rec", "wb");
	int b = 0;
	for (i = length; b < 10 && i >= 1; i--, b++)
	{
		fwrite(&a[i], sizeof(a[i]), 1, fp);
	}
	fclose(fp);
}
void Tetris::showScore() {
	char str[10];
	setfillcolor(BLACK);
	rectangle(449, 299, 576, 326);
	bar(450, 300, 575, 325);
	setcolor(WHITE);
	settextstyle(25, 0, "Calibri");
	sprintf_s(str, sizeof(str), "score:%d", score);
	outtextxy(450, 300, str);
}
void Tetris::showLevel()
{
	char str[10];
	setfillcolor(BLACK);
	rectangle(449, 249, 576, 276);
	bar(450, 250, 575, 275);
	setcolor(WHITE);
	settextstyle(25, 0, "Calibri");
	sprintf_s(str, sizeof(str), "Level:%d", level);
	outtextxy(450, 250, str);

}
void Tetris::DisplayPause()
{
	while (_getch() != 'c'&&_getch() != 'C');
}

void Tetris::setmulti() {
	ifmulti = 1;
}


void  Tetris::WelcomeMenu()
{
	initgraph(640, 480);
	IMAGE img(640, 480);
	loadimage(&img, "Pic\\Welcome_menu_1.jpg");
	SetWorkingImage(&img);
	SetWorkingImage();
	putimage(0, 0, &img);
}
int   Tetris::get_choice()
{
	MOUSEMSG m;
	int flag = 0;
	while (TRUE)
	{
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:													// opération de la souris 
			if (m.x >= 268 && m.x <= 392 && m.y >= 268 && m.y <= 312)
			{
				flag = 1;														//jouer toute seul
				break;
			}
			else if (m.x >= 268 && m.x <= 392 && m.y >= 328 && m.y <= 370)
			{
				flag = 2;														//void IA jouer 
				break;
			}
			else if (m.x > 268 && m.x <= 392 && m.y >= 388 && m.y <= 430)
			{
				flag = 3;														//jouer avec des autres

				break;
			}

		}
		if (flag != 0) break;

	}
	return flag;

}

void  Tetris::play_game_multi()
{
	Tetris tetrisa(0, 1, 0), tetrisb(0, 1, 480);
	/* commencer le jeux*/
	tetrisa.NewGame();
	tetrisb.NewGame();
	CMD  c,d;
	while (1)
	{
		c = tetrisa.GetCmd2();											    //recevoir la commande
		tetrisa.DispatchCmd(c);												// exécuter le commande
		/* ouvrir un fenêtre pour assurer quitter*/
		if (c == CMD_QUIT)
		{
			HWND wnd = GetHWnd();
			if (MessageBox(wnd, "Est-ce que vous voulez quitter?", "Bonjour!", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
			{
				break;	
			}
		}
		

		d = tetrisb.GetCmd();											    //recevoir la commande
		tetrisb.DispatchCmd(d);												// exécuter le commande
        /* ouvrir un fenêtre pour assurer quitter*/

		if (d == CMD_QUIT)
		{
			HWND wnd = GetHWnd();
			if (MessageBox(wnd, "Est-ce que vous voulez quitter?", "Bonjour!", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
			{
				break;
				
			}
		}
		
	}
	closegraph();
	start();
}

void  Tetris::top_score()
{
	initgraph(640, 480);
	IMAGE img(640, 480);
	loadimage(&img, "Pic\\Scoretop.jpg");
	SetWorkingImage(&img);
	Score_list S;
	readfile(S);
	SetWorkingImage();
	putimage(0, 0, &img);
	MOUSEMSG p;
	int flag = 0;
	while (TRUE) {
		p = GetMouseMsg();
		switch (p.uMsg)
		{
		case(WM_LBUTTONDOWN):
			if (p.x >= 520 && p.x <= 582 && p.y >= 14 && p.y <= 46)
			{
				WelcomeMenu(); flag = 1;
			}
			break;
		}if (flag != 0) break;
	}
}
void  Tetris::game_board_init_multi()
{
	initgraph(960, 480);

	HWND hwnd = GetHWnd();
	SetWindowText(hwnd, "Tetris");												// Titre de jeu 

																				/* ouvrir la musique du background*/
	mciSendString("open music\\change.mp3 alia mymusic", NULL, 0, NULL);
	mciSendString("play music\\change.mp3 repeat", NULL, 0, NULL);


	srand((unsigned)time(NULL));												// pour fabriquer un chiffre random 
	IMAGE img(960, 480);
	loadimage(&img, "Pic\\background.png");
	SetWorkingImage(&img);
	showScore();
	showLevel();

	/*region pour afficher le guide du jeur*/
	settextstyle(35, 0, "Calibri");
	setbkmode(TRANSPARENT);
	outtextxy(50, 50, "GUIDE");
	settextstyle(20, 0, "Calibri");
	outtextxy(55, 135, "W-rotate");
	outtextxy(55, 155, "A-move left");
	outtextxy(55, 175, "S-move down");
	outtextxy(55, 195, "D-move right");
	outtextxy(55, 215, "space-to the bottom");
	outtextxy(55, 235, "ESC-quit");
	settextstyle(22, 0, "Calibri");
	outtextxy(60, 300, "C-Pause the game");
	outtextxy(60, 320, "R-Return");



	/*définir la région pour jouer*/
	rectangle((WIDTH + 1)*SIZE - 1, SIZE - 1, (2 * WIDTH + 1)*SIZE, (HEIGHT + 1)*SIZE);
	rectangle(480 + (WIDTH + 1)*SIZE - 1, SIZE - 1, 480 + (2 * WIDTH + 1)*SIZE, (HEIGHT + 1)*SIZE);
	rectangle(339, 19, 420, 100);
	SetWorkingImage();
	putimage(0, 0, &img);

}

int	  Tetris::readfile(Score_list &S)
{
	int x = 78, y = 70, z = 455, i = 1;
	char ch_grades[5], ch[1], dot = '.';
	FILE *fp;
	fopen_s(&fp, "data_storage\\ScoreTop.rec", "rb");
	if (!fp)
	{
		printf("file cannot be opened");
		exit(1);
	}
	settextstyle(33, 0, "Calibri");
	setbkmode(TRANSPARENT);

	while (fread(&S, sizeof(S), 1, fp) == 1)
	{
		sprintf_s(ch, sizeof(ch), "%d", i);
		outtextxy(x - 22, y, ch);
		outtextxy(x - 11, y, dot);
		outtextxy(x, y, S.name);
		sprintf_s(ch_grades, sizeof(ch_grades), "%g", S.grades);
		outtextxy(z, y, ch_grades);
		y += 35;
		i++;
	}
	fclose(fp);
	return 0;
}
void  Tetris::Quit()
{
	closegraph();
	exit(0);
}
void Tetris::start()
{
	WelcomeMenu();
	flag = get_choice();
	goto_choice(flag);
}
void Tetris::goto_choice(int flag)
{

	if (flag == 1 || flag == 2) play_game();
	else if (flag == 3)
	{
		//setmulti();

		game_board_init_multi();
		play_game_multi();

	}
}


void Tetris::play_game()
{
	game_board_init();
	/* commencer le jeux*/
	NewGame();
	CMD  c;
	if (flag == 1)
	{
		while (1)
		{
			c = GetCmd();											    //recevoir la commande
			DispatchCmd(c);												// exécuter le commande
			/* ouvrir un fenêtre pour assurer quitter*/
			if (c == CMD_QUIT)
			{
				HWND wnd = GetHWnd();
				if (MessageBox(wnd, "Est-ce que vous voulez quitter?", "Bonjour!", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
				{
					break;
				}
						
			}

		}
	}
	else if (flag == 2)
	{
		while (1)
		{
			c = GetAiCmd();
			DispatchCmd(c);												// exécuter le commande
			/* ouvrir un fenêtre pour assurer quitter*/
			if (c == CMD_QUIT)
			{
				HWND wnd = GetHWnd();
				if (MessageBox(wnd, "Est-ce que vous voulez quitter?", "Bonjour!", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
				{
					break;	
				}
			}

		}
	}
	closegraph();
	start();
	
}
void Tetris::game_board_init()
{
	initgraph(640, 480);

	HWND hwnd = GetHWnd();
	SetWindowText(hwnd, "Tetris");												// Titre de jeu 

	/* ouvrir la musique du background*/
	mciSendString("open music\\change.mp3 alia mymusic", NULL, 0, NULL);
	mciSendString("play music\\change.mp3 repeat", NULL, 0, NULL);


	srand((unsigned)time(NULL));												// pour fabriquer un chiffre random 
	IMAGE img(640, 480);
	loadimage(&img, "Pic\\background.png");
	SetWorkingImage(&img);
	showScore();
	showLevel();

	/*region pour afficher le guide du jeur*/
	settextstyle(35, 0, "Calibri");
	setbkmode(TRANSPARENT);
	outtextxy(50, 50, "GUIDE");
	settextstyle(20, 0, "Calibri");
	outtextxy(55, 135, "up-rotate");
	outtextxy(55, 155, "left-move left");
	outtextxy(55, 175, "down-move down");
	outtextxy(55, 195, "right-move right");
	outtextxy(55, 215, "space-to the bottom");
	outtextxy(55, 235, "ESC-quit");
	settextstyle(22, 0, "Calibri");
	outtextxy(60, 300, "P-Pause the game");



	/*définir la région pour jouer*/
	rectangle((WIDTH + 1)*SIZE - 1, SIZE - 1, (2 * WIDTH + 1)*SIZE, (HEIGHT + 1)*SIZE);
	rectangle(439, 19, 520, 100);
	SetWorkingImage();
	putimage(0, 0, &img);

}

void Tetris::Retour() {
	HWND wnd = GetHWnd();
	if (MessageBox(wnd, "Est-ce que vous voulez rentrer au debut?", "Bonjour!", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		closegraph();
		start();
	}
}
CMD  Tetris::GetAiCmd()
{
	while (1)
	{

		// le bloc va tomber chaque 0.5s si il y a aucune operation 
		Sleep(100);
		DWORD newtime = GetTickCount();
		if (newtime - oldtime >= 600 - level * 50)
		{
			oldtime = newtime;
			
			return CMD_DOWN;
		}
		if (_kbhit())								// check the command
		{
			switch (_getch())
			{
			case 'c':
			case 'C':  return CMD_STOP;
			case 27:   return CMD_QUIT;
			}

		}
		else
		{
			int m = find_min();
			BlockInfo temp = g_CurBlock;
			if (g_CurBlock.x - m > 0)
			{
				--temp.x;
				if (!CheckBlock(g_CurBlock)) OnRotate();
				return CMD_LEFT;
			}	
			else if (g_CurBlock.x - m == 0)
				return CMD_DOWN;
			else
			{
				++temp.x;
				if (!CheckBlock(g_CurBlock)) OnRotate();
				return CMD_RIGHT;
			}
				
		}

		
	}

}
void Tetris::cal_max_in_col()
{
	for (int i = 0; i < WIDTH; i++)
	{
		max_table[i] = -100;
		for (int j = 0; j < HEIGHT; j++)
		{
			if (max_table[i] < AI_dec_area[i][j])
				max_table[i] = AI_dec_area[i][j];

		}
	}
}
int Tetris::find_min()
{
	int min_x;
	int min = 100;
	for (int i = 0; i < WIDTH; i++)
	{
		if (min > max_table[i])
		{
			min = max_table[i];
			min_x = i;
		}
	}
	switch (g_CurBlock.id)
	{
	case 0:
		switch (g_CurBlock.dir)
		{
		case 0: return min_x;
		case 1: return min_x - 1;
		case 2: return min_x;
		case 3: return min_x - 1;
		}
	case 1: return min_x - 1;
	case 2: return min_x - 1;
		switch (g_CurBlock.dir)
		{
		case 0: return min_x - 1;
		case 1: return min_x;
		case 2: return min_x - 1;
		case 3: return min_x - 1;
		default:return min_x;
		}
	case 3:
		switch (g_CurBlock.dir)
		{
		case 0: return min_x - 1;
		case 1: return min_x;
		case 2: return min_x - 1;
		case 3: return min_x - 1;
		default:return min_x;
		}
	case 4:
		switch (g_CurBlock.dir)
		{
		case 0: return min_x;
		case 1: return min_x - 1;
		case 2: return min_x - 1;
		case 3: return min_x - 1;
		default:return min_x;
		}
	case 5: return min_x - 1;
	case 6: 
		switch (g_CurBlock.dir)
		{
		case 0: return min_x;
		case 1: return min_x;
		case 2: return min_x;
		case 3: return min_x - 1;
		default:return min_x;
		}
	default: return min_x;
		
	}

}


void Tetris::init_g_block()
{
	/*L'ensemble de blocs possible*/

	 /*
	il y a 7 types des bloc
	0x0f00 = [0000 1111 0000 0000]:	I horizontale
	0x4444=[0100 0100 0100 0100]: I verticale
	*/
	g_blocks[0] = { 0x0F00, 0x4444, 0x0F00, 0x4444, RED };				    // I
	g_blocks[1] = { 0x0660, 0x0660, 0x0660, 0x0660, BLUE };					// O
	g_blocks[2] = { 0x4460, 0x02E0, 0x0622, 0x0740, MAGENTA };			    // L
	g_blocks[3] = { 0x2260, 0x0E20, 0x0644, 0x0470, YELLOW };				// L oppose
	g_blocks[4] = { 0x0C60, 0x2640, 0x0C60, 0x2640, CYAN };					// Z
	g_blocks[5] = { 0x0360, 0x4620, 0x0360, 0x4620, GREEN };				// Z oppose
	g_blocks[6] = { 0x4E00, 0x4C40, 0x0E40, 0x4640, BROWN };			    // T
   
}
void Tetris::init_game_area()
{
	ZeroMemory(game_area, WIDTH*HEIGHT);
}

Tetris::Tetris()
{
	score = 0; level = 1; position = 0; ifmulti = 0;
	init_g_block();
	init_game_area();
	flag = 0;
	oldtime= GetTickCount();

}
Tetris::Tetris(int xscore, int xlevel, int xposition)
{
	init_g_block();
	init_game_area();
	score = xscore;
	level = xlevel;
	position = xposition;
	flag = 0;
	oldtime= GetTickCount();
}
