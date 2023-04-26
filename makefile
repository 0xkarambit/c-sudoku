FLAGS=-Wall -Wextra

NAME=game.exe

main: src/main.c src/sudoku.h src/sudoku.c
	gcc -g -o $(NAME) $(FLAGS) src/*.c src/*.h


# ya its a bad makefile

#OUTPUT_FILE="game.exe"

#run: **/*.c **/*.h   **/**/*.c **/**/*.h   *.c *.h
#   gcc -g -o $(OUTPUT_FILE) **/*.c **/**/*.c *.c -static -Os -std=c99 -Wall -Wextra -IC:\raylib\raylib\src -Iexternal -DPLATFORM_DESKTOP -lraylib -lopengl32 -lgdi32 -lwinmm

#https://www.raylib.com/examples.html
#https://gist.github.com/ArchieAtkinson/5c5758ad68d5cfd55d40430ca8e9b44d
#https://bedroomcoders.co.uk/sprite-sheets-with-raylib/
