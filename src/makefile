CC = gcc
COMMON_OBJS = GameCommand.o Game.o  SetCommand.o main.o
GUI_OBJS = ChessGameWin.o ChessGUIManager.o ChessMainWin.o ChessLoadWin.o
ALL_OBJS = $(COMMON_OBJS) $(GUI_OBJS)
EXEC = chessprog
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
COMP_FLAG = -std=c99 -Wall -Wextra -Werror -pedantic-errors -g
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main

all: $(EXEC)

$(EXEC): $(ALL_OBJS)
	$(CC) $(ALL_OBJS) $(SDL_LIB) -o $@

GameCommand.o: GameCommand.c SetCommand.h 
	$(CC) $(COMP_FLAG) -c $*.c
SetCommand.o: SetCommand.c SetCommand.h 
	$(CC) $(COMP_FLAG) -c $*.c
Game.o: Game.c SetCommand.h Game.h GameCommand.h
	$(CC) $(COMP_FLAG) -c $*.c

main.o: main.c graphics/ChessGUIManager.h SDL.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c

ChessGUIManager.o: ChessGUIManager.c ChessGUIManager.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessGameWin.o: ChessGameWin.c ChessGameWin.h ChessWindow.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessLoadWin.o: ChessLoadWin.c ChessLoadWin.h 
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessMainWin.o: ChessMainWin.c ChessMainWin.h 
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c

	
clean:
	rm -f $(ALL_OBJS) $(EXEC)
