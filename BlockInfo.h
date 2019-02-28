#pragma once
#include <graphics.h>
/*
BlockInfo c'est une information de struct Bloc.
Elle caract�rise un tetromino par son id et dir dans l'ensemble de tetromino 
qui se trouve dans la clas Bloc.De plus, elle donne l'info pour les coordonn�e du tetromino.
*/
struct BlockInfo
{
	byte id;											// ID
	char x, y;											// coordonnee
	byte dir : 2;										// 4 directions(cod?en 2 bits)
};