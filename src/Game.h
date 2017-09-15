#ifndef Game_H_
#define Game_H_
#include <stdbool.h>


#include "GameCommand.h"

typedef struct game_t {
	char gameBoard[8][8];
	int currentPlayer;
	int historySize;
	int* boardHistory;
	int saves;
	//saves if possible to do the certain castling
	bool whiteRightCastle;
	bool whiteLeftCastle;
	bool blackRightCastle;
	bool blackLeftCastle;
} Game;

void printBoard(Game* g);
int getValidMoves(Game* g, int arg, int* validMoves);
bool isColor(char c, int color);
bool isWhite(char);
bool isBlack(char);

bool isEmpty(Game*, int, int);
bool isConquerable(Game * g, int row, int col, int color);
int makeMove(Game * g, int arg1, int arg2);
int appendPawnMoves(Game *g, int row, int col, int* moves, int i);
int appendRookMoves(Game *g, int row, int col, int* moves, int i);
int appendBishopMoves(Game *g, int row, int col, int* moves, int i);
int appendKnightMoves(Game *g, int row, int col, int* moves, int i);
int appendKingMoves(Game *g, int row, int col, int* moves, int i);
int appendMoveIfEmpty(Game* g, int row, int col, int * moves, int i);
int appendMoveIfConquerable(Game* g, int row, int col, int color, int * moves,
	int i);
int appendMoveIfEmptyOrConquerable(Game* g, int row, int col, int color,
	int * moves, int i);

void resetGame(Game * g);
GAME_COMMAND twoPlayersGame(Game* g, char* moveStr);
bool isGameTied(Game* g);
int isGameChecked(Game* g);
bool currentLost(Game * g);
Game * cloneGame(Game* g);
int castle(Game * g, int arg);
#endif //!Game_H_