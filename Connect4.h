#ifndef _HEADER_H_
#define _HEADER_H_

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

//int centerX, centerY;
#define BOXH 2
#define BOXW 3
#define GAMEROW 6
#define GAMECOL 7
#define MIN -1000
#define MAX 1000


#define NON_P 0
#define P1 1
#define P2 2
#define BOARD 3

#define C_P1 COLOR_RED
#define C_P2 COLOR_BLACK
#define C_BOARD COLOR_BLUE


void cursesStart();
void cursesEnd();

struct game_state {
  unsigned char *board;
  unsigned char turn;
  unsigned char winner;
  bool AI;
  int player_pos;
};

void drawGameDetails(struct game_state *game_state);
void drawMenu();

void drawPiece(int startX, int startY, int posX, int posY, unsigned char player);
void drawPieces(int startX, int startY, unsigned char *board);
void drawBoard(int startX, int startY);
void drawPossible(int startX, int startY, int pos, unsigned char player);

bool placePiece(unsigned char *board, int pos, unsigned char player);
unsigned char checkFor4(int startX, int startY, unsigned char *board, int dirX, int dirY);
unsigned char checkWin(unsigned char *board);
void gameInit(struct game_state *game_state);
float alphabetaFunc(int node, float depth, float alpha, float beta, bool maximizingPlayer, unsigned char *board);
void update(struct game_state *game_state, int ch);
void reDraw(struct game_state *game_state);


#endif