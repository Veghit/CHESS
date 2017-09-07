#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "Game.h"




int main(int argc, char *argv[]) {

	char mode = 'c'; // console mode
	if ((argc == 2) && (argv[1][0] == 'g'))
		mode = 'g'; // gui mode
	if (mode == 'g') {
		printf("graphical mode unsupported yet.");
		return 0;
	}
	char * moveStr = (char*)calloc(1024, sizeof(char));
	GAME_COMMAND gameCmd = GAME_MOVE;
	SET_COMMAND setCmd = SET_INVALID_LINE;
	Game * g = (Game*)calloc(1, sizeof(Game));
	resetGame(g);
	while (true) {
		if ((gameCmd == GAME_QUIT) || (setCmd == SET_QUIT)) {
			printf("Exiting...\n");
			return 0;
		}
		else if ((setCmd == SET_START) && gameCmd != GAME_RESET) {
			if (setting.PLAYERS == 1) {
				printf("unsupported yet");
				return 0;
			}
			if (isGameTied(g)) {
				printf("The game is tied\n");
				return 0;
			}
			int check = isGameChecked(g);
			if (check == setting.WHITE) {
				if (currentLost(g)) {
					printf("BLACK WON!");
					return 0;
				}
				else {
					printf("Check: white King is threatend!\n");
				}
			}
			if (check == setting.BLACK) {
				if (currentLost(g)) {
					printf("white wins the game");
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

	return 0;
}