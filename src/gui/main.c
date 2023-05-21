#include <raylib.h>

#include "sudoku.h"
#include "main.h"

#define TITLE "Sudoku Solver"

const int screenWidth = 1920;
const int screenHeight = 1080;

#define BOXSIDE 50
#define MAX 9

#define number(n) (char)(n + 0x30)

const Color PRIMARY = RED;
const Color SECONDARY = SKYBLUE;

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

extern ENTRY_LIST *ledger; // linkedlist that holds all ENTRIES.
extern ENTRY_LIST *head;

void init_ledger()
{
	ledger = (ENTRY_LIST *)calloc(sizeof(ENTRY_LIST), 1);
	ledger->curr = calloc(sizeof(ENTRY), 1);
	ledger->curr->event = POP;
	ledger->curr->pos[0] = 1000;
	ledger->curr->pos[1] = 1000;
	ledger->curr->suggestions = calloc(sizeof(int) * 9, 1);
	int sug[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	memmove(ledger->curr->suggestions, sug, sizeof(int) * 9);
}

// REPLAY MACHINE LOGIC
bool FINISHED = false;

typedef struct ram_entry_s
{
	ENTRY *entry;
	int index; // to keep track of the current suggestion being followed
} RAM_ENTRY;

RAM_ENTRY RAM[81] = {0};
int RC = -1;

//  Rendering Functions

void draw_sudoku(Sudoku *s)
{

	const int global_offset = 100;
	// offset = center - (9 * BOXSIDE/2);

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
					0x0};

			DrawRectangle(
					global_offset + (c * BOXSIDE),
					global_offset + (r * BOXSIDE),
					BOXSIDE,
					BOXSIDE,
					SECONDARY);

			DrawText(
					number,
					global_offset + offset + (c * BOXSIDE),
					global_offset + xoffset + (r * BOXSIDE),
					32,
					PRIMARY);
		}
	}

	// highlights the position currectly being computed
#if 1
	if (RC > -1 && !FINISHED)
	{
		int x = RAM[RC].entry->pos[0];
		int y = RAM[RC].entry->pos[1];

		DrawRectangle(
				global_offset + (y * BOXSIDE),
				global_offset + (x * BOXSIDE),
				BOXSIDE,
				BOXSIDE,
				BLUE);
	}
#endif
	// drawing grid
	// horizontally ->
	for (int r = 0; r < 10; ++r)
	{
		DrawLine(
				global_offset + 0,
				global_offset + r * BOXSIDE,
				global_offset + (10 - 1) * BOXSIDE,
				global_offset + r * BOXSIDE,
				PRIMARY);
	}
	// vertically ^
	for (int c = 0; c < 10; ++c)
	{
		DrawLine(
				global_offset + c * BOXSIDE,
				global_offset + 0,
				global_offset + c * BOXSIDE,
				global_offset + (10 - 1) * BOXSIDE,
				PRIMARY);
	}
}

void draw_entries()
{
	Vector2 offset = {300, 100};
	int padding = 45;

	int y = offset.y;
	int x = offset.x;

	for (int i = 0; i <= RC; i++)
	{
		y += padding;

		if (i % 15 == 0)
		{
			x += 400;
			y = offset.y;
		}
		// if (i % 30 == 0)
		// 	offset.x += 400;

		// y = y % 500;

		ENTRY *e = RAM[i].entry;
		int index = RAM[i].index;

		// TODO: make a macros for this lol

		/*
		 *https://stackoverflow.com/questions/29528732/gdb-print-all-values-in-char-array
		 https://blogs.oracle.com/linux/post/8-gdb-tricks-you-should-know
		 https://duckduckgo.com/?q=viewing+structs+in+gdb&t=newext&atb=v325-1&ia=web
		*/

		char string[] = {
				'[', number(e->pos[0]), ',', ' ', number(e->pos[1]), ']', ' ', '-', '>', ' ',
				number(e->suggestions[0]),
				// ',', ' ',
				' ',
				number(e->suggestions[1]),
				// ',', ' ',
				' ',
				number(e->suggestions[2]),
				// ',', ' ',
				' ',
				number(e->suggestions[3]),
				// ',', ' ',
				' ',
				number(e->suggestions[4]),
				// ',', ' ',
				' ',
				number(e->suggestions[5]),
				// ',', ' ',
				' ',
				number(e->suggestions[6]),
				// ',', ' ',
				' ',
				number(e->suggestions[7]),
				// ',', ' ',
				' ',
				number(e->suggestions[8])};

		DrawText(string, x, y, 24, PRIMARY);
		// 10 * 10 -- for 10 chars before the actual suggestions numbers start
		// index * 24 to draw the '^' pointer at the correct number's location.
		// although its better to just highlight the selected number using a different color
		DrawText("^", x + (10 * 10) + (index * 20), y + 20, 38, BLUE);
		// DrawTextEx(font, string, (Vector2){x, y}, 24, spacing, PRIMARY);
	}
}

void render()
{
	ClearBackground(RAYWHITE);
	draw_sudoku(&temp);
	draw_entries();
}

// Update function

void update()
{
	if (FINISHED)
		return;

	// PUSH and POP entries...
	ENTRY *e = ledger->curr;
	// printf("RC: %d\n", RC);
	// printf("event: %s\n", (e->event == PUSH) ? "PUSH" : "POP");

	if (e->event == PUSH)
	{
		RC += 1;
		RAM[RC].entry = e;
		RAM[RC].index = 0;
		int r = e->pos[0];
		int c = e->pos[1];

		// updating current suggestion in the sudoku being rendered onscreen
		temp.values[r][c] = e->suggestions[0];
	}

	else if (e->event == POP)
	{
		// RAM[RC].entry = NULL;
		RC = RC - 1;

		// moving to next suggestion of previous ENTRY / STATE FRAME
		int index = ++(RAM[RC].index);

		// to update in the sudoku to render.
		int r = RAM[RC].entry->pos[0];
		int c = RAM[RC].entry->pos[1];

		// updating suggestion.
		temp.values[r][c] = RAM[RC].entry->suggestions[index];
	}

	// All STEPS has been replayed
	if (ledger->next == NULL)
	{
		FINISHED = true;
	}
	ledger = ledger->next;
}

int main(void)
{

	init_ledger();

	head = ledger; // coz `ledger` would get moved to the tail eventually due to s_solve_rec etc..

	Sudoku *t = s_alloc_sudoku(temp);
	bool solved = s_solve_rec(t, 0, 0);
	// if (!solved) printf("COULDNT SOLVE SUDOKU...\n");

	ledger = head->next; // first entry if meaningless so starting the REPLAY of the RECORDED STEPS/STATE from the 2nd entry

	// -------------- GUI

	SetTargetFPS(60);
	InitWindow(screenWidth, screenHeight, TITLE);

	SetWindowPosition(0, 0);
	int frame_count = 0;

	while (!WindowShouldClose())
	{
		frame_count++;
		if (frame_count % (10) == 0)
			update();

		BeginDrawing();

		render();
		EndDrawing();
	}

	ledger = head;
	free_ledger();
	return 0;
}