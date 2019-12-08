#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

#define attack(i, j) (hist[j] == i || abs(hist[j] - i) == col - j)

class Chessboard {
  int count = 0;
  bool print;
  int size;

  public:
    Chessboard(int n, bool verbose) {
      size = n;
      print = verbose;
    }

    bool attackable(int row, int j, int col, vector<int> hist) {
      return hist[j] == row || abs(hist[j] - row) == (col - j);
    }

    int getFirstSavePos(int row, int col, vector<int> hist){
      int j;
      for (j = 0; j < col && !attackable(row, j, col, hist); j++);
      return j;
    }

    void solve(int col, vector<int> hist) {
      // If last column is reached, print solution and return.
      if (col == size) {
        #pragma omp critical
        if (print) {
          printSolution(hist);
        }
        return;
      }
      
      for (int i = 0; i < size; i++) {
        // Get save position of queen in given row i.
        int j = getFirstSavePos(i, col, hist);

        // Only results after col need to be calculated.
        if (j < col) continue;
        
        // Set queen on position: row i, column col.
        hist[col] = i;

        #pragma omp task
        {
          solve(col + 1, hist);
        }
      }
    }

    void printSolution(vector<int> hist) {
      printf("\nNo. %d\n-----\n", ++count);
      for (int i = 0; i < size; i++, putchar('\n')) {
        for (int j = 0; j < size; j++) {
          j == hist[i] ? cout << "Q|" :  cout << "_|";
        }
      }
    }
};