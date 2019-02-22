#pragma once
#include <graphics.h>
struct BlockInfo
{
	byte id;											// ID
	char x, y;											// coordonnee
	byte dir : 2;										// direction
};