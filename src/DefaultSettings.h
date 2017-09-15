#ifndef DefaultSettings_H_
#define DefaultSettings_H_


typedef struct settings {

	int const BLACK;
	int const WHITE;
	char const EMPTY;
	///////////////////// game default settings

	int DIFF;
	int PLAYERS;
	int USER_COLOR; // white begins

}settings;

static settings setting = {0,1,'_',2,1,1};

void setDIFF(int newDiff);

void setPLAYERS(int newPlayers);

void setUSER_COLOR(int newCol);

int getDIFF();

int getUSER_COLOR();

int getPLAYERS();

int getWHITE();

int getBLACK();

int getEMPTY();

#endif// !DefaultSettings_H_
