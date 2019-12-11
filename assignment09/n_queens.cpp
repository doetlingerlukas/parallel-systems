#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

#define attack(i, j) (hist[j] == i || abs(hist[j] - i) == col - j)

int count = 0;
bool print = false;

void printSolution(int n, vector<int> hist){
	++count;
	if (!print)	return;
	printf("\nNo. %d\n-----\n", count);
	for (int i = 0; i < n; i++, putchar('\n')){
		for (int j = 0; j < n; j++){
			j == hist[i] ? cout << "Q|" :  cout << "_|";
		}
	}
}

void solve(int n, int col, vector<int> hist){

	if (col == n) { // if last column reached print solution and return
        printSolution(n, hist);
		return;
	}
    
	for (int i = 0; i < n; i++) {
		
        int j;
        for (j = 0; j < col && !attack(i, j); j++); // get save position of queen in given row i

		if (j < col) continue; // only results after col needs to be calculated
 
		hist[col] = i; // set queen on position: row i, column col
		solve(n, col + 1, hist); //recursive call with next column
	}
}
 
int main(int argc, char **argv)
{
	auto start_time = chrono::high_resolution_clock::now();

	int n = 13;
	
	if (argc > 1) {
		n = strtol(argv[1], nullptr, 10);
		if (argc > 2) {
			string verbose = argv[2];
			print = verbose == "--verbose";
		}
  	}

	vector<int> hist(n, 0);

	solve(n, 0, hist);

	cout << endl << count << " solutions found" << endl;

	// Measure elapsed time.
	auto end_time = chrono::high_resolution_clock::now();
	auto duration = chrono::duration<double>(end_time - start_time).count();
	cout << endl;
	cout << "This took " << duration << " seconds." << endl;
}