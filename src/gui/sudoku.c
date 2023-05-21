#include "sudoku.h"

// used to hold the "suggestions" -> possible values for each cell.
int cache[9][9][9] = {0};

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

// --------------------------------
// GUI STUFF

ENTRY_LIST *ledger;
ENTRY_LIST *head;

void addEntry(EVENT event, int pos[2], int suggestions[9])
{
  ENTRY *e = calloc(sizeof(ENTRY), 1);
  e->event = event;
  e->pos[0] = pos[0];
  e->pos[1] = pos[1];

  // suggestions are only valuable if its PUSH.
  if (event == PUSH)
  {
    int *sug = calloc(sizeof(int) * 9, 1);
    memmove(sug, suggestions, sizeof(int) * 9);
    e->suggestions = sug;
  }

  // adding ENTRY to LinkedList ledger and moving it forward.
  ENTRY_LIST *l = calloc(sizeof(ENTRY_LIST), 1);
  l->curr = e;
  l->next = NULL;

  ledger->next = l;

  ledger = l;
}

void deb_ledger()
{
  while (ledger->next != NULL)
  {
    ENTRY *e = ledger->curr;
    printf("POS: [%d, %d] %s\n", e->pos[0], e->pos[1], (e->event == PUSH) ? "PUSH" : "POP");
    if (e->event == PUSH)
    {
      for (int i = 0; i < 9; i++)
        printf("%d ", e->suggestions[i]);
    }
    printf("\n\n");
    ledger = ledger->next;
  }
}

void free_ledger()
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
    // moving to next row, (coz all column of this row are done)
    // return true if entire sudoku has been solved.
    if (++r == 9)
      return true;
  }

  // 0 val represents an empty cell.
  if (s->values[r][c] == 0)
  {
    int cc = 0; // cache_counter

    s_prep_cache(s, r, c);
    bool solved = false;
    addEntry(PUSH, (int[2]){r, c}, cache[r][c]);

    while (solved == false)
    {
      int suggestion = cache[r][c][cc++];

      // suggestion being 0 means we have run out of valid suggestions available for this cell.
      if (suggestion == 0)
      {
        s->values[r][c] = 0;
        memset(cache[r][c], 0, sizeof(int) * 9); // clear previous suggestions
        addEntry(POP, (int[2]){r, c}, cache[r][c]);
        return false;
      };

      // trying current suggestion and moving to next cell.
      s->values[r][c] = suggestion;
      solved = s_solve_rec(s, r, c + 1);
    }
    return true;
  }
  // moving to next cell if this cell is solved already.
  return s_solve_rec(s, r, c + 1);
}
