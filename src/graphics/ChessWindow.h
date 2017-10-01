
#ifndef CHESSWINDOW_H_
#define CHESSWINDOW_H_
#include <SDL.h>

#include <SDL_video.h>
/*
typedef enum {
//Add messages here
}EVENT_MSG;
*/
typedef struct chess_window_t ChessWin;

struct chess_window_t {
	SDL_Window* window;
	SDL_Renderer* renderer;
	void(*destroyWindowFunc)(ChessWin*);
	void(*drawFunc)(ChessWin*);
	//SP_EVENT_MSG (*handleEventFunc)(SPTicTacToe*, SDL_Event*);
};

#endif
