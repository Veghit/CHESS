#include "ChessLoadWin.h"
#include <SDL_video.h>
#include <stdio.h>

//Inner functions
/*TODO go through the entire of load, and see what/ if something's missing, except for the known part
 update guiManager accordingly*/
int isClickOnBackFromLoad(int x, int y) {
	if ((x >= 180 && x <= 280) && (y >= 160 && y <= 260))
		return 1;
	return 0;
}

int isClickOnLoadFromLoad(int x, int y) {
	if ((x >= 750 && x <= 850) && (y >= 500 && y <= 600)) {
		return 1;
	}
	return 0;
}


int isClickOnSlot(int x, int y) {
	int ctr = countSavedGames();
	int slot;
	if ((x >= 20 && x <= 120) && (y >= 50 && y <= 150))
		slot = 1;
	if ((x >= 130 && x <= 230) && (y >= 50 && y <= 150))
		slot = 2;
	if ((x >= 240 && x <= 340) && (y >= 50 && y <= 150))
		slot = 3;
	if ((x >= 350 && x <= 450) && (y >= 50 && y <= 150))
		slot = 4;
	if ((x >= 460 && x <= 560) && (y >= 50 && y <= 150))
		slot = 5;
	if (slot <= ctr)
		return slot;
	return 0;
}

int countSavedGames() {
	FILE * fp1 = fopen("save1.xml", "r");
	if (fp1 == NULL)
		return 0;
	fclose(fp1);
	FILE * fp2 = fopen("save2.xml", "r");
	if (fp2 == NULL)
		return 1;
	fclose(fp2);
	FILE * fp3 = fopen("save3.xml", "r");
	if (fp3 == NULL)
		return 2;
	fclose(fp3);
	FILE * fp4 = fopen("save4.xml", "r");
	if (fp4 == NULL)
		return 3;
	fclose(fp4);
	FILE * fp5 = fopen("save5.xml", "r");
	if (fp5 == NULL)
		return 4;
	fclose(fp5);
	return 5;
}
LoadWin* loadWindowCreate() {
	LoadWin* res = NULL;
	SDL_Surface* loadingSurface = NULL;

	res = (LoadWin*) calloc(sizeof(LoadWin), sizeof(char));
	if (res == NULL) { // means calloc didn't work
		return NULL;
	}
	res->numSlotsShown = countSavedGames(); 
	res->chosenSlot = 0;
	// Create an application window with the following settings:
	res->LoadWindow = SDL_CreateWindow("Load saved game", // window title- i dont see a need for a title in the presentation
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			900,                               // width, in pixels
			674,								// height, in pixels
			SDL_WINDOW_OPENGL);

	// Check that the window was successfully created
	if (res->LoadWindow == NULL) {
		loadWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}
	res->LoadRenderer = SDL_CreateRenderer(res->LoadWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->LoadRenderer == NULL) {
		// In the case that the window could not be made...
		loadWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}

	/*slot1 "BUTTON" INFORMATION*/
	loadingSurface = SDL_LoadBMP("images/slot1.bmp");
	if (loadingSurface == NULL) {
		printf("couldn't create slot1.bmp surface, the error is: %s\n",
				SDL_GetError());
		loadWindowDestroy(res);
		return NULL;
	}

	res->slot1Texture = SDL_CreateTextureFromSurface(res->LoadRenderer,
			loadingSurface);
	if (res->slot1Texture == NULL) {
		printf("couldn't create slot1.bmp texture, the error is: %s\n",
				SDL_GetError());

		loadWindowDestroy(res);

		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	/*slot2 "BUTTON" INFORMATION*/
	loadingSurface = SDL_LoadBMP("images/slot2.bmp");
	if (loadingSurface == NULL) {
		printf("couldn't create slot2.bmp surface, the error is: %s\n",
				SDL_GetError());
		loadWindowDestroy(res);
		return NULL;
	}

	res->slot2Texture = SDL_CreateTextureFromSurface(res->LoadRenderer,
			loadingSurface);
	if (res->slot2Texture == NULL) {
		printf("couldn't create slot2.bmp texture, the error is: %s\n",
				SDL_GetError());

		loadWindowDestroy(res);

		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	/*NEW GAME "BUTTON" INFORMATION*/
	loadingSurface = SDL_LoadBMP("images/slot3.bmp");
	if (loadingSurface == NULL) {
		printf("couldn't create slot3.bmp surface, the error is: %s\n",
				SDL_GetError());
		loadWindowDestroy(res);
		return NULL;
	}

	res->slot3Texture = SDL_CreateTextureFromSurface(res->LoadRenderer,
			loadingSurface);
	if (res->slot3Texture == NULL) {
		printf("couldn't create slot3.bmp texture, the error is: %s\n",
				SDL_GetError());

		loadWindowDestroy(res);

		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	/*NEW GAME "BUTTON" INFORMATION*/
	loadingSurface = SDL_LoadBMP("images/slot4.bmp");
	if (loadingSurface == NULL) {
		printf("couldn't create slot4.bmp surface, the error is: %s\n",
				SDL_GetError());
		loadWindowDestroy(res);
		return NULL;
	}

	res->slot4Texture = SDL_CreateTextureFromSurface(res->LoadRenderer,
			loadingSurface);
	if (res->slot4Texture == NULL) {
		printf("couldn't create slot4.bmp texture, the error is: %s\n",
				SDL_GetError());

		loadWindowDestroy(res);

		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	/*NEW GAME "BUTTON" INFORMATION*/
	loadingSurface = SDL_LoadBMP("images/slot5.bmp");
	if (loadingSurface == NULL) {
		printf("couldn't create slot5.bmp surface, the error is: %s\n",
				SDL_GetError());
		loadWindowDestroy(res);
		return NULL;
	}

	res->slot5Texture = SDL_CreateTextureFromSurface(res->LoadRenderer,
			loadingSurface);
	if (res->slot5Texture == NULL) {
		printf("couldn't create slot5.bmp texture, the error is: %s\n",
				SDL_GetError());

		loadWindowDestroy(res);

		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	/*LOAD "BUTTON" INFORMATION*/
	loadingSurface = SDL_LoadBMP("images/load.bmp");
	if (loadingSurface == NULL) {
		loadWindowDestroy(res);
		printf("couldn't create load.bmp surface\n");
		return NULL;
	}
	res->loadTexture = SDL_CreateTextureFromSurface(res->LoadRenderer,
			loadingSurface);
	if (res->loadTexture == NULL) {
		SDL_FreeSurface(loadingSurface);
		loadWindowDestroy(res);
		printf("couldn't create load.bmp texture\n");
		return NULL;
	}

	SDL_FreeSurface(loadingSurface);

	/*back "BUTTON" INFORMATION*/
	loadingSurface = SDL_LoadBMP("images/back.bmp");
	if (loadingSurface == NULL) {
		loadWindowDestroy(res);
		printf("couldn't create back.bmp surface\n");
		return NULL;
	}
	res->backTexture = SDL_CreateTextureFromSurface(res->LoadRenderer,
			loadingSurface);
	if (res->backTexture == NULL) {
		SDL_FreeSurface(loadingSurface);
		loadWindowDestroy(res);
		printf("couldn't create back.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);

	loadingSurface = SDL_LoadBMP("images/startBG.bmp");
	if (loadingSurface == NULL) {
		loadWindowDestroy(res);
		printf("couldn't create startBG.bmp surface\n");
		return NULL;
	}
	res->startBGTexture = SDL_CreateTextureFromSurface(res->LoadRenderer,
			loadingSurface);
	if (res->startBGTexture == NULL) {
		SDL_FreeSurface(loadingSurface);
		loadWindowDestroy(res);
		printf("couldn't create startBG.bmp texture\n");
		return NULL;
	}
	SDL_FreeSurface(loadingSurface);
	return res;
}

void loadWindowDestroy(LoadWin* src) {
	if (!src) {
		return;
	}
	if (src->slot1Texture != NULL) {
		SDL_DestroyTexture(src->slot1Texture);
	}
	if (src->slot2Texture != NULL) {
		SDL_DestroyTexture(src->slot2Texture);
	}
	if (src->slot3Texture != NULL) {
		SDL_DestroyTexture(src->slot3Texture);
	}
	if (src->slot4Texture != NULL) {
		SDL_DestroyTexture(src->slot4Texture);
	}
	if (src->slot5Texture != NULL) {
		SDL_DestroyTexture(src->slot5Texture);
	}
	if (src->backTexture != NULL) {
		SDL_DestroyTexture(src->backTexture);
	}
	if (src->loadTexture != NULL) {
		SDL_DestroyTexture(src->loadTexture);
	}
	if (src->startBGTexture != NULL) {
		SDL_DestroyTexture(src->startBGTexture);
	}
	if (src->LoadRenderer != NULL) {
		SDL_DestroyRenderer(src->LoadRenderer);
	}
	if (src->LoadWindow != NULL) {
		SDL_DestroyWindow(src->LoadWindow);
	}
	free(src);
}

void loadWindowDraw(LoadWin* src) {
	if (src == NULL) {
		return;
	}
	//TODO CHANGE COORDINATES SO IT LOOKS NICE
	SDL_RenderClear(src->LoadRenderer);
	SDL_Rect backR = { .x = 180, .y = 160, .h = 100, .w = 100 };//matches click
	SDL_Rect loadR = { .x = 750, .y = 500, .h = 100, .w = 100 };
	SDL_Rect startBGR = { .x = 0, .y = 0, .h = 674, .w = 900 };

	SDL_RenderCopy(src->LoadRenderer, src->startBGTexture, NULL, &startBGR);
	SDL_RenderCopy(src->LoadRenderer, src->backTexture, NULL, &backR);

	SDL_Rect slot1R = { .x = 20, .y = 50, .h = 100, .w = 100 };	//matches click
	SDL_Rect slot2R = { .x = 130, .y = 50, .h = 100, .w = 100 };//matches click
	SDL_Rect slot3R = { .x = 240, .y = 50, .h = 100, .w = 100 };//matches click
	SDL_Rect slot4R = { .x = 350, .y = 50, .h = 100, .w = 100 };//matches click
	SDL_Rect slot5R = { .x = 460, .y = 50, .h = 100, .w = 100 };
	switch (src->numSlotsShown) {
	case 5://no break because it's opposite order
		SDL_RenderCopy(src->LoadRenderer, src->slot5Texture, NULL, &slot5R);
	case 4:
		SDL_RenderCopy(src->LoadRenderer, src->slot4Texture, NULL, &slot4R);
	case 3:
		SDL_RenderCopy(src->LoadRenderer, src->slot3Texture, NULL, &slot3R);
	case 2:
		SDL_RenderCopy(src->LoadRenderer, src->slot2Texture, NULL, &slot2R);
	case 1:
		SDL_RenderCopy(src->LoadRenderer, src->slot1Texture, NULL, &slot1R);
		break;
	}
	if (src->chosenSlot > 0)
		SDL_RenderCopy(src->LoadRenderer, src->loadTexture, NULL, &loadR);

	SDL_RenderPresent(src->LoadRenderer);
}
/*TODO need to complete*/
LOAD_EVENT loadWindowHandleEvent(LoadWin* src, SDL_Event* event) {
	if (!event) {
		return LOAD_INVALID_ARGUMENT;
	}
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		if (isClickOnBackFromLoad(event->button.x, event->button.y)) {
			src->backPressed = 1;
			src->chosenSlot = 0;
			return LOAD_BACK;
		}
		if (src->chosenSlot > 0) {
			if (isClickOnLoadFromLoad(event->button.x, event->button.y)) {
				return LOAD_PRESSED;
			}
		}
		src->chosenSlot = isClickOnSlot(event->button.x, event->button.y);
		if (src->chosenSlot > 0) {
			switch (src->numSlotsShown) {
			case 5:
				if (src->chosenSlot == 5)
					return LOAD_SLOT5;
			case 4:
				if (src->chosenSlot == 4)
					return LOAD_SLOT4;
			case 3:
				if (src->chosenSlot == 3)
					return LOAD_SLOT3;
			case 2:
				if (src->chosenSlot == 2)
					return LOAD_SLOT2;
			case 1:
				if (src->chosenSlot == 1)
					return LOAD_SLOT1;
			default:
				break;
			}
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE)
			return LOAD_EXIT;
		break;
	default:
		return LOAD_NONE;
	}
	return LOAD_NONE;
}

void loadWindowHide(LoadWin* src) {
	SDL_HideWindow(src->LoadWindow);
}

void loadWindowShow(LoadWin* src) {
	SDL_ShowWindow(src->LoadWindow);
}
