#ifndef CHESSLOADWIN_H_
#define CHESSLOADWIN_H_
#include <SDL.h>

typedef enum {
	LOAD_BACK,
	LOAD_EXIT,
	LOAD_PRESSED,/*ONLY AFTER PRESSING THIS, A LOAD IS APPLIED*/
	LOAD_SLOT1,
	LOAD_SLOT2,
	LOAD_SLOT3,
	LOAD_SLOT4,
	LOAD_SLOT5,
	LOAD_INVALID_ARGUMENT,
	LOAD_NONE
} LOAD_EVENT;

typedef struct {
	/*GENERAL*/
	int loadFromMainMenu;
	int loadFromGame;/*if it's from game, and i press back, i need to return to the game*/
	SDL_Window* LoadWindow;
	SDL_Renderer* LoadRenderer;
	SDL_Texture* startBGTexture;
	/*REOCCURING BUTTONS.*/
	SDL_Texture* backTexture;
	int backPressed;
	SDL_Texture* loadTexture;
	/*local buttons, EACH BUTTON SHOULD BE SHOWN ONLY ACCORDING TO THE NUMBER OF SLOTS ACTUALLY SAVED*/
	SDL_Texture* slot1Texture;

	SDL_Texture* slot2Texture;

	SDL_Texture* slot3Texture;

	SDL_Texture* slot4Texture;

	SDL_Texture* slot5Texture;
	int numSlotsShown;
	int chosenSlot;
	
} LoadWin;
int isClickOnBackFromLoad(int x, int y);
int isClickOnLoadFromLoad(int x, int y);


LoadWin* loadWindowCreate();
void loadWindowDraw(LoadWin* src);
void loadWindowDestroy(LoadWin* src);
void loadWindowHide(LoadWin* src);
void loadWindowShow(LoadWin* src);
LOAD_EVENT loadWindowHandleEvent(LoadWin* src, SDL_Event* event);

#endif
