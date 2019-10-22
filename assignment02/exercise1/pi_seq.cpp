#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>

using namespace std;

int main(int argc, char **argv) {

    auto samples = 1000000000;
	if (argc > 1) {
        samples = strtol(argv[1], NULL, 10);
    }

    auto start_time = chrono::high_resolution_clock::now();

    srand(time(NULL)); 

    long count = 0;
    for (auto i=0; i <= samples; i++){
        double x = ((double) rand() / RAND_MAX);
        double y = ((double) rand() / RAND_MAX);
        if ((x * x + y * y) <= 1) {
            count++;
        }
    }

    double pi = (4.0 * count) / samples; 

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    
    cout << "\rπ ≈ " << pi << endl;
    cout << "was calculated with " << samples << " samples!" << endl;
    cout << "This took " << duration << " millisenconds." << endl;

	return EXIT_SUCCESS;
}
