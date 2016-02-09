#ifndef SUDOKUGRID_H
#define SUDOKUGRID_H

#include <array>
#include <bitset>
#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;

class SudokuGrid {
private:
	// private class instance vars go here
	struct Cell{
		int val;
		bool fixed;
		bool solved;
		bool pencilSet;
		std::bitset<9> pencil;
		Cell() :val{ -1 }, fixed{ false }, solved{ false }, pencilSet{ false }{}
	};

	std::array<std::array<Cell, 9>, 9> grid;

public:
	// implement the following
	//SudokuGrid();
	//~SudokuGrid();
	SudokuGrid(std::string s){
		int i = 0;
		for (int row = 0; row < 9; row++){
			for (int col = 0; col < 9; col++){
				char ch = s[i];
				if (ch != '.'){
					grid[row][col].val = atoi(&ch);
					grid[row][col].fixed = true;
					grid[row][col].solved = true;
				}
				else{
					grid[row][col].val = 0;
				}
				i++;
			}
		}
	}

	int number(int row, int col) const {
		return grid[row][col].val;
	}

	void setNumber(int row, int col, int number) {
		grid[row][col].val = number;
	}

	bool isFixed(int row, int col) const {
		return grid[row][col].fixed;
	}

	bool isSolved(int row, int col) const {
		return grid[row][col].solved;
	}

	void setSolved(int row, int col) {
		grid[row][col].solved = true;
	}

	bool isPencilSet(int row, int col, int n) const {
		if (grid[row][col].pencil.test(n - 1) == true)
			return true;
		else
			return false;
	}

	bool anyPencilsSet(int row, int col) const {
		return grid[row][col].pencil.any();
	}

	void setPencil(int row, int col, int n) {
		grid[row][col].pencil.set(n - 1);
	}
	void setAllPencils(int row, int col) {
		grid[row][col].pencil.set();
	}

	void clearPencil(int row, int col, int n) {
		grid[row][col].pencil.reset(n - 1);
	}

	void clearAllPencils(int row, int col) {
		grid[row][col].pencil.reset();
	}

	void printSudoku(){
		for (int row = 0; row < 9; row++){
			for (int col = 0; col < 9; col++){
				cout << grid[row][col].val << " ";
				if ((col % 3) == 2)
					cout << "| ";
			}
			cout << endl;
			if ((row % 3) == 2 && row < 8)
				cout << "------+-------+--------" << endl;
		}
		cout << endl;
	}
};

#endif // SUDOKUGRID_H
