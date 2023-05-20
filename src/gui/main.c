#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "../../lib/raygui.h"

#define and &&
#define or ||
#define TITLE "Sudoku Solver"
#include "sudoku.h"

#define DELAY 500
#define BOXSIDE 50
#define MAX 9

const Color PRIMARY = RED;
const Color SECONDARY = RAYWHITE;

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

// Vector2 center_coor() {}
extern ENTRY_LIST *ledger;
extern ENTRY_LIST *head;

void draw_sudoku(Sudoku *s);
void draw_entries();

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

// MACHINE LOGIC

typedef struct ram_entry_s
{
	ENTRY *entry;
	int index;
} RAM_ENTRY;

RAM_ENTRY RAM[81] = {0};
// int POSITIONS[81][2] = {0};
int RC = 0;

void update()
{
	// setup clock and FPS count for animation.

	if (ledger->next == NULL)
		return;

	// PUSH and POP entries...
	ENTRY *e = ledger->curr;
	printf("RC: %d\n", RC);
	printf("event: %s\n", (e->event == PUSH) ? "PUSH" : "POP");

	if (e->event == PUSH)
	{
		RAM[RC++].entry = e;
		int r = e->pos[0];
		int c = e->pos[1];

		temp.values[r][c] = e->suggestions[0];
		// RC++;
	}

	else if (e->event == POP)
	{
		// --RC; // -2 coz the index points to a free location by nature of ++;
		int prev_index = RC - 1;
		RAM[prev_index].index++;

		int index = RAM[prev_index].index;

		if (RAM[prev_index].entry->suggestions[index] == 0)
			RC = RC - 1;

		int r = RAM[prev_index].entry->pos[0];
		int c = RAM[prev_index].entry->pos[1];

		temp.values[r][c] = RAM[prev_index].entry->suggestions[index];
	}

	ledger = ledger->next;
}

void render()
{
	ClearBackground(RAYWHITE);
	draw_sudoku(&temp);
	draw_entries();
}

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
					global_offset + xoffset + (r * BOXSIDE),
					global_offset + offset + (c * BOXSIDE),
					BOXSIDE,
					BOXSIDE,
					SECONDARY);

			DrawText(
					number,
					global_offset + xoffset + (r * BOXSIDE),
					global_offset + offset + (c * BOXSIDE),
					32,
					PRIMARY);
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
	Vector2 offset = {800, 200};
	int padding = 50;

	for (int i = 0; i < RC; i++)
	{
		int x = offset.x;
		int y = offset.y + i * padding;

		ENTRY *e = RAM[i].entry;
		int index = RAM[i].index;

		char string[] = {
				'[', (char)(e->pos[0] + 0x30), ',', ' ', (char)(e->pos[1] + 0x30), ']', ' ', '-', '>', ' ',
				(char)(e->suggestions[0] + 0x30),
				',', ' ',
				(char)(e->suggestions[1] + 0x30),
				',', ' ',
				(char)(e->suggestions[2] + 0x30),
				',', ' ',
				(char)(e->suggestions[3] + 0x30),
				',', ' ',
				(char)(e->suggestions[4] + 0x30),
				',', ' ',
				(char)(e->suggestions[5] + 0x30),
				',', ' ',
				(char)(e->suggestions[6] + 0x30),
				',', ' ',
				(char)(e->suggestions[7] + 0x30),
				',', ' ',
				(char)(e->suggestions[8] + 0x30)};

		DrawText(string, x, y, 32, PRIMARY);
	}
}

int main(void)
{

	s_print_stack(temp);

	init_ledger();

	head = ledger;

	Sudoku *t = s_alloc_sudoku(temp);
	s_solve_rec(t, 0, 0);

	ledger = head; // lazy excuse....
	deb_entries();
	ledger = head->next; // lazy excuse....

	// -------------- GUI
	const int screenWidth = 1920;
	const int screenHeight = 1080;

	// SetConfigFlags(FLAG_WINDOW_UNDECORATED);
	InitWindow(screenWidth, screenHeight, TITLE);

	SetWindowPosition(100, 100);

	while (!WindowShouldClose())
	{

		update();

		BeginDrawing();

		render();
		EndDrawing();
	}

	free_list();
	return 0;
}