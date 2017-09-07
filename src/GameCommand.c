#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "GameCommand.h"


/**
@param str- receives a line from stdin
@ret

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
	}
	else if (equalStrings(w1, s2)) {
		cmd = GAME_GET_MOVES;
	}
	else if (equalStrings(w1, s3)) {
		cmd = GAME_SAVE;
	}
	else if (equalStrings(w1, s4)) {
		cmd = GAME_UNDO;
	}
	else if (equalStrings(w1, s5)) {
		cmd = GAME_RESET;
	}
	else if (equalStrings(w1, s6)) {
		cmd = GAME_QUIT;
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
