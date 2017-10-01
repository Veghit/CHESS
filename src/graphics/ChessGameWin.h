#ifndef CHESSGAMEWIN_H_
#define CHESSGAMEWIN_H_

#include <SDL.h>
#include <SDL_video.h>
#include "ChessWindow.h"
#include "../SetCommand.h"

typedef enum {
	GAME_EVENT_LOAD,
	GAME_EVENT_SAVE,
	GAME_EVENT_UNDO,
	GAME_EVENT_RESTART,
	GAME_EVENT_EXIT,
	GAME_EVENT_MAIN_MENU,
	GAME_EVENT_PIECE_CHOSEN,
	GAME_EVENT_MOVING,
	GAME_EVENT_WHITE_WON,
	GAME_EVENT_BLACK_WON,
	GAME_EVENT_TIE,
	GAME_EVENT_QUIT,
	GAME_EVENT_INVALID_ARGUMENT,
	GAME_EVENT_NONE
} GAME_EVENT;

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	
	SDL_Texture* bgTexture;
	
	/*additional textures*/
	SDL_Texture* restartTexture;
	SDL_Texture* saveTexture; 
	SDL_Texture* loadFromGameTexture; 
	SDL_Texture* undoTexture; 
	SDL_Texture* mainMenuTexture; 
	SDL_Texture* exitFromGameTexture; 

	//each piece appears twice to represnt color
	SDL_Texture* blackRookTexture;
	SDL_Texture* blackKnightTexture;
	SDL_Texture* blackKingTexture;
	SDL_Texture* blackQueenTexture;
	SDL_Texture* blackPawnTexture;
	SDL_Texture* blackBishopTexture;
	SDL_Texture* whiteRookTexture;
	SDL_Texture* whiteKnightTexture;
	SDL_Texture* whiteKingTexture;
	SDL_Texture* whiteQueenTexture;
	SDL_Texture* whitePawnTexture;
	SDL_Texture* whiteBishopTexture;
	Game* game;
}GameWin;

/*game window*/
/**
creates a new game window with all of the necessary fields
@ret- such a game window*/
GameWin* GameWindowCreate();
/**
@param src- the current game window
@param draggedOriginsCurDest[]- an array of length 7 which has
0, -1 or the origins, current and destination x and y of the dragged piece's coordinates
@param movingPiece[]- an array of length 1 that has the sign of the piece that is dragged,
in console mode
@param g- the current game that needs to be drawn in the gui mode
draws the game in gui mode while showing the location of the piece dragged
*/
void GameWindowDraw(GameWin* src, int draggedOriginsCurDest[], char movingPiece[],Game * g);
/**
@param src- the current game window
destroys all the fields of the window
and eventually the window itself
*/
void GameWindowDestroy(GameWin* src);
/**

@param src- the game window
@param event- sdl event that occured in the game window
@param g- the current game
updates the game settings in accordance to the buttons pressed by the user
@returns the event in accordance to the user's choice/ mouse actions*/

GAME_EVENT GameWindowHandleEvent(GameWin* src, SDL_Event* event, int draggedOriginsCurDest[], char piece[],Game * g);
/*clicks*/
/*THIS IS A DOC FOR ALL CLICKS*/
/**
/*@param x- the x coordinate of the mouse when the button is up
@param y-the x coordinate of the mouse when the button is up
returns 1 if the click was on the "button" coordinates
and 0 otherwise*/
int isClickOnRestart(int x, int y);
int isClickOnSave(int x, int y);
int isClickOnLoadFromGame(int x, int y);
int isClickOnUndo(int x, int y);
int isClickOnMainMenu(int x, int y);
int isClickOnExitFromGame(int x, int y);
/*other helpful funcs*/

/*THIS IS DOC TO ALL VOID CREATE*/
/**
@param res- the game window
@param loadingSurface- an empty/ clear sdl_surface*
updates fields of the game window in accordance to the name of the func
*/
void createBlackCouple(GameWin* res, SDL_Surface* loadingSurface);
void createWhiteCouple(GameWin* res, SDL_Surface* loadingSurface);
void createBlackPieces(GameWin* res, SDL_Surface* loadingSurface);
void createWhitePieces(GameWin* res, SDL_Surface* loadingSurface);
void createGameButtonsTextures(GameWin* res, SDL_Surface* loadingSurface);
void createGameToMainMenuButtons(GameWin* res, SDL_Surface* loadingSurface);
/**@param arr- an int arr
@param length- length of arr
@param arr2- another arr but of char type
@param arr2len- length of arr2
puts -1 or 0 or '0' accordingly, in each array*/
void nullifyHelpingArrays(int arr[], int length, char arr2[], int arr2Len);
/**@param g- the game
every field of g is reset, but the mode*/
void backToMainMenu(Game* g);

#endif
