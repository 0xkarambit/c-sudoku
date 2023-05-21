FLAGS=-Wall -Wextra

# RAYLIB=-IC:\raylib\raylib\src -LC:\raylib\raylib\src -lraylib

RAYLIB=-lraylib -DPLATFORM_DESKTOP -lopengl32 -lgdi32 -lwinmm -static

NAME=game.exe

OUT_DIR=build


main: src/main.c src/sudoku.h src/sudoku.c
	gcc -g -o $(OUT_DIR)/$(NAME) $(FLAGS) src/main.c src/sudoku.c

gui: src/gui/main.c lib/raygui.h src/gui/sudoku.c src/gui/sudoku.h
	gcc src/gui/main.c src/gui/sudoku.c -g -o $(OUT_DIR)/gui.exe $(RAYLIB) $(FLAGS)

clean:
	rm build/*
	rmdir build/

md:
	mkdir build/

all: md main gui