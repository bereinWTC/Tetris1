#pragma once
#include <graphics.h>			// Fonctionne sur windows. L'installation de EasyX est obligatoire.
struct Block
{
	WORD dir[4];                //structure des blocs
	COLORREF  color;            //couleurs des bloc 
};

//WORD: UINT16