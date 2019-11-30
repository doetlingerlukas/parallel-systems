#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <time.h>
#include <thread>
#include <omp.h>

using namespace std;


int main(int argc, char **argv) {

    unsigned int seed;

    auto samples = 1000000000;
	if (argc > 1) {
        samples = strtol(argv[1], NULL, 10);
    }

    auto start_time = chrono::high_resolution_clock::now();

    long count = 0;
    double x, y;
    #pragma omp parallel private(seed, x, y) reduction(+:count) 
    {
        seed = time(NULL) * omp_get_thread_num();
        #pragma omp for
        for (int i = 0; i <= samples; i++) {
            x = (double)rand_r(&seed) / RAND_MAX;
            y = (double)rand_r(&seed) / RAND_MAX;
            if (x*x + y*y <= 1) count++;
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


// https://stackoverflow.com/questions/19489806/why-does-calculation-with-openmp-take-100x-more-time-than-with-a-single-thread/19535787#19535787