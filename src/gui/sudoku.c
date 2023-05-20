#include "sudoku.h"

int BOXES[SIZE][2] = {
    // top
    {0, 0},
    {0, 3},
    {0, 6},
    // middle
    {3, 0},
    {3, 3},
    {3, 6},
    // bottom
    {6, 0},
    {6, 3},
    {6, 6}};

int *s_get_row_slice(Sudoku *s, int rowIndex)
{

  int *arr = calloc(SIZE, sizeof(int));
  memmove(arr, s->values[rowIndex], SIZE * sizeof(int));
  return arr;
}

int *s_get_col_slice(Sudoku *s, int colIndex)
{

  int *arr = calloc(SIZE, sizeof(int));
  int i = 0;
  // memmove(arr, s->values[rowIndex], SIZE * sizeof(int));

  for (int r = 0; r < SIZE; ++r)
  {
    arr[i++] = s->values[r][colIndex];
  }

  return arr;
}

int *s_get_box_slice(Sudoku *s, int rowIndex, int colIndex)
{

  if (rowIndex > 8 || rowIndex < 0 || colIndex > 8 || colIndex < 0)
  {
    // handle error
    return 0;
  }

  // Flooring row
  if (rowIndex >= 0 && rowIndex <= 2)
  {
    rowIndex = 0;
  }
  else if (rowIndex >= 3 && rowIndex <= 5)
  {
    rowIndex = 3;
  }
  else
  {
    // rowIndex >= 6 && rowIndex <= 8
    rowIndex = 6;
  }

  // Flooring column
  if (colIndex >= 0 && colIndex <= 2)
  {
    colIndex = 0;
  }
  else if (colIndex >= 3 && colIndex <= 5)
  {
    colIndex = 3;
  }
  else
  {
    // colIndex >= 6 && colIndex <= 8
    colIndex = 6;
  }

  // WE DONT ACTUALLY NEED THIS IF ELSE CODE ABOVE LOL

  int *arr = calloc(SIZE, sizeof(int));
  int i = 0;
  // memmove(arr, s->values[rowIndex], SIZE * sizeof(int));

  for (int r = rowIndex; r < rowIndex + 3; ++r)
  {
    for (int c = colIndex; c < colIndex + 3; ++c)
    {
      arr[i++] = s->values[r][c];
    }
  }

  return arr;
}

// we need to check the mini_box, row, col
// i think this will run quite slow because of a lot calloc use !
bool s_is_solved(Sudoku *s)
{
  // check all rows
  for (int bc = 0; bc < SIZE; ++bc)
  {
    int *row = s_get_row_slice(s, bc);
    if (s_is_array_valid(row) == false)
    {
      return false;
    }
    free(row);
  }

  // check all cols
  for (int bc = 0; bc < SIZE; ++bc)
  {
    int *col = s_get_col_slice(s, bc);
    if (s_is_array_valid(col) == false)
    {
      return false;
    }
    free(col);
  }

  // check all boxes
  for (int bc = 0; bc < SIZE; ++bc)
  {
    int r = BOXES[bc][0];
    int c = BOXES[bc][1];

    int *box = s_get_box_slice(s, r, c);

    // check if the array is valid
    if (s_is_array_valid(box) == false)
    {
      return false;
    }
    free(box);
  }

  return true;
}

bool s_is_array_valid(int *array)
{
  // O(n^2) But its only 9 by 9 so i guess its okay
  for (int i = 0; i < SIZE; ++i)
  {
#if 0
        if (array[i] < 1 && array[i] > 9) return false;
#endif
    for (int j = 0; j < SIZE; ++j)
    {
      if (i == j)
        continue;
      else
      {
        if (array[i] == array[j])
          return false;
      }
    }
  }

  return true;
}

void s_print(Sudoku *s)
{
  for (int r = 0; r < SIZE; ++r)
  {
    for (int c = 0; c < SIZE; ++c)
    {
      printf(" %d |", s->values[r][c]);
    }
    printf("\n");
  }
}

void s_print_stack(Sudoku s)
{
  // top border line
  printf(" ");
  for (int c = 0; c < SIZE; ++c)
    printf("---");
  printf("\n");

  for (int r = 0; r < SIZE; ++r)
  {
    // for (int c = 0; c < SIZE; ++c) printf("----");
    // printf("\n");

    for (int c = 0; c < SIZE; ++c)
    {
      // printf(" %d |", s.values[r][c]);
      printf(" %d ", s.values[r][c]);

      if (c == 2 || c == 5)
        printf("|");
    }

    printf("\n");

    if (r == 2 || r == 5)
    {
      printf(" ");
      for (int c = 0; c < SIZE; ++c)
        printf("---");
      printf("\n");
    }
  }

  // bottom border line
  printf(" ");
  for (int c = 0; c < SIZE; ++c)
    printf("---");
  printf("\n");
}

Sudoku *s_alloc_sudoku(Sudoku sudoku)
{
  Sudoku *s = (Sudoku *)calloc(1, sizeof(Sudoku));

  memmove(s->values, sudoku.values, sizeof(int) * 9 * 9);

  return s;
}

// int row_c[9][9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 }; // 0 = null
// int col_c[9][9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 }; // 0 = null
// int grid_c[9][9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 }; // 0 = null

// [r][c][suggestions] = {1,2,3,4,0,0,0,0,0}
int cache[9][9][9] = {0};

void s_prep_cache(Sudoku *s, int r, int c)
{
  int num = s->values[r][c];
  if (num != 0)
    return;

  int *row = s_get_row_slice(s, r);
  int *col = s_get_col_slice(s, c);
  int *box = s_get_box_slice(s, r, c);

  // find the numbers which dont occur in any 3 of these arrays
  bool suggestions[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // max index = 9

  for (int i = 0; i < SIZE; ++i)
  {
    int row_num = row[i];
    int col_num = col[i];
    int box_num = box[i];
    // if (num == 0) continue;
    // if a number exists in any of row, col or box the value for its index will be set to 1.
    suggestions[row_num] = false;
    suggestions[col_num] = false;
    suggestions[box_num] = false;
  }

  int cache_counter = 0;
  for (int i = 1; i < 10; ++i)
  {
    // wait a sec i have made the guesses position oriented ..... [r,c] lmao
    // mem usage with r, c, b = sizeof(int) * 9 * 9 * 3
    // mem usage with pos = sizeof(int) * 9 * 9 * 9                hmmmm.....

    // but then we would have to make one for the Union of r, c, b.. so ig its equal
    if (suggestions[i] == true)
    {
      cache[r][c][cache_counter++] = i;
    }
  }

  free(row);
  free(col);
  free(box);
}

void s_prep_cache_all(Sudoku *s)
{
  for (int r = 0; r < SIZE; ++r)
  {
    for (int c = 0; c < SIZE; ++c)
    {
      s_prep_cache(s, r, c);
    }
    printf("\n");
  }
}

bool s_solve(Sudoku *s)
{
  // Using Recursive Backtracking.
  // possible numbers for sudoku cache per col, row and subgrid (box)
  return s_solve_rec(s, 0, 0);
}

// --------------------------------
// GUI STUFF

ENTRY_LIST *ledger;
ENTRY_LIST *head;

void addEntry(EVENT event, int pos[2], int suggestions[9])
{
  ENTRY *e = calloc(sizeof(ENTRY), 1);

  if (event == PUSH)
  {
    int *sug = calloc(sizeof(int) * 9, 1);
    memmove(sug, suggestions, sizeof(int) * 9);

    e->event = event;
    e->pos[0] = pos[0];
    e->pos[1] = pos[1];
    e->suggestions = sug;
  }
  else
  {
    e->event = event;
    e->pos[0] = pos[0];
    e->pos[1] = pos[1];
    // e->suggestions = sug;
  }

  ENTRY_LIST *l = calloc(sizeof(ENTRY_LIST), 1);
  l->curr = e;
  l->next = NULL;

  ledger->next = l;

  ledger = l;
}

void deb_entries()
{
  while (ledger->next != NULL)
  {
    ENTRY *e = ledger->curr;
    printf("POS: [%d, %d] %s\n", e->pos[0], e->pos[1], (e->event == PUSH) ? "PUSH" : "POP");
    if (e->event == PUSH)
    {
      for (int i = 0; i < 9; i++)
      {
        printf("%d ", e->suggestions[i]);
      }
    }
    printf("\n\n");
    ledger = ledger->next;
  }
}

// void free_list(ENTRY_LIST *list)
void free_list()
{
  while (ledger->next != NULL)
  {
    ENTRY_LIST *next = ledger->next;
    free(ledger->curr->suggestions);
    free(ledger->curr);
    free(ledger);
    ledger = next;
  }
}

bool s_solve_rec(Sudoku *s, int r, int c)
{
  if (c == 9)
  {
    c = 0;
    ++r;
    if (r == 9)
      return true;
  }

  if (s->values[r][c] == 0)
  {
    int cc = 0; // cache_counter

    s_prep_cache(s, r, c);
    bool solved = false;
    addEntry(PUSH, (int[2]){r, c}, cache[r][c]);

    while (solved == false)
    {
      int suggestion = cache[r][c][cc++];

      if (suggestion == 0)
      {
        s->values[r][c] = 0;
        memset(cache[r][c], 0, sizeof(int) * 9);
        addEntry(POP, (int[2]){r, c}, cache[r][c]);
        return false;
      };

      s->values[r][c] = suggestion;

      solved = s_solve_rec(s, r, c + 1);
    }
    // we would essentially run the loop till we keep getting false..
    // exit condition -> exhaust all options. -> return false.
    //                   solved = true.
    return true;
  }
  else
  {
    return s_solve_rec(s, r, c + 1);
  }
}
