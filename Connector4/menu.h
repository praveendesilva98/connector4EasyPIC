#ifndef MENU_H
#define	MENU_H

#include <htc.h>

void menu_game();
int player_choice();
void printBoard(char *board);
int takeTurn(char *board, int player, const char *PIECES);
int horizontalCheck(char *board);
int verticalCheck(char *board);
int diagonalCheck(char *board);
int checkWin(char *board);
int checkFour(char *board, int a, int b, int c, int d);
void opening_menu();
int mode_choice();

#endif

