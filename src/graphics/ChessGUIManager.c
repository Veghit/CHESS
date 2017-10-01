#include "ChessGUIManager.h"
#include <stdio.h>
#include <stdlib.h>

char piece[1] = { '0' };
int draggedOriginsCurDest[7] = { 0, -1, -1, -1, -1, -1, -1 };

GuiManager* ManagerCreate() {
	GuiManager* res = (GuiManager*) malloc(sizeof(GuiManager));
	if (res == NULL) {
		return NULL;
	}
	res->mainWin = MainWindowCreate();
	if (res->mainWin == NULL) {
		free(res);
		return NULL;
	}
	res->activeWin = MAIN_WINDOW_ACTIVE;
	res->gameWin = NULL;
	res->nextSave = 1;
	return res;
}
void ManagerDestroy(GuiManager* src) {
	if (!src) {
		return;
	}
	if (src->activeWin == GAME_WINDOW_ACTIVE) {
		GameWindowDestroy(src->gameWin);
	}
	MainWindowDestroy(src->mainWin);
	free(src);
}

/*@param src- the 
 depending on which active window it is, it draws/ prints to screen
 the correct updated window, including game state, if in the game window*/
void ManagerDraw(GuiManager* src, Game* g) {
	if (!src) {
		return;
	}
	if (src->activeWin == MAIN_WINDOW_ACTIVE) {
		MainWindowDraw(src->mainWin);
	} else if (src->activeWin == LOAD_WINDOW_ACTIVE)
		loadWindowDraw(src->loadWin);
	else if (src->activeWin == GAME_WINDOW_ACTIVE) {
		GameWindowDraw(src->gameWin, draggedOriginsCurDest, piece, g);
	}
}
MANAGER_EVENET handleManagerDueToMainEvent(GuiManager* src, MAIN_EVENT event,
		Game * g) {
	if (src == NULL) {
		return MANAGER_NONE;
	}
	switch (event) {
	case MAIN_LOAD:
		MainWindowHide(src->mainWin);
		src->loadWin = loadWindowCreate();
		if (src->loadWin == NULL) {
			printf("couldn't create load window \n");
			return MANAGER_QUTT;
		}
		src->loadWin->loadFromMainMenu = 1;
		src->activeWin = LOAD_WINDOW_ACTIVE;
		break;

	case MAIN_NUMPLAYER_START:
	case MAIN_COLOR_START:
		MainWindowHide(src->mainWin);
		src->gameWin = GameWindowCreate();
		if (src->gameWin == NULL) {
			printf("Couldn't create game window\n");
			return MANAGER_QUTT;
		}

		//since the game window activated fine, the now active window in src is that
		src->activeWin = GAME_WINDOW_ACTIVE;
		break;
		//but now the received event should be checked
		//since even if the game started correctly, suppose the person wanted out
		//since the main window, and pressed on it, then out we go
	case MAIN_EXIT:
		return MANAGER_QUTT;
	default:
		break;
	}
	return MANAGER_NONE;

}

bool loadGameFromSlot(Game* loadedGame, int slotPressed, GuiManager* src) {
	int nextSave = src->nextSave;
	int chosenSave = nextSave - slotPressed; // the 1st slot gives the latest save. the 2nd slot gives the 2nd latest and so on.
	if (chosenSave<0)
		chosenSave+=5;
	//printf("chosen save is:%d",chosenSave);
	if (chosenSave == 1)
		return xml_load(loadedGame, "save1.xml");
	else if (chosenSave == 2)
		return xml_load(loadedGame, "save2.xml");
	else if (chosenSave == 3)
		return xml_load(loadedGame, "save3.xml");
	else if (chosenSave == 4)
		return xml_load(loadedGame, "save4.xml");
	else if (chosenSave == 0)
		return xml_load(loadedGame, "save5.xml");
	else
		return false;
}

MANAGER_EVENET handleManagerDueToLoadEvent(GuiManager* src, LOAD_EVENT event,
		Game * g) {
	if (src == NULL) {
		return MANAGER_NONE;
	}
	Game* loadedGame = (Game *) calloc(1, sizeof(Game));
	switch (event) {
	case LOAD_BACK:
		if (src->loadWin->loadFromMainMenu == 1) {
			src->loadWin->loadFromMainMenu = 0;
			loadWindowDestroy(src->loadWin);
			src->loadWin = NULL;
			src->activeWin = MAIN_WINDOW_ACTIVE;
			MainWinNullifyIntSet(src->mainWin);
			MainWindowShow(src->mainWin);
		} else if (src->loadWin->loadFromGame == 1) {
			src->loadWin->loadFromGame = 0;
			src->activeWin = GAME_WINDOW_ACTIVE;
			loadWindowDestroy(src->loadWin);
			src->loadWin = NULL;
		}
		break;

	case LOAD_PRESSED:
		loadWindowHide(src->loadWin);
		src->gameWin = GameWindowCreate();
		if (src->gameWin == NULL) {
			printf("Couldn't create game window\n");
			return MANAGER_QUTT;
		}
		src->activeWin = GAME_WINDOW_ACTIVE;
		break;
	case LOAD_EXIT:
		return MANAGER_QUTT;
	case LOAD_SLOT1:
		if (loadGameFromSlot(loadedGame, 1,src)) {
			deleteGame(g);
			g = cloneGame(loadedGame);
		} else {
			printf("ERROR: BAD FILE");
			return MANAGER_QUTT;
		}
		break;
	case LOAD_SLOT2:
		if (loadGameFromSlot(loadedGame, 2,src)) {
			deleteGame(g);
			g = cloneGame(loadedGame);
		} else {
			printf("ERROR: BAD FILE");
			return MANAGER_QUTT;
		}
		break;
	case LOAD_SLOT3:
		if (loadGameFromSlot(loadedGame, 3,src)) {
			deleteGame(g);
			g = cloneGame(loadedGame);
		} else {
			printf("ERROR: BAD FILE");
			return MANAGER_QUTT;
		}
		break;
	case LOAD_SLOT4:
		if (loadGameFromSlot(loadedGame, 4,src)) {
			deleteGame(g);
			g = cloneGame(loadedGame);
		} else {
			printf("ERROR: BAD FILE");
			return MANAGER_QUTT;
		}
		break;
	case LOAD_SLOT5:
		if (loadGameFromSlot(loadedGame, 5,src)) {
			deleteGame(g);
			g = cloneGame(loadedGame);
		} else {
			printf("ERROR: BAD FILE");
			return MANAGER_QUTT;
		}
		break;
	default:
		break;
	}
	deleteGame(loadedGame);
	return MANAGER_NONE;
}

void saveGameInSlot(Game * g, GuiManager * src) {
	int slot = src->nextSave;
	//printf("saving in slot:%d\n",slot);
	if (slot == 1)
		xml_save(g, "save1.xml");
	else if (slot == 2)
		xml_save(g, "save2.xml");
	else if (slot == 3)
		xml_save(g, "save3.xml");
	else if (slot == 4)
		xml_save(g, "save4.xml");
	else if (slot == 5)
		xml_save(g, "save5.xml");
	if (slot == 5)
		src->nextSave = 1;
	else
		src->nextSave += 1;
}

MANAGER_EVENET handleManagerDueToGameEvent(GuiManager* src, GAME_EVENT event,
		Game * g) {
	if ((src == NULL) || (event == GAME_EVENT_NONE))
		return MANAGER_NONE;

	if (event == GAME_EVENT_SAVE) {
		saveGameInSlot(g, src);
		return MANAGER_NONE;
	}
	if (event == GAME_EVENT_RESTART) {
		GameWindowDestroy(src->gameWin);
		src->gameWin = GameWindowCreate();
		return MANAGER_NONE;		  //TODO not entirely sure
	}
	if (event == GAME_EVENT_UNDO) {
		return MANAGER_NONE;		  //TODO not entirely sure
	}

	/*TODO until here, not including anything below*/
	if (event == GAME_EVENT_WHITE_WON) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game over", "WHITE won",
		NULL);
	} else if (event == GAME_EVENT_BLACK_WON) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game over", "BLACK won",
		NULL);
	} else if (event == GAME_EVENT_TIE) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game over",
				"it's a tie", NULL);
	}
	if (event == GAME_EVENT_PIECE_CHOSEN) {
		return MANAGER_NONE;
	}
	/*SO FROM HERE KINDA TAKES CARE OF MAIN MENU BUTTON AS WELL*/
	if (event == GAME_EVENT_LOAD) {
		src->loadWin = loadWindowCreate();
		if (src->loadWin == NULL) {
			printf("couldn't create load window \n");
			return MANAGER_QUTT;
		}
		src->loadWin->loadFromGame = 1;
		src->activeWin = LOAD_WINDOW_ACTIVE;
	} else {
		if (event == GAME_EVENT_EXIT || event == GAME_EVENT_MAIN_MENU
				|| event == GAME_EVENT_QUIT) {
			GameWindowDestroy(src->gameWin);
			src->gameWin = NULL;
			src->activeWin = MAIN_WINDOW_ACTIVE;
			MainWinNullifyIntSet(src->mainWin);
			MainWindowShow(src->mainWin);
		}
		return MANAGER_NONE;
	}
	return MANAGER_NONE;
}

MANAGER_EVENET ManagerHandleEvent(GuiManager* src, SDL_Event* event, Game* g) {
	//print_settings(g);

	if (src == NULL || event == NULL) {
		return MANAGER_NONE;
	}

	if (src->activeWin == MAIN_WINDOW_ACTIVE) {
		MAIN_EVENT mainEvent = MainWindowHandleEvent(src->mainWin, event, g);
		ManagerDraw(src, g);
		return handleManagerDueToMainEvent(src, mainEvent, g);
	}
	if (src->activeWin == LOAD_WINDOW_ACTIVE) {
		LOAD_EVENT loadEvent = loadWindowHandleEvent(src->loadWin, event);
		ManagerDraw(src, g);
		return handleManagerDueToLoadEvent(src, loadEvent, g);
	}
	if (src->activeWin == GAME_WINDOW_ACTIVE) {
		GAME_EVENT gameEvent = GameWindowHandleEvent(src->gameWin, event,
				draggedOriginsCurDest, piece, g);
		ManagerDraw(src, g);
		return handleManagerDueToGameEvent(src, gameEvent, g);
	}
	return MANAGER_NONE;
}

