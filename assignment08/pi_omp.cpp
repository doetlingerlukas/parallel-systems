#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <time.h>
#include <thread>

using namespace std;

long samplesInsideCircle(const int chunkSize)
{
    long count = 0;
    for (auto i=0; i <= chunkSize; i++){
        double x = ((double) rand() / RAND_MAX);
        double y = ((double) rand() / RAND_MAX);
        if ((x * x + y * y) <= 1) {
            count++;
        }
    }

    return count;
}

int main(int argc, char **argv) {

    auto samples = 10000000;
	if (argc > 1) {
        samples = strtol(argv[1], NULL, 10);
    }

    auto start_time = chrono::high_resolution_clock::now();

    srand(time(NULL)); 

    int numChunks = 8;
    int chunk = samples / numChunks;
    long count = 0;

    #pragma omp parallel for shared(numChunks, chunk) reduction(+:count)
    for (int i = 0; i < numChunks; i++)
    {
        count += samplesInsideCircle(chunk);
    }

    double pi = (4.0 * count) / samples; 


    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    
    cout << "\rπ ≈ " << pi << endl;
    cout << "was calculated with " << samples << " samples!" << endl;
    cout << "This took " << duration << " millisenconds." << endl;

	return EXIT_SUCCESS;
}


// http://jakascorner.com/blog/2016/05/omp-monte-carlo-pi.html