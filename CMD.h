#pragma once
/*
L'ensmeble de commande possible donné par jueurs 
*/
enum CMD
{
	CMD_ROTATE,						 // rotation 
	CMD_LEFT, CMD_RIGHT, CMD_DOWN,	 // bouger 
	CMD_SINK,				         // aller tous droite a la base 
	CMD_QUIT,						 // quitter 
	CMD_STOP,                        // pause
	CMD_RETOUR                       //return
};


