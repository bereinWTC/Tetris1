#include "Tetris.h"

//définir le score et le niveau de difficulté 
int score = 0;
int level = 1;

Pai user;                     // définir user pour stocker le score et nom de joueur


void main()
{
	WelcomeMenu();
	_getch();
	closegraph();

}

//Menu au début
void WelcomeMenu()
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
		case WM_LBUTTONDOWN:                      // opération de la souris 
			if (m.x >= 268 && m.x <= 392 && m.y >= 268 && m.y <= 312)
			{
				flag = 1;
				break;
			}
			else if (m.x >= 268 && m.x <= 392 && m.y >= 328 && m.y <= 370)
			{
				flag = 2;
				break;
			}
			else if (m.x>268 && m.x <= 392 && m.y >= 388 && m.y <= 430)
			{
				flag = 3;

				break;
			}

		}
		if (flag != 0) break;
	}
	if (flag == 1) {
		Init();
		CMD  c;
		while (1)
		{
			// recevoir et exécuter le commande 
			c = GetCmd();
			DispatchCmd(c);
			// ouvrir un fenêtre pour assurer quitter 
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

// initialiser
void Init()
{


	initgraph(640, 480);

	HWND hwnd = GetHWnd();                           
	SetWindowText(hwnd, "Tetris");      // Titre de jeu 

											  // ouvrir la musique du background 
	mciSendString("open music\\change.mp3 alia mymusic", NULL, 0, NULL);
	mciSendString("play music\\change.mp3 repeat", NULL, 0, NULL);


	srand((unsigned)time(NULL));    // pour fabriquer un chiffre random 
	IMAGE img(640, 480);
	loadimage(&img, "Pic\\1-110501012153.jpg");
	SetWorkingImage(&img);
	showScore();
	showLevel();
	// disposer GUIDE
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



	// définir la région pour jouer 
	rectangle((WIDTH + 1)*SIZE - 1, SIZE - 1, (2 * WIDTH + 1)*SIZE, (HEIGHT + 1)*SIZE);
	rectangle(439, 19, 520, 100);
	SetWorkingImage();
	putimage(0, 0, &img);
	// commencer le jeux
	NewGame();


}


void NewGame()
{

	setfillcolor(BLACK);
	bar((WIDTH + 1)*SIZE, SIZE, (2 * WIDTH + 1)*SIZE - 1, (HEIGHT + 1)*SIZE - 1);            // créer un rectangle noir 
	ZeroMemory(game_area, WIDTH*HEIGHT);                                       


																			   // commencer à créer des blocs
	g_NextBlock.id = rand() % 7;
	g_NextBlock.dir = rand() % 4;
	g_NextBlock.x = 11;
	g_NextBlock.y = 21;

	// obtenir le bloc prochain 
	NewBlock();
}

//GAMEOVER
void GameOver()
{
	HWND wnd = GetHWnd();
	writefile(user);
	Paihang(user);
	if (MessageBox(wnd, "GAME OVER \nJouer encore une fois?", "GAME OVER", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{

		NewGame();
		score = 0;
		showScore();
	}
	else
		Quit();


}

//quitter
void Quit()
{
	closegraph();
	exit(0);
}


DWORD  oldtime;            // contr?le de temp pour charge opération 
						   // recevoir lès commandes
CMD GetCmd()
{
	while (1)
	{
		// le bloc va tomber chaque 0.5s si il y a aucune opération 
		DWORD newtime = GetTickCount();       

		if (newtime - oldtime >= 600 - level * 50)  
		{
			oldtime = newtime;
			return CMD_DOWN;
		}
		if (_kbhit())                            // check the command
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
			case 0:                           //speciale
			case 0xE0:                        //speciale
				switch (_getch())               //recevoir le commande prochain
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


void DispatchCmd(CMD _cmd)
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


/*Opération des blocs*/


void NewBlock()
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

	// créer un bloc 
	DrawBlock(g_CurBlock);

	// créer le bloc prochain 
	setfillcolor(BLACK);
	bar(440, 20, 519, 99);
	DrawBlock(g_NextBlock);

	// les blocs tombent automatiquement
	oldtime = GetTickCount();

}



void DrawBlock(BlockInfo _block, DRAW _draw)
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

	for (int i = 0;i<16;i++)                                        //对一个方块区域内16个像素扫描
	{
		if (b & 0x8000)
		{
			x = _block.x + i % 4;
			y = _block.y - i / 4;
			if (y<HEIGHT)
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

// 检测指定方块是否可以放下
bool CheckBlock(BlockInfo _block)
{
	WORD b = g_blocks[_block.id].dir[_block.dir];
	int x, y;

	for (int i = 0;i<16;i++)
	{
		if (b & 0x8000)
		{
			x = _block.x + i % 4;
			y = _block.y - i / 4;
			if ((x<0) || (x >= WIDTH) || (y<0))                  //若方块坐标超出宽度，返回false表示放不下
				return false;
			if ((y<HEIGHT) && (game_area[x][y]))             //若方块坐标超出高度，返回false表示放不下
				return false;

		}
		b <<= 1;
	}
	return true;
}


// Rotation
void OnRotate()
{
	// 获取可以旋转的偏移量（X）
	int distance;
	BlockInfo temp = g_CurBlock;
	temp.dir++;              if (CheckBlock(temp)) { distance = 0; goto rotate; }      // 当变换到下一个形状时直接旋转
	temp.x = g_CurBlock.x - 1;   if (CheckBlock(temp)) { distance = -1; goto rotate; }     // x坐标左移一格再旋转
	temp.x = g_CurBlock.x + 1;   if (CheckBlock(temp)) { distance = 1; goto rotate; }      // x坐标右移移一格再旋转
	temp.x = g_CurBlock.x - 2;   if (CheckBlock(temp)) { distance = -2; goto rotate; }     // x坐标左移两格再旋转
	temp.x = g_CurBlock.x + 2;   if (CheckBlock(temp)) { distance = 2; goto rotate; }      // x坐标右移两格再旋转
	return;

	//rotation:tuer le bloc à l'instant et créer un nouveau bloc(qui est 'tourné') 
rotate:
	DrawBlock(g_CurBlock, HIDE);  
	++g_CurBlock.dir;
	g_CurBlock.x += distance;
	DrawBlock(g_CurBlock);       

}


void OnLeft()
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



void OnRight()
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


void OnDown()
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



void OnSink()
{
	int i, x, y;
	int count = 0;
	// 连续下移
	DrawBlock(g_CurBlock, HIDE);
	BlockInfo temp = g_CurBlock;
	--temp.y;
	while (CheckBlock(temp))          // 判断下方能否移动
	{
		--g_CurBlock.y;
		--temp.y;
	}
	DrawBlock(g_CurBlock, FIX);      //直至不能下移，填充到底部

									 // 固定方块在游戏区
	WORD b = g_blocks[g_CurBlock.id].dir[g_CurBlock.dir];
	for (i = 0; i < 16; i++)
	{
		if (b & 0x8000)
		{
			if (g_CurBlock.y - i / 4 >= HEIGHT)
			{	// 如果方块的固定位置超出高度，结束游戏
				GameOver();
				return;
			}
			else
				game_area[g_CurBlock.x + i % 4][g_CurBlock.y - i / 4] = 1;
		}

		b <<= 1;
	}

	// 检查是否需要消掉行，并标记
	int row[4] = { 0 };
	bool bRow = false;
	for (y = g_CurBlock.y; y >= max(g_CurBlock.y - 3, 0);y--)
	{
		i = 0;
		for (x = 0; x<WIDTH;x++)
			if (game_area[x][y] == 1)
				i++;
		if (i == WIDTH)
		{
			bRow = true;
			row[g_CurBlock.y - y] = 1;    // 标记行
			setfillcolor(WHITE);
			bar(220, (HEIGHT - y - 1)*SIZE + SIZE / 2 - 2 + 20, WIDTH * SIZE - 1 + 220, (HEIGHT - y - 1) * SIZE + SIZE / 2 + 2 + 20);
		}
	}

	if (bRow)
	{
		// 延时200毫秒
		Sleep(200);
		// 擦掉刚才标记的行
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
		}// 计算分数、等级
		score += count * 10;
		showScore();
		level = score / 100 + 1;
		showLevel();
	}

	// 产生新的方块
	NewBlock();
}




// 游戏结束后把游戏所得分数保存
void writefile(Pai &P)
{


	FILE *fp;
	fopen_s(&fp,"data_storage\\ScoreTop.rec", "ab");
	if (!fp)
	{
		printf("file cannot be opened");
		exit(1);
	}
	do
	{
		InputBox(P.name, 10, "Votre prenom：");      // 弹出文本框提示输入姓名
		P.grades = score;
		fwrite(&P, sizeof(P), 1, fp);


		break;
	} while (1);
	fclose(fp);

}

// 读取保存的排行榜信息
int readfile(Pai &P)
{
	int x = 78, y = 70, z = 455, i = 1;
	char ch_grades[5], ch[1], dot = '.';
	FILE *fp;
	fopen_s(&fp,"data_storage\\ScoreTop.rec", "rb");
	if (!fp)
	{
		printf("file cannot be opened");
		exit(1);
	}
	settextstyle(33, 0, "Calibri");
	setbkmode(TRANSPARENT);

	while (fread(&P, sizeof(P), 1, fp) == 1)
	{
		sprintf_s(ch,sizeof(ch), "%d", i);
		outtextxy(x - 22, y, ch);
		outtextxy(x - 11, y, dot);
		outtextxy(x, y, P.name);
		sprintf_s(ch_grades,sizeof(ch_grades), "%g", P.grades);
		outtextxy(z, y, ch_grades);
		y += 35;
		i++;
	}
	fclose(fp);
	return 0;
}

// Pour le liste des scores après jouer
void Paihang(Pai &P)
{
	Pai a[12];
	FILE *fp;
	int length;


	int i, j;
	fopen_s(&fp,"data_storage\\ScoreTop.rec", "rb");

	for (i = 1;fread(&P, sizeof(P), 1, fp) == 1 && i <= 11;i++)
	{

		a[i] = P;
	}

	length = i - 1;

	for (i = 2;i <= length;i++)
		if (a[i].grades<a[i - 1].grades)
		{
			a[0] = a[i];
			a[i] = a[i - 1];
			for (j = i - 2;a[0].grades<a[j].grades;j--)
				a[j + 1] = a[j];
			a[j + 1] = a[0];
		}


	fclose(fp);

	fopen_s(&fp,"data_storage\\ScoreTop.rec", "wb");
	int b = 0;
	for (i = length;b<10 && i >= 1;i--, b++)
	{
		fwrite(&a[i], sizeof(a[i]), 1, fp);
	}
	fclose(fp);
}

// afficher les scores
void ScoreTop()
{
	initgraph(640, 480);
	IMAGE img(640, 480);
	loadimage(&img, "Pic\\Scoretop.jpg");
	SetWorkingImage(&img);
	Pai P;
	readfile(P);
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
				WelcomeMenu();flag = 1;
			}
							 break;
		}if (flag != 0) break;
	}
}


void showScore() {
	char str[10];

	setfillcolor(BLACK);

	rectangle(449, 299, 576, 326);

	bar(450, 300, 575, 325);

	setcolor(WHITE);

	settextstyle(25, 0, "Calibri");

	sprintf_s(str,sizeof(str), "score:%d", score);

	outtextxy(450, 300, str);

}

//niveau de difficulté 
void showLevel()
{
	char str[10];

	setfillcolor(BLACK);

	rectangle(449, 249, 576, 276);

	bar(450, 250, 575, 275);

	setcolor(WHITE);

	settextstyle(25, 0, "Calibri");

	sprintf_s(str,sizeof(str), "Level:%d", level);

	outtextxy(450, 250, str);

}

// Pause
void DisplayPause()
{
	while (_getch() != 'p'&&_getch() != 'P')
		;
}



