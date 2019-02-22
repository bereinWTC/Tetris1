#include "Tetris.h"
#include <conio.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>   


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

BlockInfo g_CurBlock, g_NextBlock;



void Tetris::play()
{
	WelcomeMenu();
	_getch();
	closegraph();
}
void Tetris::WelcomeMenu()
{

	initgraph(640, 480);
	IMAGE img(640, 480);
	loadimage(&img, "Pic\\Welcome_menu_1.jpg");
	SetWorkingImage(&img);
	SetWorkingImage();
	putimage(0, 0, &img);
	MOUSEMSG m;
	int flag = 0;
	while (TRUE)
	{
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:													// op�ration de la souris 
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
	if (flag == 1)
	{
		Init();
		CMD  c;
		while (1)
		{

			c = GetCmd();												//recevoir la commande
			DispatchCmd(c);												// ex�cuter le commande
			/* ouvrir un fen�tre pour assurer quitter*/
			if (c == CMD_QUIT)
			{
				HWND wnd = GetHWnd();
				if (MessageBox(wnd, "Est-ce que vous voulez quitter?", "Bonjour!", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
					Quit();
			}

		}
	}
	else if (flag == 2)  ScoreTop();
	else if (flag == 3) 	Quit();
}
void Tetris::Init()
{
	initgraph(640, 480);

	HWND hwnd = GetHWnd();
	SetWindowText(hwnd, "Tetris");												// Titre de jeu 

	/* ouvrir la musique du background*/
	mciSendString("open music\\change.mp3 alia mymusic", NULL, 0, NULL);
	mciSendString("play music\\change.mp3 repeat", NULL, 0, NULL);


	srand((unsigned)time(NULL));												// pour fabriquer un chiffre random 
	IMAGE img(640, 480);
	loadimage(&img, "Pic\\1-110501012153.jpg");
	SetWorkingImage(&img);
	showScore();
	showLevel();
	/*disposer GUIDE*/
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



	/*d�finir la r�gion pour jouer*/
	rectangle((WIDTH + 1)*SIZE - 1, SIZE - 1, (2 * WIDTH + 1)*SIZE, (HEIGHT + 1)*SIZE);
	rectangle(439, 19, 520, 100);
	SetWorkingImage();
	putimage(0, 0, &img);
	/* commencer le jeux*/
	NewGame();


}
void Tetris::NewGame()
{

	setfillcolor(BLACK);
	bar((WIDTH + 1)*SIZE, SIZE, (2 * WIDTH + 1)*SIZE - 1, (HEIGHT + 1)*SIZE - 1);            // cr�er un rectangle noir 
	ZeroMemory(game_area, WIDTH*HEIGHT);


	/* commencer � cr�er des blocs*/
	g_NextBlock.id = rand() % 7;
	g_NextBlock.dir = rand() % 4;
	g_NextBlock.x = 11;
	g_NextBlock.y = 21;

	/* obtenir le bloc prochain */
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
		Quit();


}
void Tetris::Quit()
{
	closegraph();
	exit(0);
}		
CMD Tetris::GetCmd()
{
	while (1)
	{
		// le bloc va tomber chaque 0.5s si il y a aucune op�ration 
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
			case 'P':
			case 'p':  return CMD_STOP;
			case 27:   return CMD_QUIT;
			case ' ':  return CMD_SINK;
			case 0:									//speciale
			case 0xE0:								//speciale
				switch (_getch())					//recevoir le commande prochain
				{
				case 72:	return CMD_ROTATE;      //up
				case 75:	return CMD_LEFT;        //left
				case 77:	return CMD_RIGHT;       //right
				case 80:	return CMD_DOWN;        //down
				}
			}

		}
		Sleep(20);
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
	}
}
void Tetris::NewBlock()
{
	g_CurBlock.id = g_NextBlock.id, g_NextBlock.id = rand() % 7;
	g_CurBlock.dir = g_NextBlock.dir, g_NextBlock.dir = rand() % 4;
	g_CurBlock.x = 3;
	g_CurBlock.y = 24;

	WORD c = g_blocks[g_CurBlock.id].dir[g_CurBlock.dir];
	while ((c & 0X000F) == 0)
	{
		--g_CurBlock.y;
		c >>= 4;
	}

	/* cr�er un bloc */
	DrawBlock(g_CurBlock);

	/* cr�er le bloc prochain */
	setfillcolor(BLACK);
	bar(440, 20, 519, 99);
	DrawBlock(g_NextBlock);

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
	case SHOW: color = g_blocks[_block.id].color; break;
	case HIDE: color = BLACK;	break;
	case FIX: color = g_blocks[_block.id].color / 3; break;
	}
	setfillcolor(color);

	for (int i = 0; i < 16; i++)                                        //scanner une r�gion de ��*�� et dessiner le bloc 
	{
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
			if ((y < HEIGHT) && (game_area[x][y]))					//trop haut
				return false;

		}
		b <<= 1;
	}
	return true;
}
void Tetris::OnRotate()
{

	int distance;
	BlockInfo temp = g_CurBlock;
	temp.dir++;              if (CheckBlock(temp)) { distance = 0; goto rotate; }			// rotation sans bouger
	temp.x = g_CurBlock.x - 1;   if (CheckBlock(temp)) { distance = -1; goto rotate; }		// bouger � gauche pour un unit� et apr�s tourner 
	temp.x = g_CurBlock.x + 1;   if (CheckBlock(temp)) { distance = 1; goto rotate; }		// bouger � droite pour un unit� et apr�s tourner
	temp.x = g_CurBlock.x - 2;   if (CheckBlock(temp)) { distance = -2; goto rotate; }		// bouger � gauche pour 2 unit�s et apr�s tourner
	temp.x = g_CurBlock.x + 2;   if (CheckBlock(temp)) { distance = 2; goto rotate; }		// bouger � droite pour 2 unit�s et apr�s tourner
	return;

	//rotation:tuer le bloc � l'instant et cr�er un nouveau bloc(qui est 'tourn�') 
rotate:
	DrawBlock(g_CurBlock, HIDE);
	++g_CurBlock.dir;
	g_CurBlock.x += distance;
	DrawBlock(g_CurBlock);

}
void Tetris::OnLeft()
{
	BlockInfo temp = g_CurBlock;
	--temp.x;

	if (CheckBlock(temp))
	{
		DrawBlock(g_CurBlock, HIDE);
		--g_CurBlock.x;
		DrawBlock(g_CurBlock);
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
		DrawBlock(g_CurBlock);
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
		DrawBlock(g_CurBlock);
	}
	else OnSink();
}
void Tetris::OnSink()
{
	int i, x, y;
	int count = 0;
	/* continuer � bouger au dessous */
	DrawBlock(g_CurBlock, HIDE);
	BlockInfo temp = g_CurBlock;
	--temp.y;
	while (CheckBlock(temp))									//assurer que le bloc peut bouger 
	{
		--g_CurBlock.y;
		--temp.y;
	}
	DrawBlock(g_CurBlock, FIX);									//continue � bouger au dessus si possible

	/* mettre le bloc sur la place finale */
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
				game_area[g_CurBlock.x + i % 4][g_CurBlock.y - i / 4] = 1;
		}

		b <<= 1;
	}

	/* confirmer est-ce que on doit supprimer un ou plusieurs lignes*/
	int row[4] = { 0 };
	bool bRow = false;
	for (y = g_CurBlock.y; y >= max(g_CurBlock.y - 3, 0); y--)
	{
		i = 0;
		for (x = 0; x < WIDTH; x++)
			if (game_area[x][y] == 1)
				i++;
		if (i == WIDTH)
		{
			bRow = true;
			row[g_CurBlock.y - y] = 1;						// marquer le ligne
			setfillcolor(WHITE);
			bar(220, (HEIGHT - y - 1)*SIZE + SIZE / 2 - 2 + 20, WIDTH * SIZE - 1 + 220, (HEIGHT - y - 1) * SIZE + SIZE / 2 + 2 + 20);
		}
	}

	if (bRow)
	{
		Sleep(200);
		/* supprimer le ligne marqu� */
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
					}
				getimage(&img, 220, 20, WIDTH * SIZE, (HEIGHT - (g_CurBlock.y - i + 1)) * SIZE);
				putimage(220, SIZE + 20, &img);
			}
		}
		/* calculer le score et le niveaux de difficult�*/
		score += count * 10;
		showScore();
		level = score / 100 + 1;					          //le niveau est incr�ment� tanque tout les 100 points de score 
		showLevel();
	}
	NewBlock();												  // cr�er un nouveau bloc
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
		InputBox(S.name, 10, "Votre pr�nom");					// ajouter le pr�nom de joueur 
		S.grades = score;
		fwrite(&S, sizeof(S), 1, fp);


		break;
	} while (1);
	fclose(fp);

}
int	 Tetris::readfile(Score_list &S)
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
void Tetris::ScoreTop()
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
	while (_getch() != 'p'&&_getch() != 'P')
		;
}



