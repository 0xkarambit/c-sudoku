#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "../lib/raygui.h"

#define and &&
#define or ||
#define TITLE "Sudoku Solver"
#include "sudoku.h"

#define DELAY 500
#define BOXSIDE 50
#define MAX 9

Sudoku temp = {
		.values = {
				{0, 7, 0, 0, 0, 0, 0, 4, 3},
				{0, 4, 0, 0, 0, 9, 6, 1, 0},
				{8, 0, 0, 6, 3, 4, 9, 0, 0},
				{0, 9, 4, 0, 5, 2, 0, 0, 0},
				{3, 5, 8, 4, 6, 0, 0, 2, 0},
				{0, 0, 0, 8, 0, 0, 5, 3, 0},
				{0, 8, 0, 0, 7, 0, 0, 9, 1},
				{9, 0, 2, 1, 0, 0, 0, 0, 5},
				{0, 0, 7, 0, 4, 0, 8, 0, 2}}};

Vector2 center_coor()
{
}

void draw_sudoku(Sudoku *s)
{

	const int global_offset = 100;
	//offset = center - (9 * BOXSIDE/2);

	const Color PRIMARY = RED;
	const Color SECONDARY = RAYWHITE;
	const int offset = 11;
	const int xoffset = 15;

	for (int r = 0; r < 9; ++r)
	{
		for (int c = 0; c < 9; ++c)
		{
			int val = s->values[r][c];

			if (!val)
				continue;

			char number[2] = {
				0x30 + val,
				0x0
			};

			DrawRectangle(
				global_offset + xoffset + (r * BOXSIDE),
				global_offset + offset + (c * BOXSIDE),
				BOXSIDE,
				BOXSIDE,
				SECONDARY
			);

			DrawText(
				number,
				global_offset + xoffset + (r * BOXSIDE),
				global_offset + offset + (c * BOXSIDE),
				32,
				PRIMARY
			);
		}
	}

	// drawing grid
	// horizontally ->
	for (int r = 0; r < 10; ++r)
	{
		DrawLine(
			global_offset + 0,
			global_offset + r * BOXSIDE,
			global_offset + (10 - 1) * BOXSIDE,
			global_offset + r * BOXSIDE,
			PRIMARY
		);
	}
	// vertically ^
	for (int c = 0; c < 10; ++c)
	{
		DrawLine(
			global_offset + c * BOXSIDE,
			global_offset + 0,
			global_offset + c * BOXSIDE,
			global_offset + (10 - 1) * BOXSIDE,
			PRIMARY
		);
	}
}

int main(void)
{

	const int screenWidth = 800;
	const int screenHeight = 600;

	// SetConfigFlags(FLAG_WINDOW_UNDECORATED);
	InitWindow(screenWidth, screenHeight, TITLE);

	SetWindowPosition(100, 100);

	while (!WindowShouldClose())
	{

		BeginDrawing();

		ClearBackground(RAYWHITE);
		draw_sudoku(&temp);

		EndDrawing();
	}

	return 0;
}