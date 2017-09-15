#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "Game.h"



/**
@param argc-
@param argv-
@ret
plays a chess game in a gui or console mode, according to the user's wishes
*/
int main(int argc, char *argv[]) {

	char mode = 'c'; // console mode
	if ((argc == 2) && (argv[1][0] == 'g'))
		mode = 'g'; // gui mode
	if (mode == 'g') {
		printf("graphical mode unsupported yet.");
		return 0;
	}
	/**
	initializing a new game
	*/
	char * moveStr = (char*)calloc(1024, sizeof(char));
	GAME_COMMAND gameCmd = GAME_MOVE;
	SET_COMMAND setCmd = SET_INVALID_LINE;
	Game * g = (Game*)calloc(1, sizeof(Game));
	resetGame(g);
	/**
	finished initializing new game
	starting to actually make the game run
	*/
	while (true) {
		// first of all receives/ checks the game/set command. if it is to quit, then quits.
		//SAPIR no freeing the memory? is it unnecessary?
		if ((gameCmd == GAME_QUIT) || (setCmd == SET_QUIT)) {
			printf("Exiting...\n");
			return 0;
		}
		// if a game is to begin, but not reset, we can theoretically play the game
		else if ((setCmd == SET_START) && gameCmd != GAME_RESET) {
			//player vs pc is not yet supported, requires a different module
			if (getPLAYERS() == 1) {
				printf("unsupported yet");
				return 0;
			}
			// checks if the current player can make no move
			//	SAPIR i think first it should be a win check, not tied. because making no move could be the same as a win
			if (isGameTied(g)) {
				printf("The game is tied\n");
				return 0;
			}
			//SAPIR the winning messages are not as shown in the pdf
			int check = isGameChecked(g);
			if (check == getWHITE()) {
				if (currentLost(g)) {
					printBoard(g);
					printf("Checkmate! black player wins the game\n");
					getchar();
					return 0;
				}
				else {
					printf("Check: white King is threatend!\n");
				}
			}
			if (check == getBLACK()) {
				if (currentLost(g)) {
					printBoard(g);
					printf("Checkmate! white player wins the game\n");
					getchar();
					return 0;
				}
				else {
					printf("Check: black King is threatend!\n");
				}

			}
			gameCmd = twoPlayersGame(g, moveStr);
		}
		else if (gameCmd == GAME_RESET) {
			printf("Restarting...\n");
			resetGame(g);
			gameCmd = GAME_MOVE;
			setCmd = game_settings();
		}
		else {
			setCmd = game_settings();
		}
	}
	getchar();
	return 0;
}