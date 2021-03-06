﻿#include "ChessGameWin.h"
#include <assert.h>
#include <stdio.h>

int isClickOnRestart(int x, int y) {
	if ((x >= 600 && x <= 700) && (y >= 10 && y <= 80))
		return 1;
	return 0;
}
int isClickOnSave(int x, int y) {
	if ((x >= 600 && x <= 700) && (y >= 112 && y <= 182))
		return 1;
	return 0;
}
int isClickOnLoadFromGame(int x, int y) {
	if ((x >= 600 && x <= 700) && (y >= 214 && y <= 284))
		return 1;
	return 0;
}
int isClickOnUndo(int x, int y) {
	if ((x >= 600 && x <= 700) && (y >= 316 && y <= 386))
		return 1;
	return 0;
}
int isClickOnMainMenu(int x, int y) {
	if ((x >= 600 && x <= 700) && (y >= 418 && y <= 488))
		return 1;
	return 0;
}
int isClickOnExitFromGame(int x, int y) {
	if ((x >= 600 && x <= 700) && (y >= 520 && y <= 590))
		return 1;
	return 0;
}
//TODO this is the toughest one to change into chess game
 GameWin*  GameWindowCreate() {
	 //printf("in game window create\n");
	 GameWin* res = ( GameWin*)calloc(sizeof( GameWin), sizeof(char));
	SDL_Surface* loadingSurface = NULL; //Used as temp surface
	if (res == NULL) {
		printf("Couldn't create  GameWin struct\n");
		return NULL;
	}
	/* TODO in the original, res->game was all about creating a new game, hence*/
	Game* g = (Game*)malloc(sizeof(Game));
	resetGame(g, 'g');
	res->game = g;
	/* the two lines above*/
	if (res->game == NULL) {
		printf("Couldn't create game\n");
		 GameWindowDestroy(res);
		return NULL;
	}
	// Create an application window with the following settings:
	res->window = SDL_CreateWindow("Chess", // window title
		SDL_WINDOWPOS_CENTERED,           // initial x position
		SDL_WINDOWPOS_CENTERED,           // initial y position
		//TODO this 2 originally were 600
		900,                               // width, in pixels
		608,                               // height, in pixels
		SDL_WINDOW_OPENGL                  // flags - see below
	);

	// Check that the window was successfully created
	if (res->window == NULL) {
		// In the case that the window could not be made...
		printf("Could not create a game window: %s\n", SDL_GetError());
		 GameWindowDestroy(res);
		return NULL;
	}
	res->renderer = SDL_CreateRenderer(res->window, -1, SDL_RENDERER_ACCELERATED);
	if (res->renderer == NULL) {
		printf("Could not create a game renderer: %s\n", SDL_GetError());
		 GameWindowDestroy(res);
		return NULL;
	}

	//Create a background texture:
	loadingSurface = SDL_LoadBMP("C:/Users/SAPIRA/Documents/GitHub/CHESS/src/graphics/images/board.bmp");
	if (loadingSurface == NULL) {
		printf("Could not create a game board.bmp surface: %s\n", SDL_GetError());
		 GameWindowDestroy(res);
		return NULL;
	}
	res->bgTexture = SDL_CreateTextureFromSurface(res->renderer, loadingSurface);
	if (res->bgTexture == NULL) {
		printf("Could not create a game board.bmp texture: %s\n", SDL_GetError());
		 GameWindowDestroy(res);
		return NULL;
	}
	SDL_FreeSurface(loadingSurface); //We finished with the surface -> delete it

	loadingSurface = SDL_LoadBMP("C:/Users/SAPIRA/Documents/GitHub/CHESS/src/graphics/images/restart.bmp");
	if (loadingSurface == NULL) {
		 GameWindowDestroy(res);
		printf("couldn't create restart.bmp surface\n");
		return NULL;
	}
	res->restartTexture = SDL_CreateTextureFromSurface(res->renderer, loadingSurface);
	if (res->restartTexture == NULL) {
		SDL_FreeSurface(loadingSurface);
		 GameWindowDestroy(res);
		printf("couldn't create restart.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("C:/Users/SAPIRA/Documents/GitHub/CHESS/src/graphics/images/save.bmp");
	if (loadingSurface == NULL) {
		GameWindowDestroy(res);
		printf("couldn't create save.bmp surface\n");
		return NULL;
	}
	res->saveTexture = SDL_CreateTextureFromSurface(res->renderer, loadingSurface);
	if (res->saveTexture == NULL) {
		SDL_FreeSurface(loadingSurface);
		GameWindowDestroy(res);
		printf("couldn't create save.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("C:/Users/SAPIRA/Documents/GitHub/CHESS/src/graphics/images/load.bmp");
	if (loadingSurface == NULL) {
		GameWindowDestroy(res);
		printf("couldn't create load.bmp surface in game window\n");
		return NULL;
	}
	res->loadFromGameTexture = SDL_CreateTextureFromSurface(res->renderer, loadingSurface);
	if (res->loadFromGameTexture == NULL) {
		SDL_FreeSurface(loadingSurface);
		GameWindowDestroy(res);
		printf("couldn't create load.bmp texture in game window\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("C:/Users/SAPIRA/Documents/GitHub/CHESS/src/graphics/images/undo.bmp");
	if (loadingSurface == NULL) {
		GameWindowDestroy(res);
		printf("couldn't create undo.bmp surface\n");
		return NULL;
	}
	res->undoTexture = SDL_CreateTextureFromSurface(res->renderer, loadingSurface);
	if (res->undoTexture == NULL) {
		SDL_FreeSurface(loadingSurface);
		GameWindowDestroy(res);
		printf("couldn't create undo.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("C:/Users/SAPIRA/Documents/GitHub/CHESS/src/graphics/images/mainMenu.bmp");
	if (loadingSurface == NULL) {
		GameWindowDestroy(res);
		printf("couldn't create mainMenu.bmp surface\n");
		return NULL;
	}
	res->mainMenuTexture = SDL_CreateTextureFromSurface(res->renderer, loadingSurface);
	if (res->mainMenuTexture == NULL) {
		SDL_FreeSurface(loadingSurface);
		GameWindowDestroy(res);
		printf("couldn't create mainMenu.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("C:/Users/SAPIRA/Documents/GitHub/CHESS/src/graphics/images/exit.bmp");
	if (loadingSurface == NULL) {
		GameWindowDestroy(res);
		printf("couldn't create exit.bmp surface\n");
		return NULL;
	}
	res->exitFromGameTexture = SDL_CreateTextureFromSurface(res->renderer, loadingSurface);
	if (res->exitFromGameTexture == NULL) {
		SDL_FreeSurface(loadingSurface);
		GameWindowDestroy(res);
		printf("couldn't create exit.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);


	/*white pawn creation*/
	loadingSurface = SDL_LoadBMP("images/whitePawn.bmp");
	if (loadingSurface == NULL) {
		printf("Could not create a whitePawn.bmp surface: %s\n", SDL_GetError());
		 GameWindowDestroy(res);
		return NULL;
	}

	SDL_SetColorKey(loadingSurface, SDL_TRUE, SDL_MapRGB(loadingSurface->format, 255,255, 255));
	res->whitePawnTexture = SDL_CreateTextureFromSurface(res->renderer, loadingSurface);
	if (res->whitePawnTexture == NULL) {
		printf("Could not create a whitePawn.bmp texture: %s\n", SDL_GetError());
		 GameWindowDestroy(res);
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);
	/*white bishop creation*/
	loadingSurface = SDL_LoadBMP("images/whiteBishop.bmp");
	if (loadingSurface == NULL) {
		printf("Could not create a whiteBishop.bmp surface: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	//We use SetColorKey to make texture background transparent
	 
	SDL_SetColorKey(loadingSurface, SDL_TRUE,
		SDL_MapRGB(loadingSurface->format, 255, 255, 255));
	res->whiteBishopTexture = SDL_CreateTextureFromSurface(res->renderer, loadingSurface);
	if (res->whiteBishopTexture == NULL) {
		printf("Could not create a whiteBishop.bmp texture: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);
	/*white knight creation*/
	loadingSurface = SDL_LoadBMP("images/whiteKnight.bmp");
	if (loadingSurface == NULL) {
		printf("Could not create a whiteKnight.bmp surface: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	//We use SetColorKey to make texture background transparent
	 
	SDL_SetColorKey(loadingSurface, SDL_TRUE,
		SDL_MapRGB(loadingSurface->format, 255, 255, 255));
	res->whiteKnightTexture = SDL_CreateTextureFromSurface(res->renderer, loadingSurface);
	if (res->whiteKnightTexture == NULL) {
		printf("Could not create a whiteKnight.bmp texture: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);
	/*white rook creation*/
	loadingSurface = SDL_LoadBMP("images/whiteRook.bmp");
	if (loadingSurface == NULL) {
		printf("Could not create a whiteRook.bmp surface: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	//We use SetColorKey to make texture background transparent
	 
	SDL_SetColorKey(loadingSurface, SDL_TRUE,
		SDL_MapRGB(loadingSurface->format, 255,255, 255));
	res->whiteRookTexture = SDL_CreateTextureFromSurface(res->renderer, loadingSurface);
	if (res->whiteRookTexture == NULL) {
		printf("Could not create a whiteRook.bmp texture: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);
	/*white king creation*/
	loadingSurface = SDL_LoadBMP("images/whiteKing.bmp");
	if (loadingSurface == NULL) {
		printf("Could not create a whiteKing.bmp surface: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	//We use SetColorKey to make texture background transparent
	 
	SDL_SetColorKey(loadingSurface, SDL_TRUE,
		SDL_MapRGB(loadingSurface->format, 255, 255, 255));
	res->whiteKingTexture = SDL_CreateTextureFromSurface(res->renderer, loadingSurface);
	if (res->whiteKingTexture == NULL) {
		printf("Could not create a whiteKing.bmp texture: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);
	/*white queen creation*/
	loadingSurface = SDL_LoadBMP("images/whiteQueen.bmp");
	if (loadingSurface == NULL) {
		printf("Could not create a whiteQueen.bmp surface: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	//We use SetColorKey to make texture background transparent
	 
	SDL_SetColorKey(loadingSurface, SDL_TRUE,
		SDL_MapRGB(loadingSurface->format, 255,255, 255));
	res->whiteQueenTexture = SDL_CreateTextureFromSurface(res->renderer, loadingSurface);
	if (res->whiteQueenTexture == NULL) {
		printf("Could not create a whiteQueen.bmp texture: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);
	/*black pawn creation*/
	loadingSurface = SDL_LoadBMP("images/blackPawn.bmp");
	if (loadingSurface == NULL) {
		printf("Could not create a blackPawn.bmp surface: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	//We use SetColorKey to make texture background transparent
	 
	SDL_SetColorKey(loadingSurface, SDL_TRUE,
		SDL_MapRGB(loadingSurface->format, 255,255, 255));
	res->blackPawnTexture = SDL_CreateTextureFromSurface(res->renderer, loadingSurface);
	if (res->blackPawnTexture == NULL) {
		printf("Could not create a blackPawn.bmp texture: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);
	/*black bishop creation*/
	loadingSurface = SDL_LoadBMP("images/blackBishop.bmp");
	if (loadingSurface == NULL) {
		printf("Could not create a blackBishop.bmp surface: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	//We use SetColorKey to make texture background transparent
	 
	SDL_SetColorKey(loadingSurface, SDL_TRUE,
		SDL_MapRGB(loadingSurface->format, 255,255, 255));
	res->blackBishopTexture = SDL_CreateTextureFromSurface(res->renderer, loadingSurface);
	if (res->blackBishopTexture == NULL) {
		printf("Could not create a blackBishop.bmp texture: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);
	/*black knight creation*/
	loadingSurface = SDL_LoadBMP("images/blackKnight.bmp");
	if (loadingSurface == NULL) {
		printf("Could not create a blackKnight.bmp surface: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	//We use SetColorKey to make texture background transparent
	 
	SDL_SetColorKey(loadingSurface, SDL_TRUE,
		SDL_MapRGB(loadingSurface->format, 255,255, 255));
	res->blackKnightTexture = SDL_CreateTextureFromSurface(res->renderer, loadingSurface);
	if (res->blackKnightTexture == NULL) {
		printf("Could not create a blackKnight.bmp texture: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);
	/*black rook creation*/
	loadingSurface = SDL_LoadBMP("images/blackRook.bmp");
	if (loadingSurface == NULL) {
		printf("Could not create a blackRook.bmp surface: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	//We use SetColorKey to make texture background transparent
	 
	SDL_SetColorKey(loadingSurface, SDL_TRUE,
		SDL_MapRGB(loadingSurface->format, 255,255, 255));
	res->blackRookTexture = SDL_CreateTextureFromSurface(res->renderer, loadingSurface);
	if (res->blackRookTexture == NULL) {
		printf("Could not create a blackRook.bmp texture: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);
	/*black king creation*/
	loadingSurface = SDL_LoadBMP("images/blackKing.bmp");
	if (loadingSurface == NULL) {
		printf("Could not create a blackKing.bmp surface: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	//We use SetColorKey to make texture background transparent
	 
	SDL_SetColorKey(loadingSurface, SDL_TRUE,
		SDL_MapRGB(loadingSurface->format, 255, 255, 255));
	res->blackKingTexture = SDL_CreateTextureFromSurface(res->renderer, loadingSurface);
	if (res->blackKingTexture == NULL) {
		printf("Could not create a blackKing.bmp texture: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);
	/*black queen creation*/
	loadingSurface = SDL_LoadBMP("images/blackQueen.bmp");
	if (loadingSurface == NULL) {
		printf("Could not create a blackQueen.bmp surface: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	//We use SetColorKey to make texture background transparent
	
	SDL_SetColorKey(loadingSurface, SDL_TRUE,
		SDL_MapRGB(loadingSurface->format, 255, 255, 255));
	res->blackQueenTexture = SDL_CreateTextureFromSurface(res->renderer, loadingSurface);
	if (res->blackQueenTexture == NULL) {
		printf("Could not create a blackQueen.bmp texture: %s\n", SDL_GetError());
		GameWindowDestroy(res);
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);
	return res;
}

void  GameWindowDestroy( GameWin* src) {
	if (!src) {
		return;
	}
	 deleteGame(src->game);
	 /*additional textures*/
	 if (src->restartTexture != NULL) {
		 SDL_DestroyTexture(src->restartTexture);
	 }
	 if (src->saveTexture != NULL) {
		 SDL_DestroyTexture(src->saveTexture);
	 }
	 if (src->loadFromGameTexture != NULL) {
		 SDL_DestroyTexture(src->loadFromGameTexture);
	 }
	 if (src->undoTexture != NULL) {
		 SDL_DestroyTexture(src->undoTexture);
	 }
	 if (src->mainMenuTexture != NULL) {
		 SDL_DestroyTexture(src->mainMenuTexture);
	 }
	 if (src->exitFromGameTexture != NULL) {
		 SDL_DestroyTexture(src->exitFromGameTexture);
	 }
	 /*pawn delete*/
	if (src->whitePawnTexture != NULL) {
		SDL_DestroyTexture(src->whitePawnTexture);
	}
	if (src->blackPawnTexture != NULL) {
		SDL_DestroyTexture(src->blackPawnTexture);
	}
	/*bishop delete*/
	if (src->whiteBishopTexture != NULL) {
		SDL_DestroyTexture(src->whiteBishopTexture);
	}
	if (src->blackBishopTexture != NULL) {
		SDL_DestroyTexture(src->blackBishopTexture);
	}
	/*knight delete*/
	if (src->whiteKnightTexture != NULL) {
		SDL_DestroyTexture(src->whiteKnightTexture);
	}
	if (src->blackKnightTexture != NULL) {
		SDL_DestroyTexture(src->blackKnightTexture);
	}
	/*rook delete*/
	if (src->whiteRookTexture != NULL) {
		SDL_DestroyTexture(src->whiteRookTexture);
	}
	if (src->blackRookTexture != NULL) {
		SDL_DestroyTexture(src->blackRookTexture);
	}
	/*queen delete*/
	if (src->whiteQueenTexture != NULL) {
		SDL_DestroyTexture(src->whiteQueenTexture);
	}
	if (src->blackQueenTexture != NULL) {
		SDL_DestroyTexture(src->blackQueenTexture);
	}
	/*king delete*/
	if (src->whiteKingTexture != NULL) {
		SDL_DestroyTexture(src->whiteKingTexture);
	}
	if (src->blackKingTexture != NULL) {
		SDL_DestroyTexture(src->blackKingTexture);
	}
	/*other definitions delete*/
	if (src->bgTexture != NULL) {
		SDL_DestroyTexture(src->bgTexture);
	}
	if (src->renderer != NULL) {
		SDL_DestroyRenderer(src->renderer);
	}
	if (src->window != NULL) {
		SDL_DestroyWindow(src->window);
	}
	free(src);
}
/*TODO update the "TODO VERY IMPORTANT" part
this is where the current state of the game is drawn*/
void  GameWindowDraw( GameWin* src, int draggedOriginsCurDest[], char movingPiece[]) {
	//printf("entered game draw\n");
	int dragged = 0;
	char piece = '0';

	if (src == NULL) {
		return;
	}
	
	//TODO, again, originally were 600, changed both
	//so here, in contrast to how i theoretically tried to make MainWindow bg work,
	//they made it work like this
	//but when i changed the height and width, it changed too, in a weird way.
	//however, only because the original didnt match the size of the change
	//then again, i'd have to adjust the places of the pieces, apparently, anyway.
	SDL_Rect rec = { .x = 0,.y = 0,.w = 900,.h = 608 };
	SDL_Rect recRestart = { .x = 600,.y = 10,.w = 100,.h = 70 }; 
	SDL_Rect recSave = { .x = 600,.y = 112,.w = 100,.h = 70 };
	SDL_Rect recLoad = { .x = 600,.y = 214,.w = 100,.h = 70 };
	SDL_Rect recUndo = { .x = 600,.y = 316,.w = 100,.h = 70 };
	SDL_Rect recMainMenu = { .x = 600,.y = 418,.w = 100,.h = 70 };
	SDL_Rect recExit = { .x = 600,.y = 520,.w = 100,.h = 70 };
	SDL_SetRenderDrawColor(src->renderer, 255, 255, 255, 255);
	SDL_RenderClear(src->renderer);
	SDL_RenderCopy(src->renderer, src->bgTexture, NULL, &rec);
	SDL_RenderCopy(src->renderer, src->exitFromGameTexture, NULL, &recExit);
	SDL_RenderCopy(src->renderer, src->restartTexture, NULL, &recRestart);
	SDL_RenderCopy(src->renderer, src->loadFromGameTexture, NULL, &recLoad);
	SDL_RenderCopy(src->renderer, src->saveTexture, NULL, &recSave);
	SDL_RenderCopy(src->renderer, src->undoTexture, NULL, &recUndo);
	SDL_RenderCopy(src->renderer, src->mainMenuTexture, NULL, &recMainMenu);

	/*TODO VERY IMPORTANT 
	this should be done only after checking the move
	AND we need to change the place of the piece to the goal place
	POSSIBLY DO ALL THIS IN A FUNC, AND CHANGE FOR DRAG N DROP ABILITIES N SUCH
	apparently this is only supposed to check and update("paste") the current game board
	in the gui mode*/
	int i = 0, j = 0;
	for (i = 0; i < 8; i++) { //TODO CHANGED N_ROWS AND N_COLUMNS into 8
		for (j = 0; j<8; j++) {// בהתאמה
			/*TODO i need to make adjustments to the origins, curs and dests 
			so they'd actually match the i and j on a console board*/
			rec.h = 71;
			rec.w = 65;
			if (draggedOriginsCurDest[0] == 0 || i != draggedOriginsCurDest[1] ||
				draggedOriginsCurDest[2] != j) {
				rec.x =20+ j *65;
				rec.y =20+ i *71;
				//TODO check the all the sizes of all the pieces, including their locations
				//TODO not necessarily this way, since i also need to draw the moving of a piece

				piece = src->game->gameBoard[i][j];
			}
			else {
				rec.x = draggedOriginsCurDest[3];
				rec.y = draggedOriginsCurDest[4];
				piece = movingPiece[0];
			}
			switch(piece){
			case 'r': 
				SDL_RenderCopy(src->renderer, src->whiteRookTexture, NULL, &rec);
				break;
			case 'R':
				SDL_RenderCopy(src->renderer, src->blackRookTexture, NULL, &rec);
				break;
			case 'm':
				SDL_RenderCopy(src->renderer, src->whitePawnTexture, NULL, &rec);
				break;
			case 'M':
				SDL_RenderCopy(src->renderer, src->blackPawnTexture, NULL, &rec);
				break;
			case 'n':
				SDL_RenderCopy(src->renderer, src->whiteKnightTexture, NULL, &rec);
				break;
			case 'N':
				SDL_RenderCopy(src->renderer, src->blackKnightTexture, NULL, &rec);
				break;
			case 'b':
				SDL_RenderCopy(src->renderer, src->whiteBishopTexture, NULL, &rec);
				break;
			case 'B':
				SDL_RenderCopy(src->renderer, src->blackBishopTexture, NULL, &rec);
				break;
			case 'q':
				SDL_RenderCopy(src->renderer, src->whiteQueenTexture, NULL, &rec);
				break;
			case 'Q':
				SDL_RenderCopy(src->renderer, src->blackQueenTexture, NULL, &rec);
				break;
			case 'k':
				SDL_RenderCopy(src->renderer, src->whiteKingTexture, NULL, &rec);
				break;
			case 'K':
				SDL_RenderCopy(src->renderer, src->blackKingTexture, NULL, &rec);
				break;
			}	
		}
	}
	SDL_RenderPresent(src->renderer);
}

/*TODO update this entire func
this is where the "fun" happens. the actual game.*/

GAME_EVENT  GameWindowHandleEvent(GameWin* src, SDL_Event* event, int draggedOriginsCurDest[], char piece[]) {
	if (event == NULL || src == NULL) {
		return  GAME_EVENT_INVALID_ARGUMENT;
	}
	if (event->button.button == SDL_BUTTON_LEFT) {
		switch (event->type) {
		case SDL_MOUSEBUTTONDOWN:
			if (draggedOriginsCurDest[0] == 0 && draggedOriginsCurDest[1] == -1
				&& draggedOriginsCurDest[2] == -1 && piece[0] == '0' ) {
				//	TODO should add an if, to check the color of the piece
				//that the user is trying to move, if it is rlly their own color
				//if yes, does as below
				//if not, shows a simple window that says it's not their piece
				draggedOriginsCurDest[1] = event->button.x;
				draggedOriginsCurDest[2] = event->button.y;
				draggedOriginsCurDest[0] = 1;
				//TODO i need to somehow find out what piece it is...

			}
			if (piece[0] != '0') {
				if (event->type == SDL_MOUSEMOTION) {
					draggedOriginsCurDest[3] = event->button.x;
					draggedOriginsCurDest[4] = event->button.y;
					return GAME_EVENT_MOVING;
				}
				return GAME_EVENT_PIECE_CHOSEN;
			}
			break;


		case SDL_MOUSEBUTTONUP:
			//TODO instead of TicTacToeSetMove, changed into makeMove
			if (draggedOriginsCurDest[0] ==1  && draggedOriginsCurDest[1] != -1
				&& draggedOriginsCurDest[2] != -1 && piece[0] != '0') {
				/*TODO 00 in gui board is up, not down, so i might have to flip the places
				anyway, this are the destinations i need to match to makeMove event->button.y / 200,
				event->button.x / 200
				and this are the origins i need to match to makeMove originX, originY
				*/
				makeMove(src->game, event->button.y / 200,
					event->button.x / 200);
				draggedOriginsCurDest[0] = 0;
				piece[0] = '0';
				draggedOriginsCurDest[1] = -1;
				draggedOriginsCurDest[2] = -1;

				//TODO changed TicTacToeCheckWinner into currentLost
				//TODO need to check what currentLost returns and how to connect it to the color of the winner
			   //TODO probably need some && in the if's for that
				bool winner = currentLost(src->game);
				if (winner == true) {
					return  GAME_EVENT_WHITE_WON;// X IS SUPPOSED TO BE WHITE
				}
				else if (winner == false) {
					return  GAME_EVENT_BLACK_WON; // O IS SUPPOSED TO BE BLACK
				}
				//TODO check this func returns false if game isn't ending in a tie
				//otherwise, need to add another if
				else if (isGameTied(src->game)) {
					return  GAME_EVENT_TIE;
				}
			}
			if (isClickOnLoadFromGame(event->button.x, event->button.y)) {
				return GAME_EVENT_LOAD;
			}
			if (isClickOnExitFromGame(event->button.x, event->button.y)) {
				nullifyHelpingArrays(draggedOriginsCurDest, 7, piece, 1);
				return GAME_EVENT_EXIT;
			}
			if (isClickOnMainMenu(event->button.x, event->button.y)) {
				nullifyHelpingArrays(draggedOriginsCurDest, 7, piece, 1);
				return GAME_EVENT_MAIN_MENU;
			}
			if (isClickOnRestart(event->button.x, event->button.y)) {
				nullifyHelpingArrays(draggedOriginsCurDest, 7, piece, 1);
				return GAME_EVENT_RESTART;
			}
			if (isClickOnSave(event->button.x, event->button.y)) {
				return GAME_EVENT_SAVE;
			}
			if (isClickOnUndo(event->button.x, event->button.y)) {
				return GAME_EVENT_UNDO;
			}
			break;
		case SDL_WINDOWEVENT:
			if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
				return  GAME_EVENT_QUIT;
			}
			break;
		default:
			return  GAME_EVENT_NONE;
		}
		
	}
	return  GAME_EVENT_NONE;
}

void nullifyHelpingArrays(int arr[], int length, char arr2[], int arr2Len) {
	for (int i = 0; i < length; i++) {
		arr[i] = 0;
		if (i < arr2Len)
			arr2[i] = '0';
	}
	for (int j = length - 1; j < arr2Len; j++) {
		arr2[j] = '0';
	}
}