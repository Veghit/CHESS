
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "Game.h"




/** SAPIR
what's the purpose of that?
*/
bool currentLost(Game * g) {
	return false;
}

// it there a CHECK?
/**
@param g- the current game
@ret- white if the white king is checked
		black if the black king is checked
		-1 if the game isn't checked

SAPIR it can be shortened, imo.
*/
int isGameChecked(Game* g) {
	int a = 0;
	int moves[64]; //its like the board flattened into 1 dimensional array
	int king_index = -10;
	//if the current player is white
	if (g->currentPlayer == ((*stg).WHITE)) {
		for (a = 0; a < 64; a++) {
			// we find the location of the white king
			if (g->gameBoard[a / 8][a % 8] == 'k') {
				king_index = a;
				break;
			}
		}
		//we make sure the king isn't threatened by any black piece, in the spot he's currently at
		for (a = 0; a < 64; a++) {// a is like the current spot on the flattened board 
			if (isBlack(g->gameBoard[a / 8][a % 8])) {
				//updates the possible moves array
				int j = getValidMoves(g, a, moves);
				for (int i = 0; i < j; i++) {
					//if one of the possible moves is in the same index as the king's, 
					//then the king is checked
					if (moves[i] == king_index)
						return ((*stg).WHITE);
				}
			}
		}

	}
	//same for the black player.
	else {
		for (a = 0; a < 64; a++) {
			if (g->gameBoard[a / 8][a % 8] == 'K') {
				king_index = a;
				break;
			}
		}
		for (a = 0; a < 64; a++) {
			if (isWhite(g->gameBoard[a / 8][a % 8])) {
				int j = getValidMoves(g, a, moves);
				for (int i = 0; i < j; i++) {
					if (moves[i] == king_index)
						return ((*stg).BLACK);
				}
			}
		}

	}
	return -1;
}

/**
@param c- the character of the piece
@param color- the color of the piece
@ret- true if the character and color of the piece match
		--> capital letter as a sign for black and so is the color of the piece
			or little letter as a sign for white, and so it the color if the piece
		false otherwise

*/
bool isColor(char c, int color) {
	return ((isWhite(c) && (color == (*stg).WHITE)) || (isBlack(c) && (color == (*stg).BLACK)));
}

/**
@param c- character of the piece
@ret- true if it is a capital letter
		that shows it is black, as the sign for a black character is a capital letter.
		false otherwise.

*/
bool isBlack(char c) {
	return (c >= 'A') && (c <= 'Z');
}

/**
@param c- character of the piece
@ret- true if it is an english letter, which isn't a capital
		that shows the piece is white, as is the sign for a white piece is a none-capital
		false otherwise

*/
bool isWhite(char c) {
	return (c >= 'a') && (c <= 'z');
}

/**
@param g- the current game
@ret- true if there are no more valid moves for the current player, for any piece
		false otherwise

*/
bool isGameTied(Game* g) {
	int a = 0;
	int moves[64];
	for (a = 0; a < 64; a++) {

		if (g->currentPlayer == (*stg).WHITE) {
			if (isWhite(g->gameBoard[a / 8][a % 8])
				&& (getValidMoves(g, a, moves) > 0)) {
				return false;
			}
		}
		else {
			if (isWhite(g->gameBoard[a / 8][a % 8])
				&& (getValidMoves(g, a, moves) > 0)) {
				return false;
			}
		}
	}
	return true;
}


/**
@param g- the current game
@param moveStr- regularly an empty string in the length of 1024
@ret-

*/
GAME_COMMAND twoPlayersGame(Game* g, char* moveStr) {
	GameCommand move;
	printBoard(g);
	if (g->currentPlayer == (*stg).WHITE)
		printf("White player - enter your move:\n");
	else
		printf("Black player - enter your move:\n");
	//recieves the desirable move by the user
	fgets(moveStr, 1024, stdin);
	//parses the game command string (aka moveStr) into a proper gameCommand var
	move = game_parse(moveStr);
	if (move.cmd == GAME_MOVE) {
		if (move.validArg) {
			int row1 = move.arg1 / 8;
			int col1 = move.arg1 % 8;
			char piece = g->gameBoard[row1][col1];
			if (g->currentPlayer == (*stg).WHITE && isWhite(piece)) {
				if (-1 != makeMove(g, move.arg1, move.arg2)) {
					g->currentPlayer = (*stg).BLACK;
				}
				else {
					printf("Illegal move\n");
				}

			}
			else if (g->currentPlayer == (*stg).BLACK && isBlack(piece)) {
				if (-1 != makeMove(g, move.arg1, move.arg2)) {
					g->currentPlayer = (*stg).BLACK;
				}
				else {
					printf("Illegal move\n");
				}
			}
			else {
				printf("The specified position does not contain your piece\n");
			}
		}
		else {
			printf("Invalid position on the board\n");
		}

	}
	return move.cmd;
}

/**
@param g- current game
@ret- void
resets the game's settings and board to a new game's settings


*/
void resetGame(Game * g) {
	g->currentPlayer = (*stg).WHITE;
	char arr[64] = { 'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r', 'm', 'm', 'm', 'm',
		'm', 'm', 'm', 'm', '_', '_', '_', '_', '_', '_', '_', '_', '_',
		'_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_',
		'_', '_', '_', '_', '_', '_', '_', '_', '_', '_', 'M', 'M', 'M',
		'M', 'M', 'M', 'M', 'M', 'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R', };
	int x = 0;
	for (x = 0; x <= 63; x++) {
		g->gameBoard[x / 8][x % 8] = arr[x];
	}
}

/**
@param g- the current game
@param arg1- 
@param arg2-
@ret-

*/
int makeMove(Game * g, int arg1, int arg2) {

	int row1 = arg1 / 8;
	int row2 = arg2 / 8;
	int col1 = arg1 % 8;
	int col2 = arg2 % 8;
	char piece = g->gameBoard[row1][col1];
	int validMoves[64];
	int j;
	for (j = 0; j < 64; j++)
		validMoves[j] = -1;
	getValidMoves(g, arg1, validMoves);
	for (j = 0; j < 64; j++) {
		if (validMoves[j] == arg2) {
			break;
			printf("%d", j);
		}
	}
	if (validMoves[j] == arg2) {
		g->gameBoard[row1][col1] = '_';
		g->gameBoard[row2][col2] = piece;
	}
	else
		return -1;
}

/**
@param g- current game
@param arg- the piece's location on a flattened board
@param moves- an existing array of possible moves, usually filled with other pieces possible moves or/and
				0's or -1's at the very least.
@ret- the number of next valid moves for the piece(aka arg) in the game,
		in accordance with the piece's type obviously

*/
int getValidMoves(Game* g, int arg, int* moves) {
	char piece = g->gameBoard[arg / 8][arg % 8];
	int i = 0;
	int row = arg / 8;
	int col = arg % 8;
	switch (piece) {
	case 'M':
	case 'm':
		i = appendPawnMoves(g, row, col, moves, i);
		break;
	case 'R':
	case 'r':
		i = appendRookMoves(g, row, col, moves, i);
		break;
	case 'N':
	case 'n':
		i = appendKnightMoves(g, row, col, moves, i);
		break;
	case 'k':
	case 'K':
		i = appendKingMoves(g, row, col, moves, i);
		break;
	case 'Q':
	case 'q':
		i = appendRookMoves(g, row, col, moves, i); //rook here intentionally queen is rook+bishop
		i = appendBishopMoves(g, row, col, moves, i);
		break;
	case 'b':
	case 'B':
		i = appendBishopMoves(g, row, col, moves, i);
		break;
	}
	return i;
}

/**
@param g- current game
@param row- current row of the piece
@param col- current column of the piece
@param moves- an existing array of possible moves, usually filled with other pieces possible moves or/and
				0's or -1's at the very least.
@param i- the next empty/ fillable location in the array
@ret- number of possible moves for the pawn from it's current place

*/
int appendPawnMoves(Game *g, int row, int col, int* moves, int i) {
	char color = (*stg).BLACK;
	if (isWhite(g->gameBoard[row][col]))
		color = (*stg).WHITE;
	if (color == (*stg).WHITE) {
		// first white Pawn move
		if (row == 1)
			i = appendMoveIfEmpty(g, row + 2, col, moves, i);
		//regular white pawn move
		i = appendMoveIfEmpty(g, row + 1, col, moves, i);
		// white pawn right capture
		i = appendMoveIfConquerable(g, row + 1, col + 1, color, moves, i);
		//white pawn left capture
		i = appendMoveIfConquerable(g, row + 1, col - 1, color, moves, i);
	}
	else {
		// first black Pawn move
		if (row == 6)
			i = appendMoveIfEmpty(g, row - 2, col, moves, i);
		//regular black pawn move
		i = appendMoveIfEmpty(g, row - 1, col, moves, i);
		// black pawn right capture
		i = appendMoveIfConquerable(g, row - 1, col + 1, color, moves, i);
		// black pawn left capture
		i = appendMoveIfConquerable(g, row - 1, col - 1, color, moves, i);

	}
	return i;
}

/**
@param g- current game
@param row- current row of the piece
@param col- current column of the piece
@param moves- an existing array of possible moves, usually filled with other pieces possible moves or/and
				0's or -1's at the very least.
@param i- the next empty/ fillable location in the array
@ret- number of possible moves for the rook from it's current place

*/
int appendRookMoves(Game *g, int row, int col, int* moves, int i) {
	char color = (*stg).BLACK;
	if (isWhite(g->gameBoard[row][col]))
		color = (*stg).WHITE;
	//rook UP
	int j = 1;
	while (appendMoveIfEmpty(g, row + j, col, moves, i) > i) {
		i++;
		j++;
	}
	i = appendMoveIfConquerable(g, row + j, col, color, moves, i);
	//Down
	j = 1;
	while (appendMoveIfEmpty(g, row - j, col, moves, i) > i) {
		i++;
		j++;
	}
	i = appendMoveIfConquerable(g, row - j, col, color, moves, i);
	//right
	j = 1;
	while (appendMoveIfEmpty(g, row, col + j, moves, i) > i) {
		i++;
		j++;
	}
	i = appendMoveIfConquerable(g, row, col + j, color, moves, i);
	// left
	j = 1;
	while (appendMoveIfEmpty(g, row, col - j, moves, i) > i) {
		i++;
		j++;
	}
	i = appendMoveIfConquerable(g, row, col - j, color, moves, i);
	return i;
}


/**
@param g- current game
@param row- current row of the piece
@param col- current column of the piece
@param moves- an existing array of possible moves, usually filled with other pieces possible moves or/and
				0's or -1's at the very least.
@param i- the next empty/ fillable location in the array
@ret- number of possible moves for the bishop from it's current place

*/
int appendBishopMoves(Game *g, int row, int col, int* moves, int i) {
	char color = (*stg).BLACK;
	if (isWhite(g->gameBoard[row][col]))
		color = (*stg).WHITE;
	//up right
	int j = 1;
	while (appendMoveIfEmpty(g, row + j, col + j, moves, i) > i) {
		i++;
		j++;
	}
	i = appendMoveIfConquerable(g, row + j, col + j, color, moves, i);
	//down left
	j = 1;
	while (appendMoveIfEmpty(g, row - j, col - j, moves, i) > i) {
		i++;
		j++;
	}
	i = appendMoveIfConquerable(g, row - j, col - j, color, moves, i);
	//up left
	j = 1;
	while (appendMoveIfEmpty(g, row + j, col - j, moves, i) > i) {
		i++;
		j++;
	}
	i = appendMoveIfConquerable(g, row + j, col - j, color, moves, i);
	//down right
	j = 1;
	while (appendMoveIfEmpty(g, row - j, col + j, moves, i) > i) {
		i++;
		j++;
	}
	i = appendMoveIfConquerable(g, row - j, col + j, color, moves, i);
	return i;
}


/**
@param g- current game
@param row- current row of the piece
@param col- current column of the piece
@param moves- an existing array of possible moves, usually filled with other pieces possible moves or/and
				0's or -1's at the very least.
@param i- the next empty/ fillable location in the array
@ret- number of possible moves for the knight from it's current place

*/
int appendKnightMoves(Game *g, int row, int col, int* moves, int i) {
	char color = (*stg).BLACK;
	if (isWhite(g->gameBoard[row][col]))
		color = (*stg).WHITE;
	i = appendMoveIfEmptyOrConquerable(g, row + 2, col - 1, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row + 2, col + 1, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row + 1, col + 2, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row - 1, col + 2, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row - 2, col - 1, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row - 2, col + 1, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row + 1, col - 2, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row - 1, col - 2, color, moves, i);
	return i;
}

/**
@param g- current game
@param row- current row of the piece
@param col- current column of the piece
@param moves- an existing array of possible moves, usually filled with other pieces possible moves or/and
				0's or -1's at the very least.
@param i- the next empty/ fillable location in the array
@ret- number of possible moves for the king from it's current place
*/
int appendKingMoves(Game *g, int row, int col, int* moves, int i) {
	char color = (*stg).BLACK;
	if (isWhite(g->gameBoard[row][col]))
		color = (*stg).WHITE;
	i = appendMoveIfEmptyOrConquerable(g, row + 1, col, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row + 1, col + 1, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row, col + 1, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row - 1, col + 1, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row - 1, col, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row - 1, col - 1, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row, col - 1, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row + 1, col - 1, color, moves, i);
	return i;
}

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
int appendMoveIfEmpty(Game* g, int row, int col, int * moves, int i) {
	if (isEmpty(g, row, col)) {
		moves[i] = 8 * row + col;
		return i + 1;
	}
	return i;
}

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
int appendMoveIfConquerable(Game* g, int row, int col, int color, int * moves, int i) {
	if (isConquerable(g, row, col, color)) {
		moves[i] = 8 * row + col;
		return i + 1;
	}
	return i;
}

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
int appendMoveIfEmptyOrConquerable(Game* g, int row, int col, int color, int * moves, int i) {
	i = appendMoveIfEmpty(g, row, col, moves, i);
	i = appendMoveIfConquerable(g, row, col, color, moves, i);
	return i;
}


/**
@param g- the current game
@param row- a certain row in the game's board
@param col- a certain column in the game's board

@ret- true if in the current spot of the game's board there's no piece
		false otherwise

*/
bool isEmpty(Game* g, int row, int col) {
	if ((row < 8) && (row >= 0) && (col >= 0) && (col < 8)) {
		return g->gameBoard[row][col] == '_';
	}
	else
		return false;
}

/**
@param g- the current game
@param row- a certain row in the game's board 
@param col- a certain column in the game's board
@param color-  the color of the piece that is supposed to make the move
@ret- true if the specified spot of the current game's board is conquerable
		--> there's a different piece color there(than the one we received as param)
		false otherwise

*/
bool isConquerable(Game * g, int row, int col, int color) {
	if ((row < 8) && (row >= 0) && (col >= 0) && (col < 8)) {
		char curr = g->gameBoard[row][col];
		//if ((curr == 'K') || (curr == 'k'))
		//	return false;
		return ~isColor(curr, color);
	}
	else
		return false;
}

/**
@param g- the game
@param row- a certain row in the game's board
@param col- a certain column in the game's board
@param color- the color of the piece that is supposed to make the move
@ret- true if the spot in the current game's board is conquerable/ empty
		and false otherwise.

*/
bool isEmptyOrConquerable(Game* g, int row, int col, int color) {
	return isEmpty(g, row, col) || isConquerable(g, row, col, color);
}


/**
@param g- the game itself (and obviously it's state)
@ret- void
prints the game's state, as in, the board as it is.
*/
void printBoard(Game * g) {
	int i;
	int j;
	for (i = 1; i <= 8; i++) {
		printf("\n%d| ", 9 - i);
		for (j = 0; j < 8; j++)
			printf("%c ", g->gameBoard[8 - i][j]);
		printf("|");
	}
	printf("\n  -----------------\n");
	printf("   A B C D E F G H\n");
}