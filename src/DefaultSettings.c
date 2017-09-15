#include "DefaultSettings.h"

static settings * stg = &setting;

void setDIFF(int newDiff) {
	stg->DIFF = newDiff;
}

void setPLAYERS(int newPlayers) {
	stg->PLAYERS = newPlayers;
}

void setUSER_COLOR(int newCol) {
	stg->USER_COLOR = newCol;
}

int getDIFF() {
	return stg->DIFF;
}

int getUSER_COLOR() {
	return stg->USER_COLOR;
}

int getPLAYERS() {
	return stg->PLAYERS;
}

int getWHITE() {
	return stg->WHITE;
}

int getBLACK() {
	return stg->BLACK;
}

int getEMPTY() {
	return stg->EMPTY;
}