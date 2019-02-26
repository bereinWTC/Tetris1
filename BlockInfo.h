#pragma once
#include <graphics.h>
struct BlockInfo
{
	byte id;											// ID
	char x, y;											// coordonnee
	byte dir : 2;										// 4 directions(cod?en 2 bits)
};