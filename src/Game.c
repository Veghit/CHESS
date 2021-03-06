#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SetCommand.h"
#include <limits.h>

#ifndef CONSTS
const int WHITE = 1;
const int BLACK = 0;
#endif
/*@param g- current game
@ret - true if there's no move that can be done anymore--> game is over
false, otherwise*/
bool currentLost(Game * g) {
	int moves[64];
	int row;
	int col;
	for (row = 0; row < 8; row++)
		for (col = 0; col < 8; col++) {
			if (isColor(g->gameBoard[row][col], g->currentPlayer)) {
				int i = getValidMoves(g, row * 8 + col, moves, 0);
				int j;
				for (j = 0; j < i; j++) {
					Game * g2 = cloneGame(g);
					if (-1 != makeMove(g2, row * 8 + col, moves[j])) {
						deleteGame(g2);
						return false;
					}
					deleteGame(g2);
				}
			}
		}
	return true;
}

/**
 @param g- the current game
 @ret- white if the white king is checked
 black if the black king is checked
 -1 if the game isn't checked
 */
int isGameChecked(Game* g) {
	int a = 0;
	int king = -1; // king index
	//if the current player is white
	if (g->currentPlayer == WHITE) {
		for (a = 0; a < 64; a++) {
			// we find the location of the white king
			if (g->gameBoard[a / 8][a % 8] == 'k') {
				king = a;
				break;
			}
		}
	} else {
		for (a = 0; a < 64; a++) {
			if (g->gameBoard[a / 8][a % 8] == 'K') {
				king = a;
				break;
			}
		}
	}

	if (isThreatened(g, king))
		//if one of the possible moves is in the same index as the king's,
		//then the king is checked
		return g->currentPlayer;
	return -1;
}
/*
@param g- the game
deletes the game by using free, since it's memory was allocated
returns nothing*/
void deleteGame(Game* g) {
	if (g != NULL)
		free(g);
	g = NULL;
}

/**
 @param c- the character of the piece
 @param color- the color of the piece
 @ret- true if the character and color of the piece match
 --> capital letter as a sign for black and so is the color of the piece
 or little letter as a sign for white, and so it the color if the piece
 false otherwise

 */
bool isColor(char c, int color) {
	return ((isWhite(c) && (color == WHITE)) || (isBlack(c) && (color == BLACK)));
}

/**
 @param c- character of the piece
 @ret- true if it is a capital letter
 that shows it is black, as the sign for a black character is a capital letter.
 false otherwise.

 */

bool isBlack(char c) {
	return (c >= 'A') && (c <= 'Z');
}

int getColor(char c) {
	if (isWhite(c))
		return WHITE;
	else if (isBlack(c))
		return BLACK;
	else
		return -1;
}

/**
 @param c- character of the piece
 @ret- true if it is an english letter, which isn't a capital
 that shows the piece is white, as is the sign for a white piece is a none-capital
 false otherwise

 */
bool isWhite(char c) {
	return (c >= 'a') && (c <= 'z');
}

/**
 @param g- the current game
 @ret- true if there are no more valid moves for the current player, for any piece
 false otherwise

 */
bool isGameTied(Game* g) {
	int a = 0;
	int moves[64];
	// all pieces have no moves
	for (a = 0; a < 64; a++) {
		if (canCastle(g, a))
			return false;
		// white piece has no moves
		if (g->currentPlayer == WHITE) {
			if (isWhite(g->gameBoard[a / 8][a % 8])) {
				int i = getValidMoves(g, a, moves, 0);

				int j;
				for (j = 0; j < i; j++) {
					Game* g2 = cloneGame(g);
					int end = moves[j];
					g2->gameBoard[end / 8][end % 8] =
							g2->gameBoard[a / 8][a % 8];
					g2->gameBoard[a / 8][a % 8] = '_';
					if (isGameChecked(g2) != WHITE) {
						deleteGame(g2);
						return false;
					}
					deleteGame(g2);
				}
			}
		} else {
			if (isBlack(g->gameBoard[a / 8][a % 8])) {
				int i = getValidMoves(g, a, moves, 0);
				int j;
				for (j = 0; j < i; j++) {
					Game* g2 = cloneGame(g);
					int end = moves[j];
					g2->gameBoard[end / 8][end % 8] =
							g2->gameBoard[a / 8][a % 8];
					g2->gameBoard[a / 8][a % 8] = '_';
					if (isGameChecked(g2) != BLACK) {
						deleteGame(g2);
						return false;
					}
					deleteGame(g2);
				}
			}
		}
	}
	return true;
}

/**
 @param g- the current game
 @param moveStr- regularly an empty string in the length of 1024
 @ret- GAME_COMMAND of the move that was made, if made correctly,
 otherwise, returns a GAME_COMMAND type according to the validity of the move that was to be made

 */
//TODO too long
GAME_COMMAND twoPlayersGame(Game* g, char* moveStr) {
	GameCommand move;
	bool validMove = false;
	Game * tempClone = cloneGame(g);
	if (tempClone == NULL) {
		return GAME_INVALID_LINE;
	}
	char * lastMove = (char*) calloc(100, sizeof(char));
	if (lastMove == NULL) {
		deleteGame(tempClone);
		return GAME_INVALID_LINE;
	}
	//parses the game command string (aka moveStr) into a proper gameCommand var
	if ((g->PLAYERS == 1) && (g->USER_COLOR != g->currentPlayer)) {
		move = pcMove(g);
		print_pcMove(g, move);
	} else {
		if (g->currentPlayer == BLACK) {
			printf("black player - enter your move:\n");
		} else {
			printf("white player - enter your move:\n");
		}
		//Receives the desirable move by the user
		if (fgets(moveStr, 1024, stdin) == 0) {
			free(lastMove);
			deleteGame(tempClone);
			return GAME_QUIT;
		}
		move = game_parse(moveStr);
	}if ((move.cmd == GAME_SAVE) && move.validArg) {
		xml_save(g, move.arg3);
	}if ((move.cmd == GAME_MOVE) || (move.cmd == GAME_CASTLE) || move.cmd == GAME_GET_MOVES) {
		if (move.validArg) {
			int row1 = move.arg1 / 8;
			int col1 = move.arg1 % 8;
			int row2 = move.arg2 / 8;
			int color = g->currentPlayer;
			char piece = g->gameBoard[row1][col1];
			if ((color == WHITE && isWhite(piece))
					|| ((color == BLACK) && isBlack(piece))) {
				if (move.cmd == GAME_CASTLE) {
					if ((piece != 'r') && (piece != 'R')) {
						printf("Wrong position for a rook\n");
						move.cmd = GAME_INVALID_LINE;
					}if (canCastle(g, move.arg1) == false) {
						printf("Illegal castling move\n");
						move.cmd = GAME_INVALID_LINE;
					} else {
						castle(g, move.arg1);
						validMove = true;
					}
				}if (move.cmd == GAME_MOVE) {
					if (-1 == makeMove(g, move.arg1, move.arg2)) {
						printf("Illegal move\n");
						move.cmd = GAME_INVALID_LINE;
						validMove = false;
					} else {
						if (((piece == 'M') && (row2 == 0))
								|| ((piece == 'm') && (row2 == 7)))
							pawnPromotion(g, move.arg2);
						validMove = true;
					}
				}if (move.cmd == GAME_GET_MOVES) {
					int arg = move.arg1;
					printValidMoves(g, arg);
				}
			} else {
				printf("The specified position does not contain your piece\n");
				move.cmd = GAME_INVALID_LINE;
			}
		} else {
			printf("Invalid position on the board\n");
			move.cmd = GAME_INVALID_LINE;
		}
	}
	if (validMove) {
		handle_undo(g, tempClone, move, lastMove);
	} else {
		free(lastMove);
		deleteGame(tempClone);
	}
	return move.cmd;
}
/*@param g- game
@param move- game command of most recent move
prints the last move of pc*/
void print_pcMove(Game* g, GameCommand move) {
	char c = g->gameBoard[move.arg1 / 8][move.arg1 % 8];
	char * name;
	if ((c == 'k') || (c == 'K'))
		name = "king";
	if ((c == 'q') || (c == 'Q'))
		name = "queen";
	if ((c == 'n') || (c == 'N'))
		name = "knight";
	if ((c == 'b') || (c == 'B'))
		name = "bishop";
	if ((c == 'r') || (c == 'R'))
		name = "rook";
	if ((c == 'm') || (c == 'M'))
		name = "pawn";
	printf("Computer: move %s at <%d,%c> to <%d,%c>\n", name,
			1 + (move.arg1 / 8), move.arg1 % 8 + 'A', 1 + (move.arg2 / 8),
			move.arg2 % 8 + 'A');
}
/*@param g- game
@param tempClone- a temporary clone of the game
@param move- the most recent move made (before the undo)
@param lastMove-an empty string, originally.
updates the lastMoves array, in order to update the game to the state before the move*/
void handle_undo(Game * g, Game * tempClone, GameCommand move, char* lastMove) {
	if (move.cmd == GAME_MOVE) {
		if (g->currentPlayer == WHITE)
			sprintf(lastMove,
					"Undo move for player white : <%d,%c> -> <%d,%c>\n",
					1 + move.arg2 / 8, 'A' + move.arg2 % 8, 1 + move.arg1 / 8,
					'A' + move.arg1 % 8);
		else
			sprintf(lastMove,
					"Undo move for player black : <%d,%c> -> <%d,%c>\n",
					1 + move.arg2 / 8, 'A' + move.arg2 % 8, 1 + move.arg1 / 8,
					'A' + move.arg1 % 8);
	} else {
		if (g->currentPlayer == WHITE)
			sprintf(lastMove, "Undo castle for player white : <%d,%c>\n",
					move.arg1 / 8, 'A' + move.arg1 % 8);
		else
			sprintf(lastMove, "Undo castle for player black : <%d,%c>\n",
					move.arg1 / 8, 'A' + move.arg1 % 8);
	}
	free(lastMoves[5]);
	lastMoves[5] = lastMoves[4];
	lastMoves[4] = lastMoves[3];
	lastMoves[3] = lastMoves[2];
	lastMoves[2] = lastMoves[1];
	lastMoves[1] = lastMoves[0];
	lastMoves[0] = lastMove;
	g->currentPlayer = 1 - g->currentPlayer;
	if (g->currentPlayer == BLACK) {
		if (g->saves < 3)
			g->saves += 1;
		deleteGame(lastGames[2]);
		lastGames[2] = lastGames[1];
		lastGames[1] = lastGames[0];
		lastGames[0] = tempClone;
	} else {
		deleteGame(tempClone);
	}
}
/*@param g- the game
@param str- a string that contains the file name
saves the information of g in the file in xml style*/
void xml_save(Game * g, char * str) {

	FILE *fp;
	fp = fopen(str, "w");
	if (fp == NULL)
		printf("File cannot be created or modified\n");
	else {
		fputs("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n", fp);
		fputs("<game>\n", fp);
		fputs("\t<current_turn>", fp);
		fputc('0' + g->currentPlayer, fp);
		fputs("</current_turn>\n", fp);
		fputs("\t<game_mode>", fp);
		fputc('0' + g->PLAYERS, fp);
		fputs("</game_mode>\n", fp);
		if (g->PLAYERS == 1) {
			fputs("\t<difficulty>", fp);
			fputc('0' + g->DIFF, fp);
			fputs("</difficulty>\n", fp);
			fputs("\t<user_color>", fp);
			fputc('0' + g->USER_COLOR, fp);
			fputs("</user_color>\n", fp);
		}
		fputs("\t<board>\n", fp);
		int i, j;
		for (i = 0; i < 8; i++) {
			fputs("\t\t<row_", fp);
			fputc('8' - i, fp);
			fputc('>', fp);
			for (j = 0; j < 8; j++) {
				fputc(g->gameBoard[7 - i][j], fp);
			}
			fputs("</row_", fp);
			fputc('8' - i, fp);
			fputs(">\n", fp);
		}
		fputs("\t</board>\n", fp);
		fputs("</game>\n", fp);
		fclose(fp);
	}
}
/*@param g- game
@param arg- the place of the pawn that's being promoted in the game
promotes the pawn into any chosen game piece*/
void pawnPromotion(Game * g, int arg) {
	char * string = (char*) calloc(1024, sizeof(char));
	if (string == NULL) {
		printf("ERROR in memory allocation.");
		return;
	}
	char w1[30];

	char piece = 0;
	char *s1 = "queen";
	char *s2 = "rook";
	char *s3 = "knight";
	char *s4 = "bishop";
	char *s5 = "pawn";
	while (true) {
		printf(
				"Pawn promotion- please replace the pawn by queen, rook, knight, bishop or pawn:\n");
		while (fgets(string, 1024, stdin) == 0) {
			fgets(string, 1024, stdin);
		}
		for (int a = 0; a < 30; a++) {
			w1[a] = 0;
		}
		getNextWord(string, w1);
		free(string);
		if (equalStrings(w1, s1)) {
			piece = 'q';
		} else if (equalStrings(w1, s2)) {
			piece = 'r';
		} else if (equalStrings(w1, s3)) {
			piece = 'n';
		} else if (equalStrings(w1, s4)) {
			piece = 'b';
		} else if (equalStrings(w1, s5)) {
			piece = 'm';
		} else {
			printf("Invalid Type\n");
		}
		if (piece != 0) {
			if (g->currentPlayer == BLACK)
				piece += 'A' - 'a';
			g->gameBoard[arg / 8][arg % 8] = piece;
			return;
		}

	}

}

/**
 @param g- current game
 @ret- void
 resets the game's settings and board to a new game's default settings
 */
void resetGame(Game * g) {
	g->currentPlayer = WHITE;
	g->blackLeftCastle = true;
	g->blackRightCastle = true;
	g->whiteLeftCastle = true;
	g->whiteRightCastle = true;
	g->saves = 0;
	g->DIFF = 2;
	g->PLAYERS = 1;
	g->USER_COLOR = WHITE;
	char arr[64] = { 'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r', 'm', 'm', 'm', 'm',
			'm', 'm', 'm', 'm', '_', '_', '_', '_', '_', '_', '_', '_', '_',
			'_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_',
			'_', '_', '_', '_', '_', '_', '_', '_', '_', '_', 'M', 'M', 'M',
			'M', 'M', 'M', 'M', 'M', 'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R', };
	int x = 0;
	for (x = 0; x <= 63; x++) {
		g->gameBoard[x / 8][x % 8] = arr[x];
	}
}
/*
@param g- game
@param arg- the place of the rook
returns true if the rook can castle the king
false otherwise*/
bool canCastle(Game* g, int arg) {
	if ((arg == 0) || (arg == 7) || (arg == 56) || (arg == 63)) {
	} else
		return false;

	if (isGameChecked(g) == g->currentPlayer)
		return -1;
	if ((arg == 0) && g->whiteLeftCastle) {
		if ((g->gameBoard[0][0] == 'r') && (g->gameBoard[0][1] == '_')
				&& (g->gameBoard[0][2] == '_') && (g->gameBoard[0][3] == '_')
				&& (g->gameBoard[0][4] == 'k')) {
			Game* g1 = cloneGame(g);
			Game* g2 = cloneGame(g);
			g1->gameBoard[0][0] = '_';
			g2->gameBoard[0][0] = '_';
			g1->gameBoard[0][3] = 'k';
			g2->gameBoard[0][2] = 'k';
			bool game_checked = (isGameChecked(g1) != WHITE)
					&& (isGameChecked(g2) != WHITE);
			deleteGame(g1);
			deleteGame(g2);
			return game_checked;
		}
	}
	if ((arg == 7) && g->whiteRightCastle) {
		if ((g->gameBoard[0][7] == 'r') && (g->gameBoard[0][6] == '_')
				&& (g->gameBoard[0][5] == '_') && (g->gameBoard[0][4] == 'k')) {
			Game* g1 = cloneGame(g);
			Game* g2 = cloneGame(g);
			g1->gameBoard[0][7] = '_';
			g2->gameBoard[0][7] = '_';
			g1->gameBoard[0][6] = 'k';
			g2->gameBoard[0][5] = 'k';
			bool game_checked = (isGameChecked(g1) != WHITE)
					&& (isGameChecked(g2) != WHITE);
			deleteGame(g1);
			deleteGame(g2);
			return game_checked;
		}
	}
	if ((arg == 56) && g->blackLeftCastle) {
		if ((g->gameBoard[7][0] == 'R') && (g->gameBoard[7][1] == '_')
				&& (g->gameBoard[7][2] == '_') && (g->gameBoard[7][3] == '_')
				&& (g->gameBoard[7][4] == 'K')) {
			Game* g1 = cloneGame(g);
			Game* g2 = cloneGame(g);
			g1->gameBoard[7][0] = '_';
			g2->gameBoard[7][0] = '_';
			g1->gameBoard[7][2] = 'K';
			g2->gameBoard[7][3] = 'K';
			bool game_checked = (isGameChecked(g1) != BLACK)
					&& (isGameChecked(g2) != BLACK);
			deleteGame(g1);
			deleteGame(g2);
			return game_checked;
		}
	}
	if ((arg == 63) && g->blackRightCastle) {
		if ((g->gameBoard[7][7] == 'R') && (g->gameBoard[7][6] == '_')
				&& (g->gameBoard[7][5] == '_') && (g->gameBoard[7][4] == 'K')) {
			Game* g1 = cloneGame(g);
			Game* g2 = cloneGame(g);
			g1->gameBoard[7][0] = '_';
			g2->gameBoard[7][0] = '_';
			g1->gameBoard[7][1] = 'K';
			g2->gameBoard[7][2] = 'K';
			bool game_checked = (isGameChecked(g1) != BLACK)
					&& (isGameChecked(g2) != BLACK);
			deleteGame(g1);
			deleteGame(g2);
			return game_checked;
		}
	}
	return false;
}

int castle(Game * g, int arg) {
	if (arg == 0) {
		g->gameBoard[0][0] = '_';
		g->gameBoard[0][2] = 'k';
		g->gameBoard[0][3] = 'r';
		g->gameBoard[0][4] = '_';
		return 4;
	}
	if (arg == 7) {
		g->gameBoard[0][7] = '_';
		g->gameBoard[0][6] = 'k';
		g->gameBoard[0][5] = 'r';
		g->gameBoard[0][4] = '_';
		return 4;
	}
	if (arg == 56) {
		g->gameBoard[7][0] = '_';
		g->gameBoard[7][2] = 'K';
		g->gameBoard[7][3] = 'R';
		g->gameBoard[7][4] = '_';
		return 60;
	}

	if (arg == 63) {
		g->gameBoard[7][7] = '_';
		g->gameBoard[7][6] = 'K';
		g->gameBoard[7][5] = 'R';
		g->gameBoard[7][4] = '_';
		return 60;
	}
	return -1;
}

/**
 @param g- the current game
 @param arg1- a location of a piece in the game
 @param arg2- a goal location of the piece in the game
 @ret- -1 if the move is illegal
 otherwise, moves piece to desired location

 */
int makeMove(Game * g, int arg1, int arg2) {
	Game * g2 = cloneGame(g);
	int row1 = arg1 / 8;
	int row2 = arg2 / 8;
	int col1 = arg1 % 8;
	int col2 = arg2 % 8;
	char piece = g2->gameBoard[row1][col1];
	int validMoves[64];
	int j;
	for (j = 0; j < 64; j++)
		validMoves[j] = -1;
	getValidMoves(g2, arg1, validMoves, 0);
	for (j = 0; j < 64; j++) {
		if (validMoves[j] == arg2) {
			break;
		}
	}
	if ((j < 64) && (validMoves[j] == arg2)) {
		if ((arg1 == 0) || (arg2 == 0))
			g2->whiteLeftCastle = false;
		if ((arg1 == 7) || (arg2 == 7))
			g2->whiteRightCastle = false;
		if ((arg1 == 63) || (arg2 == 63))
			g2->blackRightCastle = false;
		if ((arg1 == 56) || (arg2 == 56))
			g2->blackLeftCastle = false;
		if ((arg1 == 4) || (arg2 == 4)) {
			g2->whiteLeftCastle = false;
			g2->whiteRightCastle = false;
		}
		if ((arg1 == 60) || (arg2 == 60)) {
			g2->blackLeftCastle = false;
			g2->blackRightCastle = false;
		}
		g2->gameBoard[row1][col1] = '_';
		g2->gameBoard[row2][col2] = piece;
		if (g2->currentPlayer == isGameChecked(g2)) {
			deleteGame(g2);
			return -1;
		}

	} else {
		deleteGame(g2);
		return -1;

	}
	g->gameBoard[row1][col1] = '_';
	g->gameBoard[row2][col2] = piece;
	deleteGame(g2);
	return 1;
}

/**
 @param g- current game
 @param arg- the piece's location on a flattened board
 @param moves- an existing array of possible moves, usually filled with other pieces possible moves or/and
 0's or -1's at the very least.
 @param i- the next empty space in the moves array
 @ret- the number of next valid moves for the piece(aka arg) in the game,
 in accordance with the piece's type obviously

 */
int getValidMoves(Game* g, int arg, int* moves, int i) {
	char piece = g->gameBoard[arg / 8][arg % 8];
	int numMoves = 0;
	int row = arg / 8;
	int col = arg % 8;
	switch (piece) {
	case 'M':
	case 'm':
		numMoves = appendPawnMoves(g, row, col, moves, i);
		break;
	case 'R':
	case 'r':
		numMoves = appendRookMoves(g, row, col, moves, i);
		break;
	case 'N':
	case 'n':
		numMoves = appendKnightMoves(g, row, col, moves, i);
		break;
	case 'k':
	case 'K':
		numMoves = appendKingMoves(g, row, col, moves, i);
		break;
	case 'Q':
	case 'q':
		numMoves = appendRookMoves(g, row, col, moves, i); //rook here intentionally queen is rook+bishop
		numMoves += appendBishopMoves(g, row, col, moves, numMoves + i); //
		break;
	case 'b':
	case 'B':
		numMoves = appendBishopMoves(g, row, col, moves, i);
		break;
	}
	return numMoves;
}

void printValidMoves(Game * g, int arg) {
	int* moves = (int*) calloc(64, sizeof(int));
	if (moves == NULL) {
		printf("ERROR in memory allocation.");
		return;
	}
	int i = getValidMoves(g, arg, moves, 0);
	int j;
	int k[64];
	for (j = 0; j < 64; j++)
		k[j] = 0;
	for (j = 0; j < i; j++) {
		int move = moves[j];
		Game * g2 = cloneGame(g);
		g2->gameBoard[move / 8][move % 8] = g2->gameBoard[arg / 8][arg % 8];
		g2->gameBoard[arg / 8][arg % 8] = '_';
		if (isGameChecked(g2) != g->currentPlayer)
			k[move] = 1;
		deleteGame(g2);
	}
	for (j = 0; j < 64; j++) {
		if (k[j]) {
			printf("<%d,%c>", 1 + (j / 8), 'A' + j % 8);
			if (g->DIFF < 3) {
				Game * g2 = cloneGame(g);
				g2->gameBoard[j / 8][j % 8] = g2->gameBoard[arg / 8][arg % 8];
				g2->gameBoard[arg / 8][arg % 8] = '_';
				g2->currentPlayer = 1 - g2->currentPlayer;
				if (isThreatened(g2, j)) {
					printf("*");
				}
				if (g->gameBoard[j / 8][j % 8] != '_') {
					printf("^");
				}
				deleteGame(g2);
			}
			printf("\n");
		}
	}
	if (canCastle(g, arg))
		printf("castle <%d,%c>", 1 + (arg / 8), 'A' + arg % 8);
	free(moves);
}

bool isThreatened(Game * g, int arg) {
	int a = 0;
	int moves[64];
	char piece = g->gameBoard[arg / 8][arg % 8];
	int c1 = getColor(piece);
	for (a = 0; a < 64; a++) {
		int c2 = getColor(g->gameBoard[a / 8][a % 8]);
		if ((c2 != -1) && (c1 != c2)) {
			int j = getValidMoves(g, a, moves, 0);
			for (int i = 0; i < j; i++) {
				if (moves[i] == arg)
					return true;
			}
		}
	}
	return false;
}

/**
 @param g- current game
 @param row- current row of the piece
 @param col- current column of the piece
 @param moves- an existing array of possible moves, usually filled with other pieces possible moves or/and
 0's or -1's at the very least.
 @param i- the next empty/ fillable location in the array
 @ret- number of possible moves for the pawn from it's current place

 */
int appendPawnMoves(Game *g, int row, int col, int* moves, int i) {
	int initialPlace = i;
	char color = BLACK;
	if (isWhite(g->gameBoard[row][col]))
		color = WHITE;
	if (color == WHITE) {
		// first white Pawn move
		if ((row == 1) && (g->gameBoard[2][col] == '_'))
			i = appendMoveIfEmpty(g, row + 2, col, moves, i);
		//regular white pawn move
		i = appendMoveIfEmpty(g, row + 1, col, moves, i);
		// white pawn right capture
		i = appendMoveIfConquerable(g, row + 1, col + 1, color, moves, i);
		//white pawn left capture
		i = appendMoveIfConquerable(g, row + 1, col - 1, color, moves, i);
	} else {
		// first black Pawn move
		if ((row == 6) && (g->gameBoard[5][col] == '_'))
			i = appendMoveIfEmpty(g, row - 2, col, moves, i);
		//regular black pawn move
		i = appendMoveIfEmpty(g, row - 1, col, moves, i);
		// black pawn right capture
		i = appendMoveIfConquerable(g, row - 1, col + 1, color, moves, i);
		// black pawn left capture
		i = appendMoveIfConquerable(g, row - 1, col - 1, color, moves, i);

	}
	return i - initialPlace;
}

/**
 @param g- current game
 @param row- current row of the piece
 @param col- current column of the piece
 @param moves- an existing array of possible moves, usually filled with other pieces possible moves or/and
 0's or -1's at the very least.
 @param i- the next empty/ fillable location in the array
 @ret- number of possible moves for the rook from it's current place

 */
int appendRookMoves(Game *g, int row, int col, int* moves, int i) {
	char color = BLACK;
	int initialPlace = i;
	if (isWhite(g->gameBoard[row][col]))
		color = WHITE;
//rook UP
	int j = 1;
	while (appendMoveIfEmpty(g, row + j, col, moves, i) > i) {
		i++;
		j++;
	}
	i = appendMoveIfConquerable(g, row + j, col, color, moves, i);
//Down
	j = 1;
	while (appendMoveIfEmpty(g, row - j, col, moves, i) > i) {
		i++;
		j++;
	}
	i = appendMoveIfConquerable(g, row - j, col, color, moves, i);
//right
	j = 1;
	while (appendMoveIfEmpty(g, row, col + j, moves, i) > i) {
		i++;
		j++;
	}
	i = appendMoveIfConquerable(g, row, col + j, color, moves, i);
// left
	j = 1;
	while (appendMoveIfEmpty(g, row, col - j, moves, i) > i) {
		i++;
		j++;
	}
	i = appendMoveIfConquerable(g, row, col - j, color, moves, i);
	return i - initialPlace;
}

/**
 @param g- current game
 @param row- current row of the piece
 @param col- current column of the piece
 @param moves- an existing array of possible moves, usually filled with other pieces possible moves or/and
 0's or -1's at the very least.
 @param i- the next empty/ fillable location in the array
 @ret- number of possible moves for the bishop from it's current place

 */
int appendBishopMoves(Game *g, int row, int col, int* moves, int i) {
	int initialPlace = i;
	char color = BLACK;
	if (isWhite(g->gameBoard[row][col]))
		color = WHITE;
//up right
	int j = 1;
	while (appendMoveIfEmpty(g, row + j, col + j, moves, i) > i) {
		i++;
		j++;
	}
	i = appendMoveIfConquerable(g, row + j, col + j, color, moves, i);
//down left
	j = 1;
	while (appendMoveIfEmpty(g, row - j, col - j, moves, i) > i) {
		i++;
		j++;
	}
	i = appendMoveIfConquerable(g, row - j, col - j, color, moves, i);
//up left
	j = 1;
	while (appendMoveIfEmpty(g, row + j, col - j, moves, i) > i) {
		i++;
		j++;
	}
	i = appendMoveIfConquerable(g, row + j, col - j, color, moves, i);
//down right
	j = 1;
	while (appendMoveIfEmpty(g, row - j, col + j, moves, i) > i) {
		i++;
		j++;
	}
	i = appendMoveIfConquerable(g, row - j, col + j, color, moves, i);
	return i - initialPlace;
}

/**
 @param g- current game
 @param row- current row of the piece
 @param col- current column of the piece
 @param moves- an existing array of possible moves, usually filled with other pieces possible moves or/and
 0's or -1's at the very least.
 @param i- the next empty/ fillable location in the array
 @ret- number of possible moves for the knight from it's current place

 */
int appendKnightMoves(Game *g, int row, int col, int* moves, int i) {
	char color = BLACK;
	int initialPlace = i;
	if (isWhite(g->gameBoard[row][col]))
		color = WHITE;
	i = appendMoveIfEmptyOrConquerable(g, row + 2, col - 1, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row + 2, col + 1, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row + 1, col + 2, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row - 1, col + 2, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row - 2, col - 1, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row - 2, col + 1, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row + 1, col - 2, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row - 1, col - 2, color, moves, i);
	return i - initialPlace;
}

/**
 @param g- current game
 @param row- current row of the piece
 @param col- current column of the piece
 @param moves- an existing array of possible moves, usually filled with other pieces possible moves or/and
 0's or -1's at the very least.
 @param i- the next empty/ fillable location in the array
 @ret- number of possible moves for the king from it's current place
 */
int appendKingMoves(Game *g, int row, int col, int* moves, int i) {
	char color = BLACK;
	int initialPlace = i;
	if (isWhite(g->gameBoard[row][col]))
		color = WHITE;
	i = appendMoveIfEmptyOrConquerable(g, row + 1, col, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row + 1, col + 1, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row, col + 1, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row - 1, col + 1, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row - 1, col, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row - 1, col - 1, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row, col - 1, color, moves, i);
	i = appendMoveIfEmptyOrConquerable(g, row + 1, col - 1, color, moves, i);
	return i - initialPlace;
}

/**
 @param g- the current game
 @param row- a certain row in the game's board
 @param col-a certain column in the game's board
 @param moves- an array which contains all "previous" moves
 @param i- the current empty spot of the moves array
 @ret- if the spot of the game's current board (before the move was made)
 is empty, updates the moves array,
 returns the next empty spot on the moves array
 otherwise, returns the current empty spot (which is just i)
 */
int appendMoveIfEmpty(Game* g, int row, int col, int * moves, int i) {
	if (isEmpty(g, row, col)) {
		moves[i] = 8 * row + col;

		return i + 1;
	} else
		return i;
}

/**
 @param g- the current game
 @param row- a certain row in the game's board
 @param col-a certain column in the game's board
 @param moves- an array which contains all "previous" moves
 @param i- the current empty spot of the moves array
 @ret- if the spot of the game's current board (before the move was made)
 is conquerable, updates the moves array,
 returns the next empty spot on the moves array
 otherwise, returns the current empty spot (which is just i)
 */
int appendMoveIfConquerable(Game* g, int row, int col, int color, int * moves,
		int i) {
	if (isConquerable(g, row, col, color)) {
		moves[i] = 8 * row + col;
		return i + 1;
	} else
		return i;
}

/**
 @param g- the current game
 @param row- a certain row in the game's board
 @param col- a certain column in the game's board
 @param moves- an array which contains all "previous" moves
 @param i- the current empty spot of the moves array
 @ret- if the spot of the game's current board (before the move was made)
 is empty or conquerable, makes a move and returns the next empty spot in the moves array
 otherwise, simply returns the current empty spot (since the move couldn't have been made)
 */
int appendMoveIfEmptyOrConquerable(Game* g, int row, int col, int color,
		int * moves, int i) {
	i = appendMoveIfEmpty(g, row, col, moves, i);
	i = appendMoveIfConquerable(g, row, col, color, moves, i);
	return i;
}

/**
 @param g- the current game
 @param row- a certain row in the game's board
 @param col- a certain column in the game's board

 @ret- true if in the current spot of the game's board there's no piece
 false otherwise

 */
bool isEmpty(Game* g, int row, int col) {
	if ((row < 8) && (row >= 0) && (col >= 0) && (col < 8)) {
		return g->gameBoard[row][col] == '_';
	} else
		return false;
}

/**
 @param g- the current game
 @param row- a certain row in the game's board
 @param col- a certain column in the game's board
 @param color-  the color of the piece that is supposed to make the move
 @ret- true if the specified spot of the current game's board is conquerable
 --> there's a different piece color there(than the one we received as param)
 false otherwise

 */
bool isConquerable(Game * g, int row, int col, int color) {
	if ((row < 8) && (row >= 0) && (col >= 0) && (col < 8)) {
		char curr = g->gameBoard[row][col];
		if (isEmpty(g, row, col))
			return false;
		return !isColor(curr, color);
	} else
		return false;
}

/**
 @param g- the game
 @param row- a certain row in the game's board
 @param col- a certain column in the game's board
 @param color- the color of the piece that is supposed to make the move
 @ret- true if the spot in the current game's board is conquerable/ empty
 and false otherwise.

 */
bool isEmptyOrConquerable(Game* g, int row, int col, int color) {
	return isEmpty(g, row, col) || isConquerable(g, row, col, color);
}

/**
 @param g- the game itself (and obviously it's state)
 @ret- void
 prints the game's state, as in, the board as it is.
 */
void printBoard(Game * g) {
	int i;
	int j;
	for (i = 1; i <= 8; i++) {
		printf("%d| ", 9 - i);
		for (j = 0; j < 8; j++)
			printf("%c ", g->gameBoard[8 - i][j]);
		printf("|\n");
	}
	printf("  -----------------\n");
	printf("   A B C D E F G H\n");
}
/**
 @param g- receives a game
 @ret- returns a game which has all the fields value of the g in it's fields
 */

Game * cloneGame(Game* g) {
	Game * g2 = (Game*) calloc(1, sizeof(Game));
	g2->DIFF = g->DIFF;
	g2->mode = g->mode;
	g2->PLAYERS = g->PLAYERS;
	g2->saves = g->saves;
	g2->USER_COLOR = g->USER_COLOR;
	g2->currentPlayer = g->currentPlayer;
	g2->blackLeftCastle = g->blackLeftCastle;
	g2->blackRightCastle = g->blackRightCastle;
	g2->whiteLeftCastle = g->whiteLeftCastle;
	g2->whiteRightCastle = g->whiteRightCastle;
	int i;
	for (i = 0; i < 64; i++)
		g2->gameBoard[i / 8][i % 8] = g->gameBoard[i / 8][i % 8];
	return g2;
}

GameCommand pcMove(Game* g) {
	return MinimaxSuggestMove(g);
}

GameCommand MinimaxSuggestMove(Game* g) {
	int maxDepth = g->DIFF;
	GameCommand move;
	if (maxDepth > 5) {
		if (g->mode == 'c')
			printf("wrong maxDepth");
		return move;
	}
	if (g == NULL || maxDepth <= 0) {
		printf("wrong param vals");
		return move;
	}
	Game * clone = cloneGame(g);
	if (clone == NULL) {
		printf("wrong -didnt manage to clone");
		return move;
	}

	int indexChosen[1];
	GameCommand * chosenMove = (GameCommand*) calloc(1, sizeof(GameCommand));
	if (chosenMove == NULL) {
		printf("ERROR in memory allocation.");
		GameCommand gc;
		gc.cmd = GAME_INVALID_LINE;
		return gc;
	}
	create_Tree(clone, maxDepth, 0, indexChosen, chosenMove, INT_MIN, INT_MAX);
	deleteGame(clone);
	GameCommand ret = *chosenMove;
	free(chosenMove);
	return ret;

}
int create_Tree(Game* curGame, int maxDepth, int curDepth, int indexChosen[], GameCommand * chosenMove, int alpha, int beta) {
	//printf("\nDEPTH:%d\t", curDepth);
	Game* clone;
	int originalPlace = 0;
	int goalPlace = 0;
	if (curDepth == maxDepth) {
		int mark = calcLowest(curGame);
		return mark;
	}
	GameCommand * allPossibleMoves = (GameCommand*) calloc(432, sizeof(GameCommand));
	if (allPossibleMoves == NULL) {
		printf("ERROR in memory allocation.");
		return 0;
	}
	listAllMoves(curGame, allPossibleMoves);
	if (allPossibleMoves[0].validArg == false) {
		free(allPossibleMoves);
		return calcLowest(curGame);
	}

	int j;
	int tempScore;
	int tempMove = 0;
	int v = 0;
	if (curGame->currentPlayer == WHITE) {
		v = INT_MIN;
		for (j = 0; j < 432; j++) {
			if (allPossibleMoves[j].validArg == true) {
				//printf("v=%d A=%d B=%d\t", v, alpha, beta);
				clone = cloneGame(curGame);
				originalPlace = allPossibleMoves[j].arg1;
				goalPlace = allPossibleMoves[j].arg2;
				//printf("\n%d => %d", originalPlace, goalPlace);
				makeMove(clone, originalPlace, goalPlace);
				clone->currentPlayer = 1 - clone->currentPlayer;
				tempScore = create_Tree(clone, maxDepth, curDepth + 1,
						indexChosen, chosenMove, alpha, beta);
				deleteGame(clone);
				if (tempScore > v) {
					v = tempScore;
					tempMove = j;
				}
				if (v > alpha)
					alpha = v;
				if (beta <= alpha) {
					//printf("A=%d B=%d\t", alpha, beta);
					//printf("BETA\n");
					break;// beta cut-off
				}
			} else
				break;
		}
	} else {
		v = INT_MAX;
		for (j = 0; j < 432; j++) {
			if (allPossibleMoves[j].validArg == true) {
				//printf("v=%d A=%d B=%d\t", v, alpha, beta);
				clone = cloneGame(curGame);
				originalPlace = allPossibleMoves[j].arg1;
				goalPlace = allPossibleMoves[j].arg2;
				//printf("\n%d => %d", originalPlace, goalPlace);
				makeMove(clone, originalPlace, goalPlace);
				clone->currentPlayer = 1 - clone->currentPlayer;
				tempScore = create_Tree(clone, maxDepth, curDepth + 1, indexChosen, chosenMove, alpha, beta);
				deleteGame(clone);
				if (tempScore < v) {
					v = tempScore;
					tempMove = j;
				}
				if (v < beta)
					beta = v;
				if (beta <= alpha) {
					//printf("A=%d B=%d\t", alpha, beta);
					//printf("ALPHA\n");
					break;// alpha cut-off
				}
			} else
				break;
		}
	}
	*chosenMove = allPossibleMoves[tempMove];
	free(allPossibleMoves);
	//printf("\nCHOSEN: %d => %d SCORE:%d\n", allPossibleMoves[tempMove].arg1,
	//		allPossibleMoves[tempMove].arg2, v);
	return v;
}

//list all possoble moves for the current player. That is, legal moves not ending in check for himself.

void listAllMoves(Game* g, GameCommand allPossibleMoves[]) {
	int index = 0;
	int movesArr[432];
	int numMoves;
	for (int i = 0; i < 64; i++) {
		char piece = g->gameBoard[i / 8][i % 8];
		if ((piece != '_') && (getColor(piece) == g->currentPlayer)) {
			numMoves = getValidMoves(g, i, movesArr, 0);
			int j = 0;
			for (j = 0; j < numMoves; j++) {
				GameCommand move;
				move.cmd = GAME_MOVE;
				move.arg1 = i;
				move.arg2 = movesArr[j];
				move.validArg = true;
				Game* testGame = cloneGame(g); //need to test if move is legal
				if (-1 != makeMove(testGame, move.arg1, move.arg2)) {
					allPossibleMoves[index] = move;
					index++;
				}
				deleteGame(testGame);
			}
		}
	}
	GameCommand move;
	move.cmd = GAME_MOVE;
	move.arg1 = 0;
	move.arg2 = 0;
	move.validArg = false;
	allPossibleMoves[index] = move;
}

void printIntArr(int* arr, int len) {
	for (int i = 0; i < len; i++) {
		printf("%d ", arr[i]);
	}
}

void printCharArr(char* arr, int len) {
	for (int i = 0; i < len; i++) {
		printf("%c ", arr[i]);
	}
}

/**
 @param arr- receives an int array
 @param length- receives the length of the array
 @param jump- receives the repititive place in which the array's value should be different
 */
void initializeSpecial(int* arr, int length, int jump) {
	for (int i = 0; i < length; i++) {
		if ((i + 1) % jump == 0)
			arr[i] = -1;
		else
			arr[i] = 0;
	}
}

/**
 @pro- after a move was made && max level of recursion && the game can go on
 @param g- the game which's state needs to be calculated
 @ret- after the move was made : if the last move was by the white player:
 evaluation of white pieces on board - the evaluation of black pieces
 otherwise, the opposite

 */
int calcLowest(Game* g) {
	if ((isGameChecked(g) == g->currentPlayer) && (currentLost(g))) {
		if (g->currentPlayer == WHITE)
			return INT_MIN;
		else
			return INT_MAX;
	}
	if (isGameTied(g))
		return 0;
	int evalWHITE = 0;
	int evalBLACK = 0;
	char cur = 'c';

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cur = g->gameBoard[i][j];
			switch (cur) {
			case 'M':
				evalBLACK++;
				break;
			case 'N':
			case 'B':
				evalBLACK += 3;
				break;
			case 'R':
				evalBLACK += 5;
				break;
			case 'Q':
				evalBLACK += 9;
				break;
				//case 'K': there are always two kings in a legal game
				//	evalBLACK += 100;
				//	break;
			case 'm':
				evalWHITE++;
				break;
			case 'n':
			case 'b':
				evalWHITE += 3;
				break;
			case 'r':
				evalWHITE += 5;
				break;
			case 'q':
				evalWHITE += 9;
				break;
				//case 'k':
				//	evalWHITE += 100;
				//	break;
			}
		}

	}

	return evalWHITE - evalBLACK;
}

void restartGame(Game*g) {
	char board[64]= { 'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r', 'm', 'm', 'm', 'm',
		'm', 'm', 'm', 'm', '_', '_', '_', '_', '_', '_', '_', '_', '_',
		'_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_',
		'_', '_', '_', '_', '_', '_', '_', '_', '_', '_', 'M', 'M', 'M',
		'M', 'M', 'M', 'M', 'M', 'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R', };
	for (int x = 0; x <= 63; x++) {
		g->gameBoard[x / 8][x % 8] = board[x];
	}
	g->blackLeftCastle = true;
	g->blackRightCastle = true;
	g->whiteLeftCastle = true;
	g->whiteRightCastle = true;
	g->saves = 0;
	g->currentPlayer = WHITE;

}

