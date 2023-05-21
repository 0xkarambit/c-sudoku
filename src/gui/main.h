#include "sudoku.h"

void draw_sudoku(Sudoku *s);
void draw_entries();

// How the code Works rn
// I could nt find a way to visualise the recursive function while it is running
// so i took a RECORD and REPLAY approach instead
// I save the state of the function at each step and gave it to the gui as input/steps to go through