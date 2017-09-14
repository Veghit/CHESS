/*
 ============================================================================
 Name        : CHESS.c
 Author      : Itay & Sapir
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "CHESS.h"

int main(int argc, char *argv[]) {

	char mode = 'c'; // console mode
	if ((argc == 2) && (argv[1][0] == 'g'))
		mode = 'g'; // gui mode
	if (mode == 'g') {
		printf("graphical mode unsupported yet.");
		return 0;
	}
	char * moveStr = (char*) calloc(1024, sizeof(char));
	GAME_COMMAND gameCmd = GAME_MOVE;
	SET_COMMAND setCmd=SET_INVALID_LINE;
	Game * g = (Game*) calloc(1, sizeof(Game));
	resetGame(g);
	while (true) {
		if ((gameCmd == GAME_QUIT) || (setCmd == SET_QUIT)) {
			printf("Exiting...\n");
			return 0;
		} else if ((setCmd == SET_START) && gameCmd != GAME_RESET) {
			if (PLAYERS == 1) {
				printf("unsupported yet");
				return 0;
			}
			if (isGameTied(g)) {
				printf("The game is tied\n");
				return 0;
			}
			int check = isGameChecked(g);
			if (check == WHITE) {
				if (currentLost(g)) {
					printf("BLACK WON!");
					return 0;
				} else {
					printf("Check: white King is threatend!\n");
				}
			}
			if (check == BLACK) {
				if (currentLost(g)) {
					printf("white wins the game");
					return 0;
				} else {
					printf("Check: black King is threatend!\n");
				}

			}
			gameCmd = twoPlayersGame(g, moveStr);
		} else if (gameCmd == GAME_RESET) {
			printf("Restarting...\n");
			resetGame(g);
			gameCmd = GAME_MOVE;
			setCmd = game_settings();
		} else {
			setCmd = game_settings();
		}
	}

	return 0;
}
/*
/*
what's the purpose of that?

bool currentLost(Game * g) {
	return false;
}

// it there a CHECK?
int isGameChecked(Game* g) {
	int a = 0;
	int moves[64];
	int king_index = -10;
	if (g->currentPlayer == WHITE) {
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
						return WHITE;
				}
			}
		}

	} else {
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
						return BLACK;
				}
			}
		}

	}
	return -1;
}

bool isGameTied(Game* g) {
	int a = 0;
	int moves[64];
	for (a = 0; a < 64; a++) {

		if (g->currentPlayer == WHITE) {
			if (isWhite(g->gameBoard[a / 8][a % 8])
					&& (getValidMoves(g, a, moves) > 0)) {
				return false;
			}
		} else {
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
	if (g->currentPlayer == WHITE)
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
			if (g->currentPlayer == WHITE && isWhite(piece)) {
				if (-1!=makeMove(g, move.arg1, move.arg2)){
					g->currentPlayer = BLACK;}
				else{
					printf("Illegal move\n");
				}

			} else if (g->currentPlayer == BLACK && isBlack(piece)) {
				if (-1!=makeMove(g, move.arg1, move.arg2)){
					g->currentPlayer = BLACK;}
				else{
					printf("Illegal move\n");
				}
			} else {
				printf("The specified position does not contain your piece\n");
			}
		} else {
			printf("Invalid position on the board\n");
		}

	}
	return move.cmd;
}

void resetGame(Game * g) {
	g->currentPlayer = WHITE;
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
	} else
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
	char color = BLACK;
	if (isWhite(g->gameBoard[row][col]))
		color = WHITE;
	if (color == WHITE) {
		// first white Pawn move
		if (row == 1)
			i = appendMoveIfEmpty(g, row + 2, col, moves, i);
		//regular white pawn move
		i = appendMoveIfEmpty(g, row + 1, col, moves, i);
		// white pawn right capture
		i = appendMoveIfConquerable(g, row + 1, col + 1, color, moves, i);
		//white pawn left capture
		i = appendMoveIfConquerable(g, row + 1, col - 1, color, moves, i);
	} else {
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
	char color = BLACK;
	if (isWhite(g->gameBoard[row][col]))
		color = WHITE;
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
	char color = BLACK;
	if (isWhite(g->gameBoard[row][col]))
		color = WHITE;
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
	char color = BLACK;
	if (isWhite(g->gameBoard[row][col]))
		color = WHITE;
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
	char color = BLACK;
	if (isWhite(g->gameBoard[row][col]))
		color = WHITE;
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
bool isColor(char c, int color) {
	return ((isWhite(c) && (color == WHITE)) || (isBlack(c) && (color == BLACK)));
}
bool isBlack(char c) {
	return (c >= 'A') && (c <= 'Z');
}
bool isWhite(char c) {
	return (c >= 'a') && (c <= 'z');
}
bool isEmpty(Game* g, int row, int col) {
	if ((row < 8) && (row >= 0) && (col >= 0) && (col < 8)) {
		return g->gameBoard[row][col] == '_';
	} else
		return false;
}
bool isConquerable(Game * g, int row, int col, int color) {
	if ((row < 8) && (row >= 0) && (col >= 0) && (col < 8)) {
		char curr = g->gameBoard[row][col];
		//if ((curr == 'K') || (curr == 'k'))
		//	return false;
		return ~isColor(curr, color);
	} else
		return false;
}
bool isEmptyOrConquerable(Game* g, int row, int col, int color) {
	return isEmpty(g, row, col) || isConquerable(g, row, col, color);
}
SET_COMMAND game_settings() {

	SetCommand sp;
	char * string = (char*) calloc(1024, sizeof(char));
	printf(
			"Specify game setting or type 'start' to begin a game with the current setting:\n");
	while (true) {

		fgets(string, 1024, stdin);
		sp = setting_parse(string);
		if (sp.cmd == SET_INVALID_LINE) {
		}
		if (sp.cmd == SET_START) {
			return sp.cmd;
		}
		if (sp.cmd == SET_GAME_MODE) {
			if (sp.validArg) {
				if (sp.arg == 1) {
					PLAYERS = 1;
					printf("Game mode is set to 1 player\n");

				} else {
					PLAYERS = 2;
					printf("Game mode is set to 2 players\n");
				}
			} else {
				printf("Wrong game mode\n");
			}
		}
		if ((sp.cmd == SET_DIFFICULTY) && (PLAYERS == 1)) {
			if (sp.validArg) {
				if (sp.arg == 5) {
					printf(
							"Expert level not supported, please choose a value between 1 to 4:\n");
				} else {
					DIFF = sp.arg;
				}
			} else
				printf(
						"Wrong difficulty level. The value should be between 1 to 5\n");
		}
		if ((sp.cmd == SET_USER_COLOR) && (PLAYERS == 1) && (sp.validArg)) {
			USER_COLOR = sp.arg;
		}
		if (sp.cmd == SET_DEFAULT) {
			USER_COLOR = 1;
			DIFF = 2;
			PLAYERS = 1;
		}
		if (sp.cmd == SET_QUIT) {
			return sp.cmd;
		}
		if (sp.cmd == SET_PRINT_SETTING) {
			if (PLAYERS == 1) {
				printf("SETTINGS:\nGAME_MODE: 1\n");
			} else {
				printf(
						"SETTINGS:\nGAME_MODE: 2\nDIFFICULTY_LVL: %d\nUSER_CLR: %d\n",
						DIFF, USER_COLOR);
			}
		}
	}
	return sp.cmd;
}

SetCommand setting_parse(const char* str) {
	char w1[30];
	char w2[30];
	for (int a = 0; a < 30; a++) {
		w1[a] = 0;
		w2[a] = 0;
	}
	SetCommand sp;
	SET_COMMAND cmd = SET_INVALID_LINE;
	char *s1 = "game_mode";
	char *s2 = "difficulty";
	char *s3 = "user_color";
	char *s4 = "load";
	char *s5 = "default";
	char *s6 = "print_setting";
	char *s7 = "quit";
	char *s8 = "start";
	int index = getNextWord(str, w1);
	sp.validArg = false;
	index += getNextWord(&str[index], w2);
	if (equalStrings(w1, s1)) {
		cmd = SET_GAME_MODE;
		if (equalStrings(w2, "1") || equalStrings(w2, "2")) {
			sp.validArg = true;
			sp.arg = w2[0] - '0';
		}
	} else if (equalStrings(w1, s2)) {
		if (equalStrings(w2, "1") || equalStrings(w2, "2")
				|| equalStrings(w2, "3") || equalStrings(w2, "4")
				|| equalStrings(w2, "5")) {
			sp.validArg = true;
			sp.arg = w2[0] - '0';
		}
		cmd = SET_DIFFICULTY;
	} else if (equalStrings(w1, s3)) {
		cmd = SET_USER_COLOR;
		if (equalStrings(w2, "1") || equalStrings(w2, "0")) {
			sp.validArg = true;
			sp.arg = w2[0] - '0';
		}
	} else if (equalStrings(w1, s4)) {
		cmd = SET_LOAD;
	} else if (equalStrings(w1, s5)) {
		cmd = SET_DEFAULT;
	} else if (equalStrings(w1, s6)) {
		cmd = SET_PRINT_SETTING;
	} else if (equalStrings(w1, s7)) {
		cmd = SET_QUIT;
	} else if (equalStrings(w1, s8)) {
		cmd = SET_START;
	}
	sp.cmd = cmd;
	return sp;
}

GameCommand game_parse(const char* str) {
	char w1[30];
	char w2[30];
	char w3[30];
	char w4[30];
	for (int a = 0; a < 30; a++) {
		w1[a] = 0;
		w2[a] = 0;
		w3[a] = 0;
		w4[a] = 0;
	}
	GameCommand sp;
	GAME_COMMAND cmd = GAME_INVALID_LINE;
	char *s1 = "move";
	char *s2 = "get_moves";
	char *s3 = "save";
	char *s4 = "undo";
	char *s5 = "reset";
	char *s6 = "quit";
	char *s1_2 = "to";
	int index = getNextWord(str, w1);
	sp.validArg = false;
	index += getNextWord(&str[index], w2);
	index += getNextWord(&str[index], w3);
	index += getNextWord(&str[index], w4);
	if (equalStrings(w1, s1) && equalStrings(w3, s1_2)) {
		int row1 = w2[1] - '1';
		int row2 = w4[1] - '1';
		int col1 = w2[3] - 'A';
		int col2 = w4[3] - 'A';
		sp.arg1 = 8 * row1 + col1;
		sp.arg2 = 8 * row2 + col2;
		if ((row1 >= 0) && (row1 < 8) && (col1 >= 0) && (col2 >= 0)
				&& (row2 >= 0) && (row2 < 8) && (col1 < 8) && (col2 < 8))
			sp.validArg = true;
		cmd = GAME_MOVE;
	} else if (equalStrings(w1, s2)) {
		cmd = GAME_GET_MOVES;
	} else if (equalStrings(w1, s3)) {
		cmd = GAME_SAVE;
	} else if (equalStrings(w1, s4)) {
		cmd = GAME_UNDO;
	} else if (equalStrings(w1, s5)) {
		cmd = GAME_RESET;
	} else if (equalStrings(w1, s6)) {
		cmd = GAME_QUIT;
	}
	sp.cmd = cmd;
	return sp;
}

/*
 gets the next word
 @param s- the string from which the next word is taken
 @param word- an empty string in which the next word is written to
 @return
 the last index of s, from which the word was taken
 
int getNextWord(const char * s, char * word) {
	int i = 0;
	int j = 0;
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r') {
		i++;
	}
	while (s[i] != 0 && s[i] != ' ' && s[i] != '\t' && s[i] != '\n'
			&& s[i] != '\r') {
		word[j] = s[i];
		i++;
		j++;
	}
	word[j] = 0;
	if (j == 0)
		return 0;
	return i;
}

/*
 checks if 2 strings are equal
 @param s1- first string to check
 @param s2- second string to check
 @return
 false if by some character the strings differ
 true if they are exactly the same
 
bool equalStrings(const char * s1, const char * s2) {
	for (; *s1 == *s2; ++s1, ++s2) {
		if (*s1 == 0)
			return true;
	}
	if ((*s1 == 0) && (*s2 == 10))
		return true;
	else if ((*s2 == 0) && (*s1 == 10))
		return true;
	return false;
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

*/
