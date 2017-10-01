#include <stdio.h>
#include <stdlib.h>
#include "SetCommand.h"
#include "graphics/ChessGUIManager.h"
#include "SDL.h"


void fixGui() {
	/*Returns 0 on success or a negative error code on failure;*/
	printf("entered fixGui\n");
	Game * g = (Game*) calloc(1, sizeof(Game));
	g->mode = 'g';
	resetGame(g);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL2 INIT
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		/*Use this function to retrieve a message about the last error that occurred.
		 an empty string if there hasn't been an error message set,
		 since the last call to this func */
		printf("entereed 	if (SDL_Init(SDL_INIT_VIDEO) < 0) \n ");
		return;
	}
	GuiManager* manager = ManagerCreate();
	if (manager == NULL) {
		/*clean up all initialized subsystems.
		 should call this function even if you have already shutdown each initialized subsystem,
		 with SDL_QuitSubSystem().
		 It is safe to call this function even in the case of errors in initialization.*/
		SDL_Quit();
		printf("entered if (manager == NULL)");
		return;
	}
	SDL_Event event;
	while (true) {
		/*Use this function to wait indefinitely for the next available event.
		 Returns 1 on success or 0 if there was an error while waiting for events*/
		SDL_WaitEvent(&event);
		if (ManagerHandleEvent(manager, &event, g) == MANAGER_QUTT) {
			break;
		}
		ManagerDraw(manager, g);
	}
	ManagerDestroy(manager);
	SDL_Quit();

}
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
		fixGui();
		return 0;
	}
	/**
	 initializing a new game
	 */
	char * moveStr = (char*) calloc(1024, sizeof(char));
	if (moveStr == NULL) {
		printf("ERROR in memory allocation.");
		return 0;
	}
	GAME_COMMAND gameCmd = GAME_MOVE;
	SET_COMMAND setCmd = (SET_COMMAND) NULL;
	Game * g = (Game*) calloc(1, sizeof(Game));
	if (g == NULL) {
		free(moveStr);
		return 0;
	}
	resetGame(g);
	/**
	 finished initializing new game
	 starting to actually make the game run
	 */
	while (true) {
		// first of all receives/ checks the game/set command. if it is to quit, then quits.
		if ((gameCmd == GAME_QUIT) || (setCmd == SET_QUIT)) {
			printf("Exiting...\n");
			return exit_game(g, moveStr);
			// if a game is to begin, but not reset, we can theoretically play the game
		} else if ((setCmd == SET_START) && (gameCmd != GAME_RESET)
				&& (gameCmd != GAME_UNDO) && (gameCmd != GAME_SAVE)) {
			// checks if the current player can make no move
			int check = isGameChecked(g);
			if (check!=g->currentPlayer && isGameTied(g)) {
				//printBoard(g);
				if ((g->PLAYERS == 1) && (g->currentPlayer != g->USER_COLOR))
					printf("The game ends in a tie\n");
				else
					printf("The game is tied\n");
				return exit_game(g, moveStr);
			}
			if (gameCmd!=GAME_INVALID_LINE && check == 1) {
				if (currentLost(g)) {
					printf("Checkmate! black player wins the game\n");
					return exit_game(g, moveStr);
				} else {
					if ((g->PLAYERS == 1) && (g->USER_COLOR == 1))
						printf("Check!\n");
					else
						if(g->PLAYERS==2 || g->USER_COLOR == g->currentPlayer)
							printf("Check: white King is threatened!\n");
				}
			}
			if (gameCmd!=GAME_INVALID_LINE && check == 0) {
				if (currentLost(g)) {
					printf("Checkmate! white player wins the game\n");
					return exit_game(g, moveStr);
				} else {
					if ((g->PLAYERS == 1) && (g->USER_COLOR == 0))
						printf("Check!\n");
					else
						if(g->PLAYERS==2 || g->USER_COLOR == g->currentPlayer)
							printf("Check: black King is threatened!\n");
				}

			}
			if ((gameCmd == GAME_MOVE)
					&& ((g->PLAYERS == 2) || (g->currentPlayer == g->USER_COLOR)))
				printBoard(g);
			gameCmd = twoPlayersGame(g, moveStr);
			if (gameCmd == GAME_INVALID_LINE) {
				if (g->PLAYERS == 1 && g->currentPlayer != g->USER_COLOR) //pc move{
						{
					printf("ERROR BAD PC MOVE.");
					gameCmd = GAME_QUIT;
				}
			}

		} else if (gameCmd == GAME_RESET) {
			printf("Restarting...\n");
			resetGame(g);
			gameCmd = GAME_MOVE;
			setCmd = game_settings(g);
		} else if (gameCmd == GAME_UNDO) {
			g = game_undo(g);
			gameCmd = GAME_MOVE;
		} else if (gameCmd == GAME_SAVE) {
			gameCmd = GAME_INVALID_LINE;
		} else {
			setCmd = game_settings(g);
		}
	}
	return exit_game(g, moveStr);
}

Game * game_undo(Game * g) {
	if (g->PLAYERS == 1) {
		if (g->saves > 0) {
			deleteGame(g);
			g = lastGames[0];
			printf("%s", lastMoves[0]);
			printf("%s", lastMoves[1]);
			free(lastMoves[0]);
			free(lastMoves[1]);
			if (g->saves > 1) {
				lastGames[0] = lastGames[1];
				lastMoves[0] = lastMoves[2];
				lastMoves[1] = lastMoves[3];
				if (g->saves > 2) {
					lastGames[1] = lastGames[2];
					lastMoves[2] = lastMoves[4];
					lastMoves[3] = lastMoves[5];
				}
			}
			g->saves -= 1;
		} else {
			printf("Empty history, move cannot be undone\n");
		}
	} else {
		printf("Undo command not avaialbe in 2 players mode\n");
	}
	return g;
}

int exit_game(Game* g, char* moveStr) {
	free(moveStr);
	int i = 0;
	for (i = 0; i < g->saves; i++) {
		deleteGame(lastGames[i]);
		free(lastMoves[2 * i]);
		free(lastMoves[2 * i + 1]);
	}
	deleteGame(g);
	return 0;
}
