#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <time.h>
#include <thread>
#include <math.h>
#include <omp.h>

using namespace std;


int main(int argc, char **argv) {

    int samples = pow (10.0, 7.0);
	if (argc > 1) {
        samples = strtol(argv[1], NULL, 10);
    }

    auto start_time = chrono::high_resolution_clock::now();

    long count = 0;
    #pragma omp parallel reduction(+:count) 
    {
        unsigned int seed = time(NULL) * omp_get_thread_num();
        #pragma omp for
        for (int i = 0; i <= samples; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;
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