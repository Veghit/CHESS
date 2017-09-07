#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "SetCommand.h"


SET_COMMAND game_settings() {

	SetCommand sp;
	char * string = (char*)calloc(1024, sizeof(char));
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
					*PLAYERS = 1;
					printf("Game mode is set to 1 player\n");

				}
				else {
					*PLAYERS = 2;
					printf("Game mode is set to 2 players\n");
				}
			}
			else {
				printf("Wrong game mode\n");
			}
		}
		if ((sp.cmd == SET_DIFFICULTY) && (*PLAYERS == 1)) {
			if (sp.validArg) {
				if (sp.arg == 5) {
					printf(
						"Expert level not supported, please choose a value between 1 to 4:\n");
				}
				else {
					*DIFF = sp.arg;
				}
			}
			else
				printf(
					"Wrong difficulty level. The value should be between 1 to 5\n");
		}
		if ((sp.cmd == SET_USER_COLOR) && (*PLAYERS == 1) && (sp.validArg)) {
			*USER_COLOR = sp.arg;
		}
		if (sp.cmd == SET_DEFAULT) {
			*USER_COLOR = 1;
			*DIFF = 2;
			*PLAYERS = 1;
		}
		if (sp.cmd == SET_QUIT) {
			return sp.cmd;
		}
		if (sp.cmd == SET_PRINT_SETTING) {
			if (*PLAYERS == 1) {
				printf("SETTINGS:\nGAME_MODE: 1\n");
			}
			else {
				printf(
					"SETTINGS:\nGAME_MODE: 2\nDIFFICULTY_LVL: %d\nUSER_CLR: %d\n",
					*DIFF, *USER_COLOR);
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
	}
	else if (equalStrings(w1, s2)) {
		if (equalStrings(w2, "1") || equalStrings(w2, "2")
			|| equalStrings(w2, "3") || equalStrings(w2, "4")
			|| equalStrings(w2, "5")) {
			sp.validArg = true;
			sp.arg = w2[0] - '0';
		}
		cmd = SET_DIFFICULTY;
	}
	else if (equalStrings(w1, s3)) {
		cmd = SET_USER_COLOR;
		if (equalStrings(w2, "1") || equalStrings(w2, "0")) {
			sp.validArg = true;
			sp.arg = w2[0] - '0';
		}
	}
	else if (equalStrings(w1, s4)) {
		cmd = SET_LOAD;
	}
	else if (equalStrings(w1, s5)) {
		cmd = SET_DEFAULT;
	}
	else if (equalStrings(w1, s6)) {
		cmd = SET_PRINT_SETTING;
	}
	else if (equalStrings(w1, s7)) {
		cmd = SET_QUIT;
	}
	else if (equalStrings(w1, s8)) {
		cmd = SET_START;
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
