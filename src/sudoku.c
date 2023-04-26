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
  {6, 6}
};

int* s_get_row_slice(Sudoku* s, int rowIndex)
{

  int* arr = calloc(SIZE, sizeof(int));
  memmove(arr, s->values[rowIndex], SIZE * sizeof(int));
  return arr;
}

int* s_get_col_slice(Sudoku* s, int colIndex)
{

  int* arr = calloc(SIZE, sizeof(int));
  int i = 0;
  // memmove(arr, s->values[rowIndex], SIZE * sizeof(int));

  for (int r = 0; r < SIZE; ++r)
  {
    arr[i++] = s->values[r][colIndex];
  }

  return arr;
}

int* s_get_box_slice(Sudoku* s, int rowIndex, int colIndex)
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


  int* arr = calloc(SIZE, sizeof(int));
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
bool s_is_solved(Sudoku* s)
{
  // check all rows
  for (int bc = 0; bc < SIZE; ++bc)
  {
    int* row = s_get_row_slice(s, bc);
    if (s_is_array_valid(row) == false)
    {
      return false;
    }
    free(row);
  }

  // check all cols
  for (int bc = 0; bc < SIZE; ++bc)
  {
    int* col = s_get_col_slice(s, bc);
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

    int* box = s_get_box_slice(s, r, c);

    // check if the array is valid
    if (s_is_array_valid(box) == false)
    {
      return false;
    }
    free(box);
  }

  return true;

}

bool s_is_array_valid(int* array)
{
  // O(n^2) But its only 9 by 9 so i guess its okay
  for (int i = 0; i < SIZE; ++i)
  {
#if 0
        if (array[i] < 1 && array[i] > 9) return false;
#endif
    for (int j = 0; j < SIZE; ++j)
    {
      if (i == j) continue;
      else
      {
        if (array[i] == array[j]) return false;
      }
    }
  }

  return true;
}

void s_print(Sudoku* s)
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
  for (int r = 0; r < SIZE; ++r)
  {
    for (int c = 0; c < SIZE; ++c)
    {
      printf(" %d |", s.values[r][c]);
    }
    printf("\n");
  }
}
