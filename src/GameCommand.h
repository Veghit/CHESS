#ifndef GameCommand_H_
#define GameCommand_H_
#include <stdbool.h>


typedef enum {
	GAME_MOVE,
	GAME_GET_MOVES,
	GAME_SAVE,
	GAME_UNDO,
	GAME_RESET,
	GAME_QUIT,
	GAME_INVALID_LINE,
	GAME_CASTLE
} GAME_COMMAND;

typedef struct game_command_t {
	GAME_COMMAND cmd;
	bool validArg; //is set to true if the line contains a valid argument
	int arg1;
	int arg2;
} GameCommand;

GameCommand game_parse(const char* str);

bool equalStrings(const char * s1, const char * s2);

#endif //!GameCommand_H_
