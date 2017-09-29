#include "ChessMainWin.h"
#include <SDL_video.h>
#include <stdio.h>

//Inner functions
int isClickOnNewGame(int x, int y) {
	if ((x >= 20 && x <= 120) && (y >= 40 && y <= 140)) {
		return 1;
	}
	return 0;
}

int isClickOnExit(int x, int y) {
	if ((x >= 730 && x <= 830) && (y >=40 && y <= 140)) {
		return 1;
	}
	return 0;
}

int isClickOnLoad(int x, int y) {
	if ((x >= 220 && x <= 320) && (y >= 40 && y <= 140)) {
		return 1;
	}
	return 0;
}

int isClickOn1players(int x, int y) {
	if ((x >= 20 && x <= 120) && (y >= 40 && y <= 140)) {
		return 1;
	}
	return 0;
}

int isClickOn2players(int x, int y) {
	if ((x >= 220 && x <= 320) && (y >= 40 && y <= 140)) {
		return 1;
	}
	return 0;
}

int isClickOnStart(int x, int y) {
	if ((x >= 750 && x <= 850) && (y >= 500 && y <= 600)) {
		return 1;
	}
	return 0;
}

int isClickOnBack(int x, int y) {
	if ((x >= 180 && x <= 280) && (y >= 160 && y <= 260))
		return 1;
	return 0;
}

int isClickOnNext(int x, int y) {
	if ((x >= 750 && x <= 850) && (y >= 500 && y <= 600)) {
		return 1;
	}
	return 0;
}

int isClickOnBlack(int x, int y) {
	if ((x >= 130 && x <= 230) && (y >= 50 && y <= 150))
		return 1;
	return 0;
}
int isClickOnWhite(int x, int y) {
	if ((x >= 20 && x <= 120) && (y >= 50 && y <= 150))
		return 1;
	return 0;
}
int isClickOnDiff(int x, int y) {
	if ((x >= 20 && x <= 120) && (y >= 50 && y <= 150))
		return 1;
	if ((x >= 130 && x <= 230) && (y >= 50 && y <= 150))
		return 2;
	if ((x >= 240 && x <= 340) && (y >= 50 && y <= 150))
		return 3;
	if ((x >= 350 && x <= 450) && (y >= 50 && y <= 150))
		return 4;
	return 0;
}


/*todo add textures of pressed buttons
implement them so they show-> not only here, but in draw as well
and anywhere else, where they need to be*/
 MainWin*  MainWindowCreate() {
	MainWin* res = NULL;
	SDL_Surface* loadingSurface = NULL;

	res = (MainWin*)calloc(sizeof(MainWin), sizeof(char));
	if (res == NULL) {// means calloc didn't work
		return NULL;
	}
	res->mainMenu = 1;//TODO also did this in gui manager during the creation of mainWin, delete one of them.
	res->newGameFromMainMenuPressed = 0;
	res->loadFromMainMenuPressed = 0;
	res->numPlayersPressed = 0;
	res->onePlayerPressed = 0;
	res->nextToDiffWinPressed = 0;
	res->backFromNumPlayersPressed = 0;
	res->diffPressed = 0;
	res->nextToColorWinPressed = 0;
	res->backFromDiffPressed = 0;
	res->colorPressed = 0;
	res->startFromColorWinPressed = 0;
	res->backFromColorPressed = 0;
					  
					  
	// Create an application window with the following settings:
	res->mainWindow = SDL_CreateWindow("Chess by Itay and Sapir", // window title- i dont see a need for a title in the presentation
		SDL_WINDOWPOS_CENTERED,           // initial x position
		SDL_WINDOWPOS_CENTERED,           // initial y position
		900,                               // width, in pixels
		674,								// height, in pixels	
		SDL_WINDOW_OPENGL);

	// Check that the window was successfully created
	if (res->mainWindow == NULL) {
		 MainWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}
	res->mainRenderer = SDL_CreateRenderer(res->mainWindow, -1,SDL_RENDERER_ACCELERATED);
	if (res->mainRenderer == NULL) {
		// In the case that the window could not be made...
		 MainWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}
	
	/*NEW GAME "BUTTON" INFORMATION*/
	loadingSurface = SDL_LoadBMP("images/newGame.bmp");
	if (loadingSurface == NULL) {
		printf("couldn't create newGame.bmp surface, the error is: %s\n", SDL_GetError());
		MainWindowDestroy(res);
		return NULL;
	}
	
	res->newGameTexture = SDL_CreateTextureFromSurface(res->mainRenderer, loadingSurface);
	if (res->newGameTexture == NULL) {
		printf("couldn't create newGame.bmp texture, the error is: %s\n", SDL_GetError());
		
		 MainWindowDestroy(res);
		
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	/*LOAD "BUTTON" INFORMATION*/
	loadingSurface = SDL_LoadBMP("images/load.bmp");
	if (loadingSurface == NULL) {
		MainWindowDestroy(res);
		printf("couldn't create load.bmp surface\n");
		return NULL;
	}
	res->loadTexture = SDL_CreateTextureFromSurface(res->mainRenderer,
		loadingSurface);
	if (res->loadTexture == NULL) {
		SDL_FreeSurface(loadingSurface);
		MainWindowDestroy(res);
		printf("couldn't create load.bmp texture\n");
		return NULL;
	}

	SDL_FreeSurface(loadingSurface);

	/*EXIT "BUTTON" INFORMATION*/
	loadingSurface = SDL_LoadBMP("images/exit.bmp");
	if (loadingSurface == NULL) {
		 MainWindowDestroy(res);
		printf("couldn't create exit.bmp surface\n");
		return NULL;
	}
	res->exitTexture = SDL_CreateTextureFromSurface(res->mainRenderer,
		loadingSurface);
	if (res->exitTexture == NULL) {
		SDL_FreeSurface(loadingSurface);
		 MainWindowDestroy(res);
		printf("couldn't create exit.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	/*BG image*/
	loadingSurface = SDL_LoadBMP("images/startBG.bmp");
	if (loadingSurface == NULL) {
		MainWindowDestroy(res);
		printf("couldn't create startBG.bmp surface\n");
		return NULL;
	}
	res->startBGTexture = SDL_CreateTextureFromSurface(res->mainRenderer,
		loadingSurface);
	if (res->startBGTexture == NULL) {
		SDL_FreeSurface(loadingSurface);
		MainWindowDestroy(res);
		printf("couldn't create startBG.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	/*next "BUTTON" INFORMATION*/
	loadingSurface = SDL_LoadBMP("images/next.bmp");
	if (loadingSurface == NULL) {
		printf("couldn't create next.bmp surface, the error is: %s\n", SDL_GetError());
		MainWindowDestroy(res);
		return NULL;
	}

	res->nextToDiffWinTexture = SDL_CreateTextureFromSurface(res->mainRenderer, loadingSurface);
	if (res->nextToDiffWinTexture == NULL) {
		printf("couldn't create next.bmp texture, the error is:  %s\n", SDL_GetError());

		MainWindowDestroy(res);

		return NULL;
	}
	res->nextToColorWinTexture=res->nextToDiffWinTexture;
	SDL_FreeSurface(loadingSurface);

	/*back "BUTTON" INFORMATION*/
	loadingSurface = SDL_LoadBMP("images/back.bmp");
	if (loadingSurface == NULL) {
		printf("couldn't create back.bmp surface, the error is: %s\n", SDL_GetError());
		MainWindowDestroy(res);
		return NULL;
	}

	res->backFromNumPlayersTexture = SDL_CreateTextureFromSurface(res->mainRenderer, loadingSurface);
	if (res->backFromNumPlayersTexture == NULL) {
		printf("couldn't create back.bmp texture, the error is: %s\n", SDL_GetError());

		MainWindowDestroy(res);

		return NULL;
	}
	res->backFromColorTexture = res->backFromNumPlayersTexture;
	res->backFromDiffTexture = res->backFromNumPlayersTexture;
	SDL_FreeSurface(loadingSurface);

	/*start "BUTTON" INFORMATION*/
	loadingSurface = SDL_LoadBMP("images/start.bmp");
	if (loadingSurface == NULL) {
		printf("couldn't create start.bmp surface, the error is: %s\n", SDL_GetError());
		MainWindowDestroy(res);
		return NULL;
	}

	res->startFromNumPlayersTexture = SDL_CreateTextureFromSurface(res->mainRenderer, loadingSurface);
	if (res->startFromNumPlayersTexture == NULL) {
		printf("couldn't create start.bmp texture, the error is: %s\n", SDL_GetError());

		MainWindowDestroy(res);

		return NULL;
	}
	res->startFromColorTexture = res->startFromNumPlayersTexture;
	SDL_FreeSurface(loadingSurface);

	/*1 player image*/
	loadingSurface = SDL_LoadBMP("images/1Players.bmp");
	if (loadingSurface == NULL) {
		MainWindowDestroy(res);
		printf("couldn't create 1Players.bmp surface\n");
		return NULL;
	}
	res->onePlayerTexture = SDL_CreateTextureFromSurface(res->mainRenderer,
		loadingSurface);
	if (res->onePlayerTexture == NULL) {
		SDL_FreeSurface(loadingSurface);
		MainWindowDestroy(res);
		printf("couldn't create 1Players.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	/*2 players image*/
	loadingSurface = SDL_LoadBMP("images/2Players.bmp");
	if (loadingSurface == NULL) {
		MainWindowDestroy(res);
		printf("couldn't create 2Players.bmp surface\n");
		return NULL;
	}
	res->twoPlayerTexture = SDL_CreateTextureFromSurface(res->mainRenderer,
		loadingSurface);
	if (res->twoPlayerTexture == NULL) {
		SDL_FreeSurface(loadingSurface);
		MainWindowDestroy(res);
		printf("couldn't create 2Players.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	/*noob image*/
	loadingSurface = SDL_LoadBMP("images/diff1.bmp");
	if (loadingSurface == NULL) {
		MainWindowDestroy(res);
		printf("couldn't create diff1.bmp surface\n");
		return NULL;
	}
	res->noobTexture = SDL_CreateTextureFromSurface(res->mainRenderer,
		loadingSurface);
	if (res->noobTexture == NULL) {
		SDL_FreeSurface(loadingSurface);
		MainWindowDestroy(res);
		printf("couldn't create diff1.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	/*easy image*/
	loadingSurface = SDL_LoadBMP("images/diff2.bmp");
	if (loadingSurface == NULL) {
		MainWindowDestroy(res);
		printf("couldn't create diff2.bmp surface\n");
		return NULL;
	}
	res->easyTexture = SDL_CreateTextureFromSurface(res->mainRenderer,
		loadingSurface);
	if (res->easyTexture == NULL) {
		SDL_FreeSurface(loadingSurface);
		MainWindowDestroy(res);
		printf("couldn't create diff2.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	/*moderate image*/
	loadingSurface = SDL_LoadBMP("images/diff3.bmp");
	if (loadingSurface == NULL) {
		MainWindowDestroy(res);
		printf("couldn't create diff3.bmp surface\n");
		return NULL;
	}
	res->moderateTexture = SDL_CreateTextureFromSurface(res->mainRenderer,
		loadingSurface);
	if (res->moderateTexture == NULL) {
		SDL_FreeSurface(loadingSurface);
		MainWindowDestroy(res);
		printf("couldn't create diff3.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	/*hard image*/
	loadingSurface = SDL_LoadBMP("images/diff4.bmp");
	if (loadingSurface == NULL) {
		MainWindowDestroy(res);
		printf("couldn't create diff4.bmp surface\n");
		return NULL;
	}
	res->hardTexture = SDL_CreateTextureFromSurface(res->mainRenderer,
		loadingSurface);
	if (res->hardTexture == NULL) {
		SDL_FreeSurface(loadingSurface);
		MainWindowDestroy(res);
		printf("couldn't create diff4.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);
	
	/*white img*/
	loadingSurface = SDL_LoadBMP("images/white.bmp");
	if (loadingSurface == NULL) {
		MainWindowDestroy(res);
		printf("couldn't create white.bmp surface\n");
		return NULL;
	}
	res->whiteTexture = SDL_CreateTextureFromSurface(res->mainRenderer,
		loadingSurface);
	if (res->whiteTexture == NULL) {
		SDL_FreeSurface(loadingSurface);
		MainWindowDestroy(res);
		printf("couldn't create white.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);
	
	/*black img*/
	loadingSurface = SDL_LoadBMP("images/black.bmp");
	if (loadingSurface == NULL) {
		MainWindowDestroy(res);
		printf("couldn't create black.bmp surface\n");
		return NULL;
	}
	res->blackTexture = SDL_CreateTextureFromSurface(res->mainRenderer,
		loadingSurface);
	if (res->blackTexture == NULL) {
		SDL_FreeSurface(loadingSurface);
		MainWindowDestroy(res);
		printf("couldn't create black.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);


	return res;
}

void  MainWindowDestroy( MainWin* src) {
	if (!src) {
		return;
	}
	if (src->newGameTexture != NULL) {
		SDL_DestroyTexture(src->newGameTexture);
	}
	if (src->exitTexture != NULL) {
		SDL_DestroyTexture(src->exitTexture);
	}
	if (src->nextToDiffWinTexture != NULL) {
		SDL_DestroyTexture(src->nextToDiffWinTexture);
	}
	/*supposedly unnecessary, but for just in case*/
	if (src->nextToColorWinTexture != NULL) {
		SDL_DestroyTexture(src->nextToColorWinTexture);
	}
	if (src->backFromNumPlayersTexture != NULL) {
		SDL_DestroyTexture(src->backFromNumPlayersTexture);
	}
	/*supposedly unnecessary, but for just in case*/
	if (src->backFromColorTexture != NULL) {
		SDL_DestroyTexture(src->backFromColorTexture);
	}
	if (src->backFromDiffTexture != NULL) {
		SDL_DestroyTexture(src->backFromDiffTexture);
	}

	if (src->startFromNumPlayersTexture != NULL) {
		SDL_DestroyTexture(src->startFromNumPlayersTexture);
	}
	if (src->startFromColorTexture != NULL) {
		SDL_DestroyTexture(src->startFromColorTexture);
	}
	if (src->startBGTexture != NULL) {
		SDL_DestroyTexture(src->startBGTexture);
	}
	if (src->mainRenderer != NULL) {
		SDL_DestroyRenderer(src->mainRenderer);
	}
	if (src->mainWindow != NULL) {
		SDL_DestroyWindow(src->mainWindow);
	}
	

	free(src);
}
/*todo possibly the backs here are useless, thanks to the handle update of them*/
void  MainWindowDraw( MainWin* src) {
	if (src == NULL) {
		return;
	}
	SDL_RenderClear(src->mainRenderer);
	SDL_Rect startBGR = { .x = 0,.y = 0,.h = 674,.w = 900 };
	SDL_RenderCopy(src->mainRenderer, src->startBGTexture, NULL, &startBGR);

	SDL_Rect backR = { .x = 180,.y = 160,.h = 100,.w = 100 };//matches click
	SDL_Rect nextR = { .x = 750,.y = 500,.h = 100,.w = 100 };//matches click
	SDL_Rect startR = { .x = 750,.y = 500,.h = 100,.w = 100 };//matches click

	SDL_Rect onePlayersR = { .x = 20,.y = 40,.h = 100,.w = 100 };//matches click
	SDL_Rect twoPlayersR = { .x = 220,.y = 40,.h = 100,.w = 100 };// matches click

	if (src->nextToColorWinPressed == 1) {
		printf("in draw entered if (src->nextToColorWinPressed == 1) \n ");
		SDL_Rect whiteR = { .x = 20,.y = 50 ,.h = 100,.w = 100 };//matches click
		SDL_Rect blackR = { .x = 130,.y = 50 ,.h = 100,.w = 100 };//matches click
		SDL_RenderCopy(src->mainRenderer, src->whiteTexture, NULL, &whiteR);
		SDL_RenderCopy(src->mainRenderer, src->blackTexture, NULL, &blackR);
		SDL_RenderCopy(src->mainRenderer, src->backFromColorTexture, NULL, &backR);
		if (src->colorPressed>0) {
			printf("in draw entered if (src->colorPressed>0)\n ");
			SDL_RenderCopy(src->mainRenderer, src->startFromColorTexture, NULL, &startR);
		}
	}
	
	if (src->nextToDiffWinPressed == 1) {
		SDL_Rect noobR= { .x = 20,.y = 50 ,.h = 100,.w = 100 };//matches click
		SDL_Rect easyR = { .x = 130,.y = 50 ,.h = 100,.w = 100 };//matches click
		SDL_Rect moderateR = {.x = 240,.y = 50 ,.h = 100,.w = 100};//matches click
		SDL_Rect hardR = { .x = 350,.y = 50 ,.h = 100,.w = 100 };//matches click
		SDL_RenderCopy(src->mainRenderer, src->noobTexture, NULL, &noobR);
		SDL_RenderCopy(src->mainRenderer, src->easyTexture, NULL, &easyR);
		SDL_RenderCopy(src->mainRenderer, src->moderateTexture, NULL, &moderateR);
		SDL_RenderCopy(src->mainRenderer, src->hardTexture, NULL, &hardR);
		SDL_RenderCopy(src->mainRenderer, src->backFromDiffTexture, NULL, &backR);
		if (src->diffPressed > 0) {
			SDL_RenderCopy(src->mainRenderer, src->nextToColorWinTexture, NULL, &nextR);
		}	
	}

	if (src->newGameFromMainMenuPressed == 1) {
		SDL_RenderCopy(src->mainRenderer, src->onePlayerTexture, NULL, &onePlayersR);
		SDL_RenderCopy(src->mainRenderer, src->twoPlayerTexture, NULL, &twoPlayersR);
		SDL_RenderCopy(src->mainRenderer, src->backFromNumPlayersTexture, NULL, &backR);
		if (src->onePlayerPressed == 1) {
			SDL_RenderCopy(src->mainRenderer, src->nextToDiffWinTexture, NULL, &nextR);
		}
		else{
			if (src->numPlayersPressed == 1) {
				SDL_RenderCopy(src->mainRenderer, src->startFromNumPlayersTexture, NULL, &startR);
			}
		}
	}
	
	if (src->mainMenu == 1) {
		SDL_Rect newGameR = { .x = 20,.y = 40,.h = 100,.w = 100 };//matches click
		SDL_Rect loadR = { .x = 220,.y = 40,.h = 100,.w = 100 };//matches click
		SDL_Rect exitR = { .x = 730,.y = 40,.h = 100,.w = 100 };//matches click
		SDL_RenderCopy(src->mainRenderer, src->newGameTexture, NULL, &newGameR);
		SDL_RenderCopy(src->mainRenderer, src->loadTexture, NULL, &loadR);
		SDL_RenderCopy(src->mainRenderer, src->exitTexture, NULL, &exitR);
	}
	SDL_RenderPresent(src->mainRenderer);
}

void MainWinNullifyIntSet(MainWin* src) {
	src->mainMenu = 1;
	src->nextToColorWinPressed = 0;
	src->nextToDiffWinPressed = 0;
	src->newGameFromMainMenuPressed = 0;
	src->diffPressed = 0;
	src->colorPressed = 0;
	src->loadFromMainMenuPressed = 0;
	src->numPlayersPressed = 0;
	src->onePlayerPressed = 0;
	src->startFromColorWinPressed = 0;
	src->backFromColorPressed = 0;
	src->backFromDiffPressed = 0;
	src->backFromNumPlayersPressed = 0;
}

MAIN_EVENT  MainWindowHandleEvent(MainWin* src, SDL_Event* event) {
	if (!event) {
		return  MAIN_INVALID_ARGUMENT;
	}
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:// TODO only left mouse button?
		if (src->nextToColorWinPressed == 1) {
			if (isClickOnBack(event->button.x, event->button.y)) {
				src->colorPressed = 0;
				src->backFromColorPressed = 1;
				src->nextToColorWinPressed = 0;
				src->nextToDiffWinPressed = 1;
				return MAIN_COLOR_BACK;
			}
			if (src->colorPressed>0) {
				//printf("in main window handle, entered if (src->colorPressed == 1)\n ");
				if (isClickOnStart(event->button.x, event->button.y)) {
					printf("in main window handle, entered COLOR START ");
					src->startFromColorWinPressed = 1;
					return MAIN_COLOR_START;
				}
			}
			if (isClickOnBlack(event->button.x, event->button.y)) {
				//printf("in main window handle, enteredif (isClickOnBlack(event->button.x, event->button.y))\n ");
				src->colorPressed = 1;
				return MAIN_COLOR_BLACK;
			}
			if (isClickOnWhite(event->button.x, event->button.y)) {
				//printf("in main window handle, entered if (isClickOnWhite(event->button.x, event->button.y))\n ");
				src->colorPressed = 2;
				return MAIN_COLOR_WHITE;
			}

		}
		if (src->nextToDiffWinPressed == 1) {
			//printf("entered another if");
			if (isClickOnBack(event->button.x, event->button.y)) {
				src->backFromDiffPressed = 1;
				src->nextToDiffWinPressed = 0;
				src->diffPressed = 0;
				src->newGameFromMainMenuPressed = 1;
				return MAIN_DIFFICULTY_BACK;
			}
			if (src->diffPressed > 0) {
				if (isClickOnNext(event->button.x, event->button.y)) {
					src->diffPressed = 0;
					src->nextToColorWinPressed = 1;
					src->nextToDiffWinPressed = 0;
					return MAIN_DIFFICULTY_NEXT;
				}
			}
			src->diffPressed = isClickOnDiff(event->button.x, event->button.y);
			if (src->diffPressed > 0) {
				if (src->diffPressed == 1)
					return MAIN_DIFFICULTY_NOOB;
				if (src->diffPressed == 2)
					return MAIN_DIFFICULTY_EASY;
				if (src->diffPressed == 3)
					return MAIN_DIFFICULTY_MODERATE;
				return MAIN_DIFFICULTY_HARD;

			}
		}
		if (src->newGameFromMainMenuPressed == 1) {
			if (isClickOnBack(event->button.x, event->button.y)) {
				src->newGameFromMainMenuPressed = 0;
				src->numPlayersPressed = 0;
				src->onePlayerPressed = 0;
				src->mainMenu = 1;
				return MAIN_NUMPLAYER_BACK;
			}
			if (src->numPlayersPressed == 1 && src->onePlayerPressed == 0) {
				if (isClickOnStart(event->button.x, event->button.y)) {
					src->newGameFromMainMenuPressed = 0;//the other 2 i might need later
					printf("in main window handle, entered NUM PLAYER START ");
					return MAIN_NUMPLAYER_START;
				}
			}
			if (src->numPlayersPressed == 1 && src->onePlayerPressed == 1) {
				if (isClickOnNext(event->button.x, event->button.y)) {
					src->newGameFromMainMenuPressed = 0;
					src->nextToDiffWinPressed = 1;//the other 2 i might need later
					return MAIN_NUMPLAYER_NEXT;
				}
			}
			if (isClickOn2players(event->button.x, event->button.y)) {
				src->numPlayersPressed = 1;
				src->onePlayerPressed = 0;
				return MAIN_NUMPLAYER_TWO;
			}
			if (isClickOn1players(event->button.x, event->button.y)) {
				src->numPlayersPressed = 1;
				src->onePlayerPressed = 1;
				return MAIN_NUMPLAYER_ONE;
			}
		}
		if (src->mainMenu == 1) {
			if (isClickOnNewGame(event->button.x, event->button.y)) {
				src->mainMenu = 0;
				src->newGameFromMainMenuPressed = 1;
				return  MAIN_NEWGAME;
			}
			if (isClickOnLoad(event->button.x, event->button.y)) {
				src->mainMenu = 0;
				src->loadFromMainMenuPressed = 1;
				return MAIN_LOAD;
			}
			else if (isClickOnExit(event->button.x, event->button.y)) {
				return  MAIN_EXIT;
			}
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
			return  MAIN_EXIT;
		}
		break;
	default:
		return  MAIN_NONE;
		}
	return  MAIN_NONE;
}


void  MainWindowHide( MainWin* src) {
	SDL_HideWindow(src->mainWindow);
}

void  MainWindowShow( MainWin* src) {
	SDL_ShowWindow(src->mainWindow);
}
