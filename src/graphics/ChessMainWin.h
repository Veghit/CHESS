#ifndef CHESSMAINWIN_H_
#define CHESSMAINWIN_H_
#include <SDL.h>
#include "../SetCommand.h"

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
	SDL_Texture* onePlayerChosen;
	SDL_Texture* twoPlayerTexture;
	SDL_Texture* twoPlayerChosen;
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
	SDL_Texture* noobChosen;
	SDL_Texture* easyTexture;
	SDL_Texture* easyChosen;
	SDL_Texture* moderateTexture;
	SDL_Texture* moderateChosen;
	SDL_Texture* hardTexture;
	SDL_Texture* hardChosen;
	/*can only be pressed after a difficulty was chosen*/
	SDL_Texture* nextToColorWinTexture;
	int diffPressed;/*if this is pressed, shows next button*/
	int nextToColorWinPressed;/*if this and diff are pressed, moves on to color win*/
	int backFromDiffPressed;/*if this is pressed, returns to num players menu*/


	/*if nextToColorWin was pressed, we go on to color "window" */
	SDL_Texture* backFromColorTexture; /*can always be pressed*/
	SDL_Texture* blackTexture;
	SDL_Texture* blackChosen;
	SDL_Texture* whiteTexture;
	SDL_Texture* whiteChosen;
	/*can only be pressed after a difficulty was chosen*/
	SDL_Texture* startFromColorTexture;
	int colorPressed;/*if this is pressed, shows start button*/
	int startFromColorWinPressed;/*if this and color are pressed, moves on to game window*/
	int backFromColorPressed;/*if this is pressed, returns to difficulty menu*/





} MainWin;

/*CLICKS*/
/*THIS IS A DOC FOR ALL CLICKS*/
/**
/*@param x- the x coordinate of the mouse when the button is up
@param y-the x coordinate of the mouse when the button is up
returns 1 if the click was on the "button" coordinates
and 0 otherwise*/

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
/**
@param src- the mainWin as is
puts 0 in all of the int fields of src*/
void MainWinNullifyIntSet(MainWin* src);
/*THIS IS A DOC FOR ALL VOID CREATES*/
/**@param loadingSurface- an empty/clear surface
@param res- mainwindow
updates different fields in mainwindow, so could be used for later
according to the name of the func*/
void createMainMenuButtons(SDL_Surface* loadingSurface, MainWin* res);
void createGeneralButtons(SDL_Surface* loadingSurface, MainWin* res);
void createNumPlayersButtons(SDL_Surface* loadingSurface, MainWin* res);
void createDifficultyButtons(SDL_Surface* loadingSurface, MainWin* res);
void createLast2DiffButtons(SDL_Surface* loadingSurface, MainWin* res);
void createColorButtons(SDL_Surface* loadingSurface, MainWin* res);
/**
creates a main window with all the settings and it's fields
@ret- the main window created
*/
MainWin* MainWindowCreate();

void MainWindowDraw(MainWin* src);
/*@param src- the mainwin object
destroys any of the fields that were allocated memory*/
void MainWindowDestroy(MainWin* src);
/*@param src- the main window
hides it if shown*/
void MainWindowHide(MainWin* src);
/**
@param src- the main window
shows it if hidden*/
void MainWindowShow(MainWin* src);
/**
@param src- the main window
@param event- sdl event that occured in the main window
@param g- the current game, if there's one, for main to "refresh"
updates the game settings in accordance to the buttons pressed by the user
@returns the event in accordance to the user's choice*/
MAIN_EVENT MainWindowHandleEvent(MainWin* src, SDL_Event* event,Game* g);

#endif
