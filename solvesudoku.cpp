#include <string>
#include <iostream>
#include <algorithm>
#include "SudokuGrid.h"

#define N 9
#define UNASSIGNED 0;

using namespace std;

int numPencilsInRow(SudokuGrid& grid, int row, int n){
	int count = 0;
	for (int col = 0; col < N; col++){
		if (grid.isPencilSet(row, col, n) == true){
			count++;
		}
	}
	return count;
}

int numPencilsInColum(SudokuGrid& grid, int col, int n){
	int count = 0;
	for (int row = 0; row < N; row++){
		if (grid.isPencilSet(row, col, n) == true){
			count++;
		}
	}
	return count;
}

int numPencilsInBlock(SudokuGrid& grid, int row, int col, int n){
	int count = 0;
	int _row = row - (row % 3);
	int _col = col - (col % 3);
	for (int i=0; i < 3; i++){
		for (int j=0; j < 3; j++){
			if (grid.isPencilSet(_row+i, _col+j, n) == true)
				count++;
		}
	}
	return count;
}

/* Searches the grid to find an entry that is still unassigned. If
found, the reference parameters row, col will be set the location
that is unassigned, and true is returned. If no unassigned entries
remain, false is returned. */
bool findUnassignedLocation(SudokuGrid& grid, int& row, int& col){
	for (row = 0; row < N; row++){
		for (col = 0; col < N; col++){
			if (grid.number(row, col) == 0)
				return true;
		}
	}
	return false;
}

/* Returns a boolean which indicates whether any assigned entry
in the specified row matches the given number. */
bool usedInRow(SudokuGrid& grid, int row, int num, int col){
	for (int _col = 0; _col < N; _col++){
		if (_col != col){
			if (grid.number(row, _col) == num)
				return true;
		}
	}
	return false;
}

/* Returns a boolean which indicates whether any assigned entry
in the specified column matches the given number. */
bool usedInCol(SudokuGrid& grid, int col, int num, int row){
	for (int _row = 0; _row < N; _row++){
		if (_row != row){
			if (grid.number(_row, col) == num)
				return true;
		}
	}
	return false;
}

/* Returns a boolean which indicates whether any assigned entry
within the specified 3x3 box matches the given number. */
bool usedInBox(SudokuGrid& grid, int row, int col, int num){
	int boxStarRow = row - row % 3;
	int boxStarCol = col - col % 3;
	for (int i = boxStarRow; i < boxStarRow + 3; i++){
		for (int j = boxStarCol; j < boxStarCol + 3; j++){
			if (i != row || j != col){
				if (grid.number(i, j) == num)
					return true;
			}
		}
	}
	return false;
}

/* Returns a boolean which indicates whether it will be legal to assign
num to the given row,col location. */
bool conflictingNumber(SudokuGrid& grid, int row, int col, int num){
	/* check if 'num' is not already placed in current row,
	current column and current 3x3 box
	*/
	return (usedInRow(grid, row, num, col) ||
		usedInCol(grid, col, num, row) ||
		usedInBox(grid, row, col, num));
}

void autoPencil(SudokuGrid& grid){
	for (int r = 0; r < 9; r++){
		for (int c = 0; c < 9; c++){
			if (grid.number(r, c) == 0){
				grid.setAllPencils(r, c);
				for (int n = 1; n <= 9; n++){
					if (conflictingNumber(grid, r, c, n))
						grid.clearPencil(r, c, n);
				}
			}
		}
	}
}

void deduce(SudokuGrid& grid){
	bool changed;
	do{// repeat until no changes made
		autoPencil(grid);
		changed = false;
		for (int row = 0; row < 9; row++){
			for (int col = 0; col < 9; col++){
				for (int n = 1; n <= 9; n++){
					if (grid.isPencilSet(row, col, n) &&
						((numPencilsInRow(grid, row, n) == 1) ||
						(numPencilsInColum(grid, col, n) == 1) ||
						(numPencilsInBlock(grid, row, col, n) == 1))){

						grid.clearAllPencils(row, col);
						grid.setNumber(row, col, n);
						grid.setSolved(row, col);
						autoPencil(grid);
						changed = true;
						break;
					}
				}
			}
		}
	} while (changed);
}

bool solveSudoku(SudokuGrid& grid){
	int row, col;
	if (!findUnassignedLocation(grid, row, col))
		return true; // puzzle filled, solution found!

	for (int num = 1; num <= 9; num++){
		if (!conflictingNumber(grid, row, col, num)){
			grid.setNumber(row, col, num);// try next number
			// return, if succes
			if (solveSudoku(grid))
				return true;// solved
			grid.setNumber(row, col, 0);// not solved, clear number
		}

	}
	return false; // not solved, back track
}

int main(int argc, char *argv[]) {
//	string puzzle = "4.....8.5.3..........7......2.....6.....8.4......1.......6.3.7.5..2.....1.4......";
	string puzzle;
	cin >> puzzle;
	if (puzzle.length() != 9*9 ||
	!all_of(puzzle.begin(), puzzle.end(),
	[](char ch) {
	return ch == '.' || ('1' <= ch && ch <= '9');
	})) {
	cerr << "bogus puzzle!" << endl;
	exit(1);
	}
	
	SudokuGrid grid( puzzle );
	grid.printSudoku();
	autoPencil(grid);
	deduce(grid);
	grid.printSudoku();
	
	if (solveSudoku(grid))
		grid.printSudoku();
	else
		cout << "No possible solution\n" << endl;


	return 0;
}
