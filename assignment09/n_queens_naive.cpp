#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <chrono>
#include<algorithm>

using namespace std;

#define attack(i, j) (hist[j] == i || abs(hist[j] - i) == col - j)

int sol_count = 0;
bool print = false;

void printSolution(int n, vector<int> hist){
	++sol_count;
	if (!print) return;
	printf("\nNo. %d\n-----\n", sol_count);
	for (int i = 0; i < n ; i++, putchar('\n')){
		for (int j = 0; j < n; j++){
			j == hist[i] ? cout << "Q|" :  cout << "_|";
		}
	}
}


int isValid(int n, vector<int> hist){
	for(int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			if (i == j) continue;
			if (hist[i] == hist[j]+abs(i-j) || hist[i] == hist[j]-abs(i-j)){
				return 0;	
			}
		}
	}
    return 1;
}

void solve(int n, vector<int> hist){
	for (int i = 0; i < n; i++){
		hist[i] = i;
	}
	do{
		if (isValid(n, hist))
			printSolution(n, hist);
	} while(next_permutation(hist.begin(),hist.end()));
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

	solve(n, hist);

	cout << endl << sol_count << " solutions found" << endl;

	// Measure elapsed time.
	auto end_time = chrono::high_resolution_clock::now();
	auto duration = chrono::duration<double>(end_time - start_time).count();
	cout << endl;
	cout << "This took " << duration << " seconds." << endl;
}