#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#include "sudoku.h"

int main()
{

  // https://stackoverflow.com/questions/30980759/when-should-i-pass-or-return-a-struct-by-value
  Sudoku s = {
    .values = {
      {1,2,3,4,5,6,7,8,9}
    }
  };

  s_print_stack(s);


  printf("Hello World");
  return 0;
}