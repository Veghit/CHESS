#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SetCommand.h"


/**
 @param str- receives a line
 @ret-

 */
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
	sp.arg1 = 0;
	sp.arg2 = 0;
	GAME_COMMAND cmd = GAME_INVALID_LINE;
	char *s1 = "move";
	char *s2 = "get_moves";
	char *s3 = "save";
	char *s4 = "undo";
	char *s5 = "reset";
	char *s6 = "quit";
	char *s7 = "castle";
	char *s1_2 = "to";
	int index = getNextWord(str, w1);
	sp.validArg = false;
	index += getNextWord(&str[index], w2);
	index += getNextWord(&str[index], w3);
	index += getNextWord(&str[index], w4);
	if (equalStrings(w1, s1) && equalStrings(w3, s1_2)) {
		if ((w2[0] == '<') && (w2[2] == ',') && (w2[4] == '>') && (w4[0] == '<')
				&& (w4[2] == ',') && (w4[4] == '>')) {
			int row1 = w2[1] - '1';
			int row2 = w4[1] - '1';
			int col1 = w2[3] - 'A';
			int col2 = w4[3] - 'A';
			sp.arg1 = 8 * row1 + col1;
			sp.arg2 = 8 * row2 + col2;
			if ((row1 >= 0) && (row1 < 8) && (col1 >= 0) && (col2 >= 0)
					&& (row2 >= 0) && (row2 < 8) && (col1 < 8) && (col2 < 8))
				sp.validArg = true;
		}
		cmd = GAME_MOVE;
	} else if (equalStrings(w1, s2)) {
		cmd = GAME_GET_MOVES;
		if ((w2[0] == '<') && (w2[2] == ',') && (w2[4] == '>')) {
			int row = w2[1] - '1';
			int col = w2[3] - 'A';
			sp.arg1 = 8 * row + col;
			if ((row >= 0) && (row < 8) && (col >= 0) && (col < 8))
				sp.validArg = true;
		}
	} else if (equalStrings(w1, s3)) {
		cmd = GAME_SAVE;
		sp.arg3 = (char *) calloc(100, sizeof(char));
		if (sp.arg3 == NULL) {
			printf("ERROR in memory allocation.");
			sp.cmd = GAME_INVALID_LINE;
			return sp;
		}
		strcpy(sp.arg3, w2);
		if (w2[0] != 0)
			sp.validArg = true;
	} else if (equalStrings(w1, s4)) {
		cmd = GAME_UNDO;
	} else if (equalStrings(w1, s5)) {
		cmd = GAME_RESET;
	} else if (equalStrings(w1, s6)) {
		cmd = GAME_QUIT;
	} else if (equalStrings(w1, s7)) {
		if ((w2[0] == '<') && (w2[2] == ',') && (w2[4] == '>')) {
			int row = w2[1] - '1';
			int col = w2[3] - 'A';
			sp.arg1 = 8 * row + col;
			if ((sp.arg1 == 0) || (sp.arg1 == 7) || (sp.arg1 == 56)
					|| (sp.arg1 == 63))
				sp.validArg = true;
		}
		cmd = GAME_CASTLE;
	}
	sp.cmd = cmd;
	return sp;
}

/** checks if 2 strings are equal
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
