#ifndef Game_H_
#define Game_H_
#include <stdbool.h>
#include "GameCommand.h"

typedef struct game_t {
	char gameBoard[8][8];
	int currentPlayer; // THE ONE TO MAKE THE *NEXT* MOVE
	int saves;
	bool whiteRightCastle;
	bool whiteLeftCastle;
	bool blackRightCastle;
	bool blackLeftCastle;
	int DIFF;
	char mode;
	//number of players. either 1 or 2? SAPIR
	int PLAYERS;
	int USER_COLOR;
} Game;

Game * lastGames[3];

char * lastMoves[6];
/*GENERAL UTILITIES*/

int getColor(char c);
/**
 @param c- the character of the piece
 @param color- the color of the piece
 @ret- true if the character and color of the piece match
 --> capital letter as a sign for black and so is the color of the piece
 or little letter as a sign for white, and so it the color if the piece
 false otherwise

 */
bool isColor(char c, int color);
/**
 @param c- character of the piece
 @ret- true if it is an english letter, which isn't a capital
 that shows the piece is white, as is the sign for a white piece is a none-capital
 false otherwise

 */
bool isWhite(char);
/**
 @param c- character of the piece
 @ret- true if it is a capital letter
 that shows it is black, as the sign for a black character is a capital letter.
 false otherwise.

 */
bool isBlack(char);
/* END OF GENERAL UTILITIES*/

/* GAME UTILITIES*/
/**
 @param g- the game itself (and obviously it's state)
 @ret- void
 prints the game's state, as in, the board as it is.
 */
void printBoard(Game* g);

/**
 @param g- current game
 @ret- void
 resets the game's settings and board to a new game's settings


 */ //TODO
void resetGame(Game * g);

/**
 @param g- the current game
 @ret- true if there are no more valid moves for the current player, for any piece
 false otherwise

 */
bool isGameTied(Game* g);
// it there a CHECK?
/**
 @param g- the current game
 @ret- white if the white king is checked
 black if the black king is checked
 -1 if the game isn't checked
 *///TODO
int isGameChecked(Game* g);

bool currentLost(Game * g);
/**
 @param g- receives a game
 @ret- returns a game which has all the fields value of the g in it's fields
 */

//SAPIR filled out missing things, but what about boardHistory?
//TODO
Game * cloneGame(Game* g);

void deleteGame(Game* g);

/*END OF GAME UTILITIES*/

/* MOVES AND SUPPORTERS*/
/**
 @param g- current game
 @param arg- the piece's location on a flattened board
 @param moves- an existing array of possible moves, usually filled with other pieces possible moves or/and
 0's or -1's at the very least.
 @ret- the number of next valid moves for the piece(aka arg) in the game,
 in accordance with the piece's type obviously

 */
int getValidMoves(Game* g, int arg, int* validMoves, int i);
/**
@param g- the game
@param arg- a piece in the game
simply prints all the valid moves for arg in g*/
void printValidMoves(Game * g, int arg);
/**
 @param g- the current game
 @param arg1- the current place of the piece in the game
 @param arg2- the wannabe destined place of the piece in the game
 @ret- -1 if the move is possible and good/ not illegal,
 a different int, otherwise
 and moves piece to desired location

 */
int makeMove(Game * g, int arg1, int arg2);
/**
 @param g- current game
 @param row- current row of the piece
 @param col- current column of the piece
 @param moves- an existing array of possible moves, usually filled with other pieces possible moves or/and
 0's or -1's at the very least.
 @param i- the next empty/ fillable location in the array
 @ret- number of possible moves for the pawn from it's current place

 */
int appendPawnMoves(Game *g, int row, int col, int* moves, int i);
/**
 @param g- current game
 @param row- current row of the piece
 @param col- current column of the piece
 @param moves- an existing array of possible moves, usually filled with other pieces possible moves or/and
 0's or -1's at the very least.
 @param i- the next empty/ fillable location in the array
 @ret- number of possible moves for the rook from it's current place

 */
int appendRookMoves(Game *g, int row, int col, int* moves, int i);
/**
 @param g- current game
 @param row- current row of the piece
 @param col- current column of the piece
 @param moves- an existing array of possible moves, usually filled with other pieces possible moves or/and
 0's or -1's at the very least.
 @param i- the next empty/ fillable location in the array
 @ret- number of possible moves for the bishop from it's current place

 */
int appendBishopMoves(Game *g, int row, int col, int* moves, int i);
/**
 @param g- current game
 @param row- current row of the piece
 @param col- current column of the piece
 @param moves- an existing array of possible moves, usually filled with other pieces possible moves or/and
 0's or -1's at the very least.
 @param i- the next empty/ fillable location in the array
 @ret- number of possible moves for the knight from it's current place

 */
int appendKnightMoves(Game *g, int row, int col, int* moves, int i);
/**
 @param g- current game
 @param row- current row of the piece
 @param col- current column of the piece
 @param moves- an existing array of possible moves, usually filled with other pieces possible moves or/and
 0's or -1's at the very least.
 @param i- the next empty/ fillable location in the array
 @ret- number of possible moves for the king from it's current place
 */
int appendKingMoves(Game *g, int row, int col, int* moves, int i);
/**
 @param g- the current game
 @param row- a certain row in the game's board
 @param col-a certain column in the game's board
 @param moves- an array which contains all "previous" moves
 @param i- the current empty spot of the moves array
 @ret- if the spot of the game's current board (before the move was made)
 is empty, updates the moves array,
 returns the next empty spot on the moves array
 otherwise, returns the current empty spot (which is just i)
 */
int appendMoveIfEmpty(Game* g, int row, int col, int * moves, int i);
/**
 @param g- the current game
 @param row- a certain row in the game's board
 @param col-a certain column in the game's board
 @param moves- an array which contains all "previous" moves
 @param i- the current empty spot of the moves array
 @ret- if the spot of the game's current board (before the move was made)
 is conquerable, updates the moves array,
 returns the next empty spot on the moves array
 otherwise, returns the current empty spot (which is just i)
 */

int appendMoveIfConquerable(Game* g, int row, int col, int color, int * moves,
		int i);
/**
 @param g- the current game
 @param row- a certain row in the game's board
 @param col- a certain column in the game's board
 @param moves- an array which contains all "previous" moves
 @param i- the current empty spot of the moves array
 @ret- if the spot of the game's current board (before the move was made)
 is empty or conquerable, makes a move and returns the next empty spot in the moves array
 otherwise, simply returns the current empty spot (since the move couldn't have been made)
 */
int appendMoveIfEmptyOrConquerable(Game* g, int row, int col, int color,
		int * moves, int i);

/**
 @param g- the current game
 @param row- a certain row in the game's board
 @param col- a certain column in the game's board

 @ret- true if in the current spot of the game's board there's no piece
 false otherwise

 */
bool isEmpty(Game*, int, int);

/**
 @param g- the current game
 @param row- a certain row in the game's board
 @param col- a certain column in the game's board
 @param color-  the color of the piece that is supposed to make the move
 @ret- true if the specified spot of the current game's board is conquerable
 --> there's a different piece color there(than the one we received as param)
 false otherwise

 */
bool isConquerable(Game * g, int row, int col, int color);

/**
 @param g- the game
 @param row- a certain row in the game's board
 @param col- a certain column in the game's board
 @param color- the color of the piece that is supposed to make the move
 @ret- true if the spot in the current game's board is conquerable/ empty
 and false otherwise.

 */
bool isEmptyOrConquerable(Game* g, int row, int col, int color);
/**
@param g- the game
@param arg- a piece in the game
@ret- true if arg is threatened and false otherwise*/
bool isThreatened(Game * g, int arg);
/**
@param g- the game
@param arg- the place of the rook that is to castle the king
if the player wanted to castle, 
returns 4 if the castling of the white was possible
or 60 if the castling of the black was possible*/
int castle(Game * g, int arg);
/**
@param g- the game
deletes all allocated memory for the game*/
void deleteGame(Game* g);
/**

@param g- game
@param arg- the place of the rook
returns true if the rook can castle the king
false otherwise*/

bool canCastle(Game* g, int arg);

int getColor(char c);
void pawnPromotion(Game * g, int arg);

/* END OF MOVES AND SUPPORTERS*/

/* INCLUDING MINMAX*/
/*
 FROM HERE NEED TO MAKE/ COMPLETE/ CORRECT*/
/**
@param g- the current game
@param moveStr- regularly an empty string in the length of 1024
@ret- GAME_COMMAND of the move that was made, if made correctly,
otherwise, returns a GAME_COMMAND type according to the validity of the move that was to be made

 */
GAME_COMMAND twoPlayersGame(Game* g, char* moveStr);
int exit_game(Game* g, char* moveStr);
/*@param g- game
@param move- game command of most recent move
prints the last move of pc*/
GameCommand pcMove(Game* g);
//TODO finish doc
GameCommand MinimaxSuggestMove(Game* g);

int create_Tree(Game* curGame,int maxDepth, int curDepth,
		int indexChosen[], GameCommand* chosenMove, int alpha, int beta);

void intializeRest(char arr[][17], int len, int size, char* str);
/*
@param g- existing game with existing settings
keeps all the initial settings and resets everything else*/
void restartGame(Game*g);

void listAllMoves(Game* g, GameCommand allPossibleMoves[]);

//void fixStrAndUpdate(char fromTo[][17], int index, char piece, int originRow, int originCol, int goalRow, int goalCol);

void printIntArr(int* arr, int len);

void printCharArr(char* arr, int len);

void initializeSpecial(int* arr, int length, int jump);

int calcLowest(Game* g);

int maxElem(int* arr, int length, int indexChosen[]);

int minElem(int* arr, int length, int indexChosen[]);

Game * game_undo(Game * g);
void xml_save(Game * g, char * str);
void handle_undo(Game * g, Game * tempClone, GameCommand move, char* lastMove);
void print_pcMove(Game* g, GameCommand move);

#endif //!Game_H_
