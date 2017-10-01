#ifndef CHESSGUIMANAGER_H_
#define CHESSGUIMANAGER_H_
#include <SDL.h>
#include "ChessGameWin.h"
#include "ChessMainWin.h"
#include "ChessLoadWin.h"

typedef enum {
	MAIN_WINDOW_ACTIVE, LOAD_WINDOW_ACTIVE, GAME_WINDOW_ACTIVE
} ACTIVE_WINDOW;

typedef enum {
	MANAGER_QUTT, MANAGER_NONE,
} MANAGER_EVENET;

typedef struct {
	GameWin* gameWin;
	MainWin* mainWin;
	LoadWin* loadWin;
	ACTIVE_WINDOW activeWin;
	int nextSave;
} GuiManager;

GuiManager* ManagerCreate();

void ManagerDestroy(GuiManager* src);

void ManagerDraw(GuiManager* src, Game * g);

MANAGER_EVENET ManagerHandleEvent(GuiManager* src, SDL_Event* event, Game* g);
MANAGER_EVENET handleManagerDueToMainEvent(GuiManager* src, MAIN_EVENT event,
		Game* g);
MANAGER_EVENET handleManagerDueToLoadEvent(GuiManager* src, LOAD_EVENT event,
		Game* g);
MANAGER_EVENET handleManagerDueToGameEvent(GuiManager* src, GAME_EVENT event,
		Game* g);

#endif
