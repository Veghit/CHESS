#ifndef CHESSGAMEWIN_H_
#define CHESSGAMEWIN_H_
//TODO
//1. include sdl correctly- hopefully, i managed, but i included 2.0.6, not 2.0.5
//2. create pictures for everything
//3. go through original graphical code and look up every meaning of every func
//4. go through all the todo's in the project, and fix them all
#include <SDL.h>
#include <SDL_video.h>
#include "ChessWindow.h"
#include "../SetCommand.h"// TODO not entirely sure what to put instead

typedef enum {
	//TODO need to change winning sits to white and black accordingly.
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
	Game* game;// TODO changed from originally (before the *) SPTicTacToe
}GameWin;

/*game window*/
GameWin* GameWindowCreate();
void GameWindowDraw(GameWin*, int draggedOriginsCurDest[], char movingPiece[],Game * g);
void GameWindowDestroy(GameWin*);
GAME_EVENT GameWindowHandleEvent(GameWin* src, SDL_Event* event, int draggedOriginsCurDest[], char piece[],Game * g);
/*clicks*/
int isClickOnRestart(int x, int y);
int isClickOnSave(int x, int y);
int isClickOnLoadFromGame(int x, int y);
int isClickOnUndo(int x, int y);
int isClickOnMainMenu(int x, int y);
int isClickOnExitFromGame(int x, int y);
/*other helpful funcs*/
void createBlackCouple(GameWin* res, SDL_Surface* loadingSurface);
void createWhiteCouple(GameWin* res, SDL_Surface* loadingSurface);
void createBlackPieces(GameWin* res, SDL_Surface* loadingSurface);
void createWhitePieces(GameWin* res, SDL_Surface* loadingSurface);
void createGameButtonsTextures(GameWin* res, SDL_Surface* loadingSurface);
void createGameToMainMenuButtons(GameWin* res, SDL_Surface* loadingSurface);
void nullifyHelpingArrays(int arr[], int length, char arr2[], int arr2Len);
void backToMainMenu(Game* g);

#endif
