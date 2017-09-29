#ifndef CHESSMAINWIN_H_
#define CHESSMAINWIN_H_
#include <SDL.h>

/*TODO not entirely sure i need all of those different events*/
typedef enum {
	MAIN_EXIT,
	MAIN_NEWGAME,/*IF THIS IS PRESSED, changes screen accordingly*/ 
	MAIN_LOAD,/*IF THIS IS PRESSED, MOVES ON TO LOAD WIN*/

	/*NUMPLAYER EVENTS*/
	MAIN_NUMPLAYER_BACK,
	MAIN_NUMPLAYER_START,/*IF 2 PLAYER WAS PRESSED, THIS IS SHOWN, IF THIS IS PRESSED, MOVES ON TO GAME WIN*/
	MAIN_NUMPLAYER_NEXT,/*IF ONE PLAYER WAS  PRESSED THIS IS SHOWN INSTEAD OF START, IF THIS IS PRESSED, MOVES TO DIFFICULTY WIN*/
	MAIN_NUMPLAYER_ONE,
	MAIN_NUMPLAYER_TWO,

	/*DIFF EVENTS*/
	MAIN_DIFFICULTY_BACK,
	MAIN_DIFFICULTY_NEXT, /*IF THIS WAS CHOSEN, MOVES ON TO COLOR WIN*/
	MAIN_DIFFICULTY_NOOB,
	MAIN_DIFFICULTY_EASY,
	MAIN_DIFFICULTY_MODERATE,
	MAIN_DIFFICULTY_HARD,

	/*COLOR EVENTS*/
	MAIN_COLOR_BACK,
	MAIN_COLOR_START,
	MAIN_COLOR_BLACK,
	MAIN_COLOR_WHITE,
	
	/*OTHER POSSIBLE EVENTS*/
	MAIN_INVALID_ARGUMENT,
	MAIN_NONE
} MAIN_EVENT;

typedef struct {
	//SDL_Window* window;
	SDL_Window* mainWindow;
	/*contains a rendering state*/
	SDL_Renderer* mainRenderer;
	/*contains an efficient, driver-specific representation of pixel data.*/
	SDL_Texture* exitTexture;
	SDL_Texture* newGameTexture;
	SDL_Texture* loadTexture;
	SDL_Texture* startBGTexture;
	int newGameFromMainMenuPressed;
	int mainMenu;
	int loadFromMainMenuPressed;

	/*next "window" is where u choose num players*/
	SDL_Texture* onePlayerTexture;
	SDL_Texture* twoPlayerTexture;
	SDL_Texture* backFromNumPlayersTexture;/*can always be pressed*/
	/*according to the user's choice, start or next appears*/
	SDL_Texture* startFromNumPlayersTexture;
	SDL_Texture* nextToDiffWinTexture;
	int numPlayersPressed;/*if this is 1*/
	int onePlayerPressed;/*and this is 0, then start is supposed to show*/
	int nextToDiffWinPressed;/*otherwise next is supposed to show*/
	int backFromNumPlayersPressed;/*if this is pressed, returns to main menu*/

	/*if next was pressed, we go on to diff "window"*/
	SDL_Texture* backFromDiffTexture;/*can always be pressed*/
	SDL_Texture* noobTexture;
	SDL_Texture* easyTexture;
	SDL_Texture* moderateTexture;
	SDL_Texture* hardTexture;
	/*can only be pressed after a difficulty was chosen*/
	SDL_Texture* nextToColorWinTexture;
	int diffPressed;/*if this is pressed, shows next button*/
	int nextToColorWinPressed;/*if this and diff are pressed, moves on to color win*/
	int backFromDiffPressed;/*if this is pressed, returns to num players menu*/


	/*if nextToColorWin was pressed, we go on to color "window" */
	SDL_Texture* backFromColorTexture; /*can always be pressed*/
	SDL_Texture* blackTexture;
	SDL_Texture* whiteTexture;
	/*can only be pressed after a difficulty was chosen*/
	SDL_Texture* startFromColorTexture;
	int colorPressed;/*if this is pressed, shows start button*/
	int startFromColorWinPressed;/*if this and color are pressed, moves on to game window*/
	int backFromColorPressed;/*if this is pressed, returns to difficulty menu*/





} MainWin;

/*CLICKS*/

int isClickOnBack(int x, int y);
int isClickOnStart(int x, int y);
int isClickOnBlack(int x, int y);
int isClickOnWhite(int x, int y);
/*returns the difficulty level that was pressed*/
int isClickOnDiff(int x, int y);
int isClickOnNewGame(int x, int y);
int isClickOnLoad(int x, int y);
int isClickOnExit(int x, int y);

/*GUI HANDLERS*/
void MainWinNullifyIntSet(MainWin* src);
MainWin* MainWindowCreate();
void MainWindowDraw(MainWin* src);
void MainWindowDestroy(MainWin* src);
void MainWindowHide(MainWin* src);
void MainWindowShow(MainWin* src);
MAIN_EVENT MainWindowHandleEvent(MainWin* src, SDL_Event* event);

#endif
