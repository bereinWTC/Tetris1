#include "Tetris.h"
#include <conio.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>   

void main()
{
	Tetris tetris1(0,1,0),tetris2(0,1,480);
	start(tetris1, tetris2);
}

void start(Tetris tetris1, Tetris tetris2)
{
	WelcomeMenu();
	int flag = get_choice();
	goto_choice(flag,tetris1,tetris2);
	_getch();
	closegraph();
}

void WelcomeMenu()
{
	initgraph(640, 480);
	IMAGE img(640, 480);
	loadimage(&img, "Pic\\Welcome_menu_1.jpg");
	SetWorkingImage(&img);
	SetWorkingImage();
	putimage(0, 0, &img);
}

int  get_choice()
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
				flag = 1;														//jouer
				break;
			}
			else if (m.x >= 268 && m.x <= 392 && m.y >= 328 && m.y <= 370)
			{
				flag = 2;														//Scores
				break;
			}
			else if (m.x > 268 && m.x <= 392 && m.y >= 388 && m.y <= 430)
			{
				flag = 3;														//quitter

				break;
			}

		}
		if (flag != 0) break;

	}
	return flag;

}

void goto_choice(int flag, Tetris tetris1, Tetris tetris2)
{
	if (flag == 1) play_game(tetris1,tetris2);
	else if (flag == 2)  top_score();
	else if (flag == 3) 	Quit();
}

void play_game(Tetris tetris1, Tetris tetris2)
{
	game_board_init(tetris1, tetris2);
	/* commencer le jeux*/
	tetris1.NewGame();
	tetris2.NewGame();
	CMD  c,d;
	while (1)
	{

		c = tetris1.GetCmd();											    //recevoir la commande
		d = tetris2.GetCmd();
		tetris1.DispatchCmd(c);												// exécuter le commande
		tetris2.DispatchCmd(d);
																	/* ouvrir un fenêtre pour assurer quitter*/
		if (c == CMD_QUIT)
		{
			HWND wnd = GetHWnd();
			if (MessageBox(wnd, "Est-ce que vous voulez quitter?", "Bonjour!", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
				Quit();
		}

	}
}


void top_score()
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
		case(WM_LBUTTONDOWN) :
			if (p.x >= 520 && p.x <= 582 && p.y >= 14 && p.y <= 46)
			{
				WelcomeMenu(); flag = 1;
			}
							 break;
		}if (flag != 0) break;
	}
}

void game_board_init(Tetris tetris1, Tetris tetris2)
{
	initgraph(960, 480);

	HWND hwnd = GetHWnd();
	SetWindowText(hwnd, "Tetris");												// Titre de jeu 

																				/* ouvrir la musique du background*/
	mciSendString("open music\\change.mp3 alia mymusic", NULL, 0, NULL);
	mciSendString("play music\\change.mp3 repeat", NULL, 0, NULL);


	srand((unsigned)time(NULL));												// pour fabriquer un chiffre random 
	IMAGE img(960, 480);
	loadimage(&img, "Pic\\1-110501012153.jpg");
	SetWorkingImage(&img);
	tetris1.showScore();
	tetris1.showLevel();

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
	rectangle(480+(WIDTH + 1)*SIZE - 1, SIZE - 1, 480+(2 * WIDTH + 1)*SIZE, (HEIGHT + 1)*SIZE);
	rectangle(339, 19, 420, 100);
	SetWorkingImage();
	putimage(0, 0, &img);

}


int	 readfile(Score_list &S)
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

void Quit()
{
	closegraph();
	exit(0);
}