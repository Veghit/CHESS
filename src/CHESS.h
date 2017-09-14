#ifndef CHESS_H_
#define CHESS_H_

#include <stdbool.h>


const int BLACK = 0;
const int WHITE = 1;
const char EMPTY = '_';
///////////////////// game default settings

int DIFF = 2;
int PLAYERS = 1;
int USER_COLOR = 1; // white begins

/////////////////////

typedef enum {
	SET_GAME_MODE,
	SET_DIFFICULTY,
	SET_USER_COLOR,
	SET_LOAD,
	SET_DEFAULT,
	SET_PRINT_SETTING,
	SET_INVALID_LINE,
	SET_QUIT,
	SET_START
} SET_COMMAND;

typedef enum {
	GAME_MOVE,
	GAME_GET_MOVES,
	GAME_SAVE,
	GAME_UNDO,
	GAME_RESET,
	GAME_QUIT,
	GAME_INVALID_LINE,
} GAME_COMMAND;

typedef struct game_command_t {
	GAME_COMMAND cmd;
	bool validArg; //is set to true if the line contains a valid argument
	int arg1;
	int arg2;
} GameCommand;

GameCommand game_parse(const char* str);

//a new type that is used to encapsulate a parsed line
typedef struct command_t {
	SET_COMMAND cmd;
	bool validArg; //is set to true if the line contains a valid argument
	int arg;
} SetCommand;

/**
 checks if 2 strings are equal
 @param s1- first string to check
 @param s2- second string to check
 @return
 false if by some character the strings differ
 true if they are exactly the same
 */
bool equalStrings(const char* s1, const char *s2);
SetCommand setting_parse(const char* str);
SET_COMMAND game_settings();

typedef struct game_t {
	char gameBoard[8][8];
	int currentPlayer;
	int historySize;
	int* boardHistory;
	int saves;
} Game;

void printBoard(Game* g);
int getValidMoves(Game* g, int arg, int* validMoves);

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

int getNextWord(const char * s, char * word);

void resetGame(Game * g);
GAME_COMMAND twoPlayersGame(Game* g, char* moveStr);
bool isGameTied(Game* g);
int isGameChecked(Game* g);
bool currentLost(Game * g);
#endif
