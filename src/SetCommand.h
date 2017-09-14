#ifndef SetCommand_H_
#define SetCommand_H_
#include <stdbool.h>
#include "DefaultSettings.h"
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
int getNextWord(const char * s, char * word);
#endif // !SetCommand_H_

