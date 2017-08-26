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
	SET_COMMAND cmd = game_settings();
	GameCommand move;
	if (cmd == SET_START) {
		printf("LET THE GAME BEGIN!");
		Game * g = (Game*) calloc(1, sizeof(Game));
		char * moveStr = (char*) calloc(1024, sizeof(char));
		g->currentPlayer = 'W';
		char arr[64] = { 'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R', 'P', 'P', 'P',
				'P', 'P', 'P', 'P', 'P', '_', '_', '_', '_', '_', '_', '_', '_',
				'_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_',
				'_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', 'p', 'p',
				'p', 'p', 'p', 'p', 'p', 'p', 'r', 'n', 'b', 'q', 'k', 'b', 'n',
				'r', };
		int x = 0;
		for (x = 0; x <= 63; x++) {
			g->gameBoard[x / 8][x % 8] = arr[x];
		}

		while (true) {
			printBoard(g);
			if (g->currentPlayer == 'W')
				printf("White player - enter your move:\n");
			else
				printf("Black player - enter your move:\n");
			fgets(moveStr, 1024, stdin);
			move = game_parse(moveStr);
			if (move.cmd == GAME_MOVE && move.validArg) {
				int row1 = move.arg1 / 8;
				int row2 = move.arg2 / 8;
				int col1 = move.arg1 % 8;
				int col2 = move.arg2 % 8;
				char piece = g->gameBoard[row1][col1];
				if ((g->currentPlayer == 'W') && (piece >= 'A')
						&& (piece <= 'Z')) {
					makeMove(g, move.arg1, move.arg2);

				} else if (g->currentPlayer == 'B' && piece >= 'a'
						&& piece <= 'z') {
				} else
					printf(
							"The specified position does not contain your piece\n");
			} else
				printf("Invalid position on the board\n");
		}

	}
	if (cmd == SET_QUIT) {
		printf("QUIT THE GAME!");
	}
	return 0;
}

void makeMove(Game * g, int arg1, int arg2) {

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
		if (validMoves[j] == arg2)
		{
			break;
			printf("%d",j);
		}
	}
	if (validMoves[j] == arg2) {
		g->gameBoard[row1][col1] = '_';
		g->gameBoard[row2][col2] = piece;
	} else
		printf("Illegal move\n");
}

void getValidMoves(Game* g, int arg, int* validMoves) {
	char piece = g->gameBoard[arg / 8][arg % 8];
	int i = 0;
	int row = arg / 8;
	int col = arg % 8;
	if (g->currentPlayer == 'W') {
		if (piece == 'P') {
			if ((row == 1) && isEmpty(g->gameBoard[row + 2][col])) {
				validMoves[i] = arg + 16;
				i++;
			}
			if ((row <= 6) && isEmpty(g->gameBoard[row + 1][col])) {
				validMoves[i] = arg + 8;
				i++;
			}
			if ((col < 7) && (row < 7)
					&& isBlack(g->gameBoard[1 + row][col + 1])) {
				validMoves[i] = arg + 9;
				i++;
			}
			if ((col > 0) && (row < 7)
					&& isBlack(g->gameBoard[1 + row][col - 1])) {
				validMoves[i] = arg + 7;
				i++;
			}
		}
		if (piece == 'R') {
			//UP
			int j = row+1;
			while (j > 0) {
				if (isWhite(g->gameBoard[j][col]))
					break;

				validMoves[i] = col + 8 * j;
				i++;

				if (isBlack(g->gameBoard[j][col]))
					break;
				j++;

			}
			//Down
			j = row-1;
			while (j < 8) {
				if (isWhite(g->gameBoard[j][col]))
					break;

				validMoves[i] = col + 8 * j;
				i++;

				if (isBlack(g->gameBoard[j][col]))
					break;
				j--;

			}
			j = col+1; //right
			while (j < 8) {
				if (isWhite(g->gameBoard[row][j]))
					break;

				validMoves[i] = 8 * row + j;
				i++;

				if (isBlack(g->gameBoard[row][j]))
					break;
				j++;

			}
			j = col-1;
			while (j < 8) {
				if (isWhite(g->gameBoard[row][j]))
					break;

				validMoves[i] = 8 * row + j;
				i++;

				if (isBlack(g->gameBoard[row][j]))
					break;
				j--;

			}
		}
		if (piece == 'N') {
		}
		if (piece == 'K') {
		}
		if (piece == 'Q') {
		}
		if (piece == 'B') {
		}
	} else {
	}
}

bool isWhite(char c) {
	return (c >= 'A') && (c <= 'Z');
}
bool isBlack(char c) {
	return (c >= 'a') && (c <= 'z');
}
bool isEmpty(char c) {
	return c == '_';
}
SET_COMMAND game_settings() {
///////////////////// game default settings

	int difficulty = 2;
	int players = 1;
	int color = 1; // 1 is white

/////////////////////
	SetCommand sp;
	char * string = (char*) calloc(1024, sizeof(char));
	while (true) {
		printf(
				"Specify game setting or type 'start' to begin a game with the current setting:\n");
		fgets(string, 1024, stdin);
		sp = setting_parse(string);
		if (sp.cmd == SET_INVALID_LINE) {
		}
		if (sp.cmd == SET_START) {
			return sp.cmd;
		}
		if (sp.cmd == SET_QUIT) {
			return sp.cmd;
		}
		if (sp.cmd == SET_PRINT_SETTING) {
			if (players == 1) {
				printf("SETTINGS:\nGAME_MODE: 1\n");
			} else {
				printf(
						"SETTINGS:\nGAME_MODE: 2\nDIFFICULTY_LVL: %d\nUSER_CLR: %d\n",
						difficulty, color);
			}
		}
	}
	return sp.cmd;
}

SetCommand setting_parse(const char* str) {
	char argStr[30];
	char cmdStr[1024];
	for (int a = 0; a < 30; a++)
		argStr[a] = 0;
	for (int a = 0; a < 1024; a++)
		cmdStr[a] = 0;
	int arg;
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

	int index = getNextWord(str, cmdStr);
	sp.validArg = false;
	index = getNextWord(&str[index], argStr);
	if (equalStrings(cmdStr, s1)) {
		cmd = SET_GAME_MODE;
	} else if (equalStrings(cmdStr, s2)) {
		cmd = SET_DIFFICULTY;
	} else if (equalStrings(cmdStr, s3)) {
		cmd = SET_USER_COLOR;
	} else if (equalStrings(cmdStr, s4)) {
		cmd = SET_LOAD;
	} else if (equalStrings(cmdStr, s5)) {
		cmd = SET_DEFAULT;
	} else if (equalStrings(cmdStr, s6)) {
		cmd = SET_PRINT_SETTING;
	} else if (equalStrings(cmdStr, s7)) {
		cmd = SET_QUIT;
	} else if (equalStrings(cmdStr, s8)) {
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
	int arg1;
	int arg2;
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

/**
 gets the next word
 @param s- the string from which the next word is taken
 @param word- an empty string in which the next word is written to
 @return
 the last index of s, from which the word was taken
 */
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

/**
 checks if 2 strings are equal
 @param s1- first string to check
 @param s2- second string to check
 @return
 false if by some character the strings differ
 true if they are exactly the same
 */
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
	if (g->currentPlayer == 'B') {
		for (i = 1; i <= 8; i++) {
			printf("\n%d| ", 9 - i);
			for (j = 0; j < 8; j++)
				printf("%c ", g->gameBoard[i - 1][j]);
			printf("|");
		}

	} else {
		for (i = 1; i <= 8; i++) {
			printf("\n%d| ", 9 - i);
			for (j = 0; j < 8; j++)
				printf("%c ", g->gameBoard[8 - i][j]);
			printf("|");
		}
	}

	printf("\n  -----------------\n");
	printf("   A B C D E F G H\n");
}
