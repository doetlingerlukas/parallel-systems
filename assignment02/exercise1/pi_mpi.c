#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
 
int main(int argc, char **argv) {

    long samples = 1000000000;
    if (argc > 1) {
	    samples = atol(argv[1]);
	}

    int rank, numProcs; 
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    long count = 0;
    srand(time(NULL) + rank);
    long samples_per_rank = (samples + (numProcs - 1)) / numProcs;

    for (int i = 0; i < samples_per_rank; ++i) {
        double x = ((double) rand()) / RAND_MAX;
        double y = ((double) rand()) / RAND_MAX;
        if ((x * x + y * y) <= 1.0) {
            count++;
        }
    }
    
    if (rank == 0) {
        for(int r = 1; r < numProcs; r++) {
            long result = 0;
            MPI_Recv(&result, 1, MPI_LONG, r, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            count += result;
        }
    }
    else {
        MPI_Send(&count, 1, MPI_LONG, 0, 1, MPI_COMM_WORLD);
    }
    
    if (rank == 0) {
        double pi = ((double) count / (double) samples) * 4.0;
        printf("(Parallel) Estimate of pi with %ld samples and %d ranks: \n", samples, numProcs);
        printf("π ≈ %g \n", pi);
    }
 
    MPI_Finalize();

    return 0;
}