#pragma once
#include <graphics.h>			// Fonctionne sur windows. L'installation de EasyX est obligatoire.

/*
Cette une structure que réprésente l'ensemble de tetrominos possible dans le jeu.
*/
struct Block
{
	WORD dir[4];                //structure des blocs
	COLORREF  color;            //couleurs des bloc 
};

//WORD: UINT16
//typedef DWORD   COLORREF;