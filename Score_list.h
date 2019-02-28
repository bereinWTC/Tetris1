#pragma once
/*
C'est une structure de donne l'information du nom d'un jueur et son score. 
*/
struct Score_list
{
	char name[20];		// le nom de jueur 
	double grades;		// le score de jueur 
};
typedef struct Score_list Score_list;