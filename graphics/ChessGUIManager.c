#include "ChessGUIManager.h"
#include <stdio.h>
#include <stdlib.h>


char piece[1] = { '0' };
int draggedOriginsCurDest[7] = { 0,-1,-1, -1, -1, -1, -1 };

GuiManager*  ManagerCreate() {
	GuiManager* res = (GuiManager*)malloc(sizeof( GuiManager));
	if (res == NULL) {
		return NULL;
	}
	res->mainWin =  MainWindowCreate();
	if (res->mainWin == NULL) {
		free(res);
		return NULL;
	}
	res->activeWin = MAIN_WINDOW_ACTIVE;
	res->gameWin = NULL;
	
	return res;
}

MANAGER_EVENET  ManagerHandleEvent(GuiManager* src, SDL_Event* event) {
	if (src == NULL || event == NULL) {
		return  MANAGER_NONE;
	}
	if (src->activeWin == MAIN_WINDOW_ACTIVE) {
		MAIN_EVENT mainEvent = MainWindowHandleEvent(src->mainWin, event);
		ManagerDraw(src);
		return handleManagerDueToMainEvent(src, mainEvent);
	}
	if (src->activeWin == LOAD_WINDOW_ACTIVE) {
		LOAD_EVENT loadEvent = loadWindowHandleEvent(src->loadWin, event);
		ManagerDraw(src);
		return handleManagerDueToMainEvent(src, loadEvent);
	}
	if (src->activeWin == GAME_WINDOW_ACTIVE) {
		GAME_EVENT gameEvent = GameWindowHandleEvent(src->gameWin, event, draggedOriginsCurDest, piece);
		//printf("managed the game handle in manager\n");
		ManagerDraw(src);
		//printf("managed the game draw in manager\n");
		return handleManagerDueToGameEvent(src, gameEvent);
	}
	return  MANAGER_NONE;
}
  MANAGER_EVENET handleManagerDueToMainEvent( GuiManager* src, MAIN_EVENT event) {
	if (src == NULL) {
		return   MANAGER_NONE;
	}
	switch(event){
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
			return   MANAGER_QUTT;
		}

		//since the game window activated fine, the now active window in src is that
		src->activeWin=GAME_WINDOW_ACTIVE;
		printf("changed the active win to game\n");
		break;
	//but now the received event should be checked
	//since even if the game started correctly, suppose the person wanted out
	//since the main window, and pressed on it, then out we go
	case MAIN_EXIT:

		return   MANAGER_QUTT;
	}
	return   MANAGER_NONE;
}

 MANAGER_EVENET handleManagerDueToLoadEvent(GuiManager* src, LOAD_EVENT event) {
	  if (src == NULL) {
		  return   MANAGER_NONE;
	  }
	  switch (event) {
	  case LOAD_BACK:
		  loadWindowHide(src->loadWin);
		  if (src->loadWin->loadFromMainMenu) {
			  src->loadWin->loadFromMainMenu = 0;
			  src->activeWin = MAIN_WINDOW_ACTIVE;
		  }
		  if (src->loadWin->loadFromGame) {
			  src->loadWin->loadFromGame = 0;
			  src->activeWin = GAME_WINDOW_ACTIVE;
		  }
		  break;

	  case LOAD_PRESSED:
	  
		  loadWindowHide(src->loadWin);//TODO not sure if it should really be create game, 
		  //and if so, needs to update it (the game and the game shown)with the saved game
		  src->gameWin = GameWindowCreate();
		  if (src->gameWin == NULL) {
			  printf("Couldn't create game window\n");
			  return   MANAGER_QUTT;
		  }
		  //since the game window activated fine, the now active window in src is that
		  src->activeWin = GAME_WINDOW_ACTIVE;
		  break;
		  //but now the received event should be checked
		  //since even if the game started correctly, suppose the person wanted out
		  //since the main window, and pressed on it, then out we go
	  case LOAD_EXIT:
		  return   MANAGER_QUTT;
	  }
	  return   MANAGER_NONE;
  }

  MANAGER_EVENET handleManagerDueToGameEvent( GuiManager* src, GAME_EVENT event) {
	if (event ==   GAME_EVENT_NONE || src == NULL) {
		return   MANAGER_NONE;
	}
	
	if (event == GAME_EVENT_LOAD) {
		src->loadWin = loadWindowCreate();
		if (src->loadWin == NULL) {
			printf("couldn't create load window \n");
			return MANAGER_QUTT;
		}
		src->loadWin->loadFromGame = 1;
		src->activeWin = LOAD_WINDOW_ACTIVE;
	}
	/*TODO from here, change and update accordingly.*/
	if (event == GAME_EVENT_SAVE) {
		printf("save in gui game unsupported yet\n");
		return MANAGER_NONE;
	}
	if (event == GAME_EVENT_RESTART) {
		GameWindowDestroy(src->gameWin);
		src->gameWin = GameWindowCreate();
		return MANAGER_NONE;//TODO not entirely sure
	}
	if (event == GAME_EVENT_UNDO) {
		printf("not yet supporting undo in gui\n");
		return MANAGER_NONE;//TODO not entirely sure
	}

	/*TODO until here, not including anything below*/
	if (event ==   GAME_EVENT_WHITE_WON) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game over", "WHITE won",
			NULL);
	}
	else if (event ==   GAME_EVENT_BLACK_WON) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game over", "BLACK won",
			NULL);
	}
	else if (event ==   GAME_EVENT_TIE) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game over", "it's a tie", NULL);
	}
	/*SO FROM HERE KINDA TAKES CARE OF MAIN MENU BUTTON AS WELL*/
	 GameWindowDestroy(src->gameWin);
	src->gameWin = NULL;
	src->activeWin =  MAIN_WINDOW_ACTIVE;
	/*TODO make sure im getting to the actual beginning of main menu, and if i need all this 4 lines, and if i should make everything 0*/
	MainWinNullifyIntSet(src->mainWin);
	MainWindowShow(src->mainWin);
	return  MANAGER_NONE;
}


  /*@param src- the
  depending on which active window it is, it draws/ prints to screen
  the correct updated window, including game state, if in the game window*/
  void  ManagerDraw(GuiManager* src) {
	  if (!src) {
		  return;
	  }
	  if (src->activeWin == MAIN_WINDOW_ACTIVE) {
		  MainWindowDraw(src->mainWin);
	  }
	  if (src->activeWin == LOAD_WINDOW_ACTIVE)
		  loadWindowDraw(src->loadWin);
	  if (src->activeWin == GAME_WINDOW_ACTIVE) {
		  //printf("entered game window draw in manager draw\n");
		  GameWindowDraw(src->gameWin, draggedOriginsCurDest, piece);
	  }
  }
 void  ManagerDestroy(GuiManager* src) {
	  if (!src) {
		  return;
	  }
	  if (src->activeWin == LOAD_WINDOW_ACTIVE) {
		  if(src->loadWin->loadFromGame)
			  GameWindowDestroy(src->gameWin);
		  loadWindowDestroy(src->loadWin);

	  }
	  if (src->activeWin == GAME_WINDOW_ACTIVE) {
		  GameWindowDestroy(src->gameWin);
	  }
	  MainWindowDestroy(src->mainWin);
	  free(src);
  }
