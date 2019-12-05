#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

using namespace std;

#define attack(i, j) (hist[j] == i || abs(hist[j] - i) == col - j)

int count = 0;

void printSolution(int n, int *hist){
        printf("\nNo. %d\n-----\n", ++count);
		for (int i = 0; i < n; i++, putchar('\n')){
			for (int j = 0; j < n; j++){
                j == hist[i] ? cout << "Q|" :  cout << "_|";
            }
        }
}

int getFirstSavePos(int row, int col, int * hist){
    int j;
    for (j = 0; j < col && !attack(row, j); j++);
    return j;
}

void solve(int n, int col, int *hist)
{
	if (col == n) { // if last column reached print solution and return
        printSolution(n, hist);
		return;
	}
    
	for (int i = 0; i < n; i++) {
        int j = getFirstSavePos(i, col, hist); // get save position of queen in given row i

		if (j < col) continue; // only results after col needs to be calculated
 
		hist[col] = i; // set queen on position: row i, column col
		solve(n, col + 1, hist); //recursive call with next column
	}
}
 
int main(int n, char **argv)
{
	if (n <= 1 || (n = atoi(argv[1])) <= 0) n = 8;

	int hist[n] = {0};

	solve(n, 0, hist);
}