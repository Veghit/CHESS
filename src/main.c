//#include <string.h>
//#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "SetCommand.h"

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
	char * moveStr = (char*) calloc(1024, sizeof(char));
	GAME_COMMAND gameCmd = GAME_MOVE;
	SET_COMMAND setCmd = (SET_COMMAND) NULL;
	Game * g = (Game*) calloc(1, sizeof(Game));
	resetGame(g);// SAPIR -added a DIFF initialization
	/**
	 finished initializing new game
	 starting to actually make the game run
	 */
	while (true) {
		// first of all receives/ checks the game/set command. if it is to quit, then quits.
		if ((gameCmd == GAME_QUIT) || (setCmd == SET_QUIT)) {
			printf("Exiting...\n");
			free(moveStr);
			deleteGame(g);
			return 0;
			// if a game is to begin, but not reset, we can theoretically play the game
		} else if ((setCmd == SET_START) && (gameCmd != GAME_RESET) && (gameCmd != GAME_UNDO)) {
			//think this should be after the lost check SAPIR
			// checks if the current player can make no move
			if (isGameTied(g)) {
				printBoard(g);
				printf("The game is tied\n");
				return 0;
			}
			int check = isGameChecked(g);
			if (check == 1) {
				if (currentLost(g)) {
					printBoard(g);
					printf("Checkmate! black player wins the game\n");
					getchar();
					return 0;
				} else {
					printf("Check: white King is threatened!\n");
				}
			}
			if (check == 0) {
				if (currentLost(g)) {
					printBoard(g);
					printf("Checkmate! white player wins the game\n");
					getchar();
					return 0;
				} else {
					printf("Check: black King is threatened!\n");
				}

			}
			gameCmd = twoPlayersGame(g, moveStr);
		} else if (gameCmd == GAME_RESET) {
			printf("Restarting...\n");
			resetGame(g);
			gameCmd = GAME_MOVE;
			setCmd = game_settings(g);
		} else if (gameCmd == GAME_UNDO) {
			if (g->saves > 0) {
				deleteGame(g);
				g = cloneGame(lastGames[0]);
				lastGames[0] = lastGames[1];
				lastGames[1] = lastGames[2];
				printf("%s", lastMoves[0]);
				printf("%s", lastMoves[1]);
				lastMoves[0] = lastMoves[2];
				lastMoves[1] = lastMoves[3];
				lastMoves[2] = lastMoves[4];
				lastMoves[3] = lastMoves[5];
				g->saves -= 1;
			} else {
				printf("Empty history, move cannot be undone\n");
			}
			gameCmd = GAME_MOVE;

		} else {
			setCmd = game_settings(g);
		}
	}
	getchar();
	return 0;
}
