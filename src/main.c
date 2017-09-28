//#include <string.h>
//#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "graphics\ChessGUIManager.h"
#include "SDL.h"




void fixGui() {
	/*Returns 0 on success or a negative error code on failure;*/
	printf("entered fixGui\n");
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { //SDL2 INIT
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		/*Use this function to retrieve a message about the last error that occurred.
		an empty string if there hasn't been an error message set,
		since the last call to this func */
		printf("entereed 	if (SDL_Init(SDL_INIT_VIDEO) < 0) \n ");
		return ;
	}
	GuiManager* manager = ManagerCreate();
	if (manager == NULL) {
		/*clean up all initialized subsystems. 
		should call this function even if you have already shutdown each initialized subsystem,
		with SDL_QuitSubSystem(). 
		It is safe to call this function even in the case of errors in initialization.*/
		SDL_Quit();
		printf("entered if (manager == NULL)");
		return ;
	}
	SDL_Event event;
	while (1) {
		/*Use this function to wait indefinitely for the next available event.
		Returns 1 on success or 0 if there was an error while waiting for events*/
		SDL_WaitEvent(&event);
		if (ManagerHandleEvent(manager, &event) == MANAGER_QUTT) {
			break;
		}
		//TODO sapir, how can i receive an event before i draw?
		ManagerDraw(manager);
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
		printf("graphical mode NOT FULLY SUPPORTED yet.");
		fixGui();
		
	}
	/**
	 initializing a new game
	 */
	if (mode == 'c') {
		char * moveStr = (char*)calloc(1024, sizeof(char));
		GAME_COMMAND gameCmd = GAME_MOVE;
		SET_COMMAND setCmd = (SET_COMMAND)NULL;
		Game * g = (Game*)calloc(1, sizeof(Game));
		resetGame(g, mode);
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
			}
			else if ((setCmd == SET_START) && (gameCmd != GAME_RESET)
				&& (gameCmd != GAME_UNDO)) {
				// checks if the current player can make no move
				if (isGameTied(g)) {
					printBoard(g);
					printf("The game is tied\n");
					return exit_game(g, moveStr);
				}
				int check = isGameChecked(g);
				if (check == 1) {
					if (currentLost(g)) {
						//printBoard(g);
						printf("Checkmate! black player wins the game\n");
						return exit_game(g, moveStr);
					}
					else {
						printf("Check: white King is threatened!\n");
					}
				}
				if (check == 0) {
					if (currentLost(g)) {
						//printBoard(g);
						printf("Checkmate! white player wins the game\n");
						return exit_game(g, moveStr);
					}
					else {
						printf("Check: black King is threatened!\n");
					}

				}
				if (gameCmd == GAME_MOVE)
					printBoard(g);
				gameCmd = twoPlayersGame(g, moveStr);
			}
			else if (gameCmd == GAME_RESET) {
				printf("Restarting...\n");
				resetGame(g, mode);
				gameCmd = GAME_MOVE;
				setCmd = game_settings(g);
			}
			else if (gameCmd == GAME_UNDO) {
				if (g->saves > 0) {
					deleteGame(g);
					g = lastGames[0];
					lastGames[0] = lastGames[1];
					lastGames[1] = lastGames[2];
					printf("%s", lastMoves[0]);
					printf("%s", lastMoves[1]);
					free(lastMoves[0]);
					free(lastMoves[1]);
					lastMoves[0] = lastMoves[2];
					lastMoves[1] = lastMoves[3];
					lastMoves[2] = lastMoves[4];
					lastMoves[3] = lastMoves[5];
					g->saves -= 1;
				}
				else {
					printf("Empty history, move cannot be undone\n");
				}
				gameCmd = GAME_MOVE;

			}
			else {
				setCmd = game_settings(g);
			}
		}
		return exit_game(g, moveStr);
	}
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

