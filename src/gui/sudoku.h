
#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 9

#define and &&
#define or ||

typedef struct sudoku_s
{
  // Numbers to be Placed as they are, no reordering.
  int values[SIZE][SIZE];
} Sudoku;

// functions

Sudoku *s_alloc_sudoku(Sudoku sudoku);

void s_print(Sudoku *s);
void s_print_stack(Sudoku s);

int *s_get_row_slice(Sudoku *s, int rowIndex);
int *s_get_col_slice(Sudoku *s, int colIndex);
int *s_get_box_slice(Sudoku *s, int rowIndex, int colIndex);

bool s_is_array_valid(int *array);
bool s_is_solved(Sudoku *s);

void s_prep_cache(Sudoku *s, int r, int c);
void s_prep_cache_all(Sudoku *s);

bool s_solve(Sudoku *s);
bool s_solve_rec(Sudoku *s, int r, int c);

// ----------------------------------------
// GUI stuff

typedef enum event
{
  PUSH,
  POP
} EVENT;

typedef struct entry_s
{
  EVENT event;
  int pos[2];
  int *suggestions;
} ENTRY;

struct entry_list_s;
typedef struct entry_list_s
{
  ENTRY *curr;
  struct entry_list_s *next;
} ENTRY_LIST;

void free_list();
void deb_entries();
void addEntry(EVENT event, int pos[2], int suggestions[9]);