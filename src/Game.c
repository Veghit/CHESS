
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
int isGameChecked(Game* g) {
	int a = 0;
	int moves[64];
	int king_index = -10;
	if (g->currentPlayer == (setting.WHITE)) {
		for (a = 0; a < 64; a++) {
			if (g->gameBoard[a / 8][a % 8] == 'k') {
				king_index = a;
				break;
			}
		}
		for (a = 0; a < 64; a++) {
			if (isBlack(g->gameBoard[a / 8][a % 8])) {
				int j = getValidMoves(g, a, moves);
				for (int i = 0; i < j; i++) {
					if (moves[i] == king_index)
						return (setting.WHITE);
				}
			}
		}

	}
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
						return (setting.BLACK);
				}
			}
		}

	}
	return -1;
}
bool isColor(char c, int color) {
	return ((isWhite(c) && (color == setting.WHITE)) || (isBlack(c) && (color == setting.BLACK)));
}

bool isBlack(char c) {
	return (c >= 'A') && (c <= 'Z');
}
bool isWhite(char c) {
	return (c >= 'a') && (c <= 'z');
}

bool isGameTied(Game* g) {
	int a = 0;
	int moves[64];
	for (a = 0; a < 64; a++) {

		if (g->currentPlayer == setting.WHITE) {
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

GAME_COMMAND twoPlayersGame(Game* g, char* moveStr) {
	GameCommand move;
	printBoard(g);
	if (g->currentPlayer == setting.WHITE)
		printf("White player - enter your move:\n");
	else
		printf("Black player - enter your move:\n");
	fgets(moveStr, 1024, stdin);
	move = game_parse(moveStr);
	if (move.cmd == GAME_MOVE) {
		if (move.validArg) {
			int row1 = move.arg1 / 8;
			int col1 = move.arg1 % 8;
			char piece = g->gameBoard[row1][col1];
			if (g->currentPlayer == setting.WHITE && isWhite(piece)) {
				if (-1 != makeMove(g, move.arg1, move.arg2)) {
					g->currentPlayer = setting.BLACK;
				}
				else {
					printf("Illegal move\n");
				}

			}
			else if (g->currentPlayer == setting.BLACK && isBlack(piece)) {
				if (-1 != makeMove(g, move.arg1, move.arg2)) {
					g->currentPlayer = setting.BLACK;
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

void resetGame(Game * g) {
	g->currentPlayer = setting.WHITE;
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

int appendPawnMoves(Game *g, int row, int col, int* moves, int i) {
	char color = setting.BLACK;
	if (isWhite(g->gameBoard[row][col]))
		color = setting.WHITE;
	if (color == setting.WHITE) {
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
int appendRookMoves(Game *g, int row, int col, int* moves, int i) {
	char color = setting.BLACK;
	if (isWhite(g->gameBoard[row][col]))
		color = setting.WHITE;
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

int appendBishopMoves(Game *g, int row, int col, int* moves, int i) {
	char color = setting.BLACK;
	if (isWhite(g->gameBoard[row][col]))
		color = setting.WHITE;
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

int appendKnightMoves(Game *g, int row, int col, int* moves, int i) {
	char color = setting.BLACK;
	if (isWhite(g->gameBoard[row][col]))
		color = setting.WHITE;
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
int appendKingMoves(Game *g, int row, int col, int* moves, int i) {
	char color = setting.BLACK;
	if (isWhite(g->gameBoard[row][col]))
		color = setting.WHITE;
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
int appendMoveIfEmpty(Game* g, int row, int col, int * moves, int i) {
	if (isEmpty(g, row, col)) {
		moves[i] = 8 * row + col;
		return i + 1;
	}
	return i;
}
int appendMoveIfConquerable(Game* g, int row, int col, int color, int * moves,
	int i) {
	if (isConquerable(g, row, col, color)) {
		moves[i] = 8 * row + col;
		return i + 1;
	}
	return i;
}
int appendMoveIfEmptyOrConquerable(Game* g, int row, int col, int color,
	int * moves, int i) {
	i = appendMoveIfEmpty(g, row, col, moves, i);
	i = appendMoveIfConquerable(g, row, col, color, moves, i);
	return i;
}

bool isEmpty(Game* g, int row, int col) {
	if ((row < 8) && (row >= 0) && (col >= 0) && (col < 8)) {
		return g->gameBoard[row][col] == '_';
	}
	else
		return false;
}
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
bool isEmptyOrConquerable(Game* g, int row, int col, int color) {
	return isEmpty(g, row, col) || isConquerable(g, row, col, color);
}

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