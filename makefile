FLAGS=-Wall -Wextra

# RAYLIB=-IC:\raylib\raylib\src -LC:\raylib\raylib\src -lraylib

RAYLIB=-lraylib -DPLATFORM_DESKTOP -lopengl32 -lgdi32 -lwinmm -static

NAME=game.exe

OUT_DIR=build


main: src/main.c src/sudoku.h src/sudoku.c
	gcc -g -o $(OUT_DIR)/$(NAME) $(FLAGS) src/main.c src/sudoku.c

window: src/window.c src/sudoku.c src/sudoku.h
	gcc src/window.c src/sudoku.c  -o $(OUT_DIR)/window.exe $(RAYLIB) $(FLAGS)
# Ig the filenames should appear before some other args like -l

gui: src/gui.c lib/raygui.h
	gcc src/gui.c -o $(OUT_DIR)/gui.exe $(RAYLIB) $(FLAGS)

all: main window gui

clean:
	rmdir -f build/

# lol i should just make a create_build_dir Target and add it to others as a dependency..

# Todo Make notes on gcc commands + init.gdb and gcc options & c libs linking and makefiles...
# Todo check the type punning for (int array[9]) to (long long int) and the immediate struct construction expression syntax (struct name)({.values={}}) ?
# other c tricks too like returning a string using strdup function.

#  GCC
# https://www.rapidtables.com/code/linux/gcc/gcc-l.html
# -I header files (Include)
# -L linking files .ddl, .so or .lib, .a  } Must be (libraylib.a) {lib<libname>.a}
# -l library name.. to link to

# -c compile without linking !
# -S produce assembly
# -s strip !


# works
#  gcc window.c -o window.exe -IC:\raylib\raylib\src\raylib -LC:\raylib\raylib\src\raylib -lraylib -DPLATFORM_DESKTOP -lopengl32 -lgdi32 -lwinmm
#  gcc window.c -o window.exe -IC:\raylib\raylib\src\raylib -lraylib -DPLATFORM_DESKTOP -lopengl32 -lgdi32 -lwinmm
#  gcc window.c -o window.exe -lraylib -DPLATFORM_DESKTOP -lopengl32 -lgdi32 -lwinmm -static


#OUTPUT_FILE="game.exe"

#run: **/*.c **/*.h   **/**/*.c **/**/*.h   *.c *.h
#   gcc -g -o $(OUTPUT_FILE) **/*.c **/**/*.c *.c -static -Os -std=c99 -Wall -Wextra -IC:\raylib\raylib\src -Iexternal -DPLATFORM_DESKTOP -lraylib -lopengl32 -lgdi32 -lwinmm

#https://www.raylib.com/examples.html
#https://gist.github.com/ArchieAtkinson/5c5758ad68d5cfd55d40430ca8e9b44d
#https://bedroomcoders.co.uk/sprite-sheets-with-raylib/
