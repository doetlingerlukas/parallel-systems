#include "../shared/chessboard.hpp"

#include <chrono>

using namespace std;

int main(int argc, char **argv) {
	auto start_time = chrono::high_resolution_clock::now();

	int n = 13;
	bool print = false;
	
	if (argc > 1) {
    n = strtol(argv[1], nullptr, 10);
    if (argc > 2) {
      string verbose = argv[2];
      print = verbose == "--verbose";
    }
  }

	Chessboard chessboard = Chessboard(n, print);

	#pragma omp parallel
	{
		#pragma omp single
		chessboard.solve(0, vector<int>(n, 0));
	}

	// Measure elapsed time.
	auto end_time = chrono::high_resolution_clock::now();
	auto duration = chrono::duration<double>(end_time - start_time).count();
	cout << endl;
	cout << "This took " << duration << " seconds." << endl;
}