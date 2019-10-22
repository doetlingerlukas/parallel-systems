#include <chrono>
#include <iostream>
#include <mpi.h>

using namespace std;
 
int main(int argc, char **argv) {

    unsigned long long samples = 1000000000;
    if (argc > 1) {
	    samples = strtoull(argv[1], nullptr, 10);
	}

    auto start_time = chrono::high_resolution_clock::now();

    int rank, numProcs; 
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    srand(time(NULL) + rank);
    unsigned long long count = 0;
    unsigned long long samples_per_rank = (samples + (numProcs - 1)) / numProcs;

    for (unsigned long long i = 0; i < samples_per_rank; ++i) {
        double x = ((double) rand()) / RAND_MAX;
        double y = ((double) rand()) / RAND_MAX;
        if ((x * x + y * y) <= 1.0) {
            count++;
        }
    }
    
    unsigned long long result = 0;
    MPI_Reduce(&count, &result, 1, MPI_UNSIGNED_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        double pi = ((double) result / (double) samples) * 4.0;

        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

        cout << "(Parallel) Estimate of pi with " << samples << " samples and " << numProcs << " ranks." << endl;
        cout << "π ≈ " << pi << endl;
        cout << "This took " << duration << " millisenconds." << endl;
    }
 
    MPI_Finalize();
    return EXIT_SUCCESS;
}