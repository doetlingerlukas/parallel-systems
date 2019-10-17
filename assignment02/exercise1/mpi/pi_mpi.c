#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char *argv[]) {

	if (argc != 2){
        printf("invalid number of arguments\n");
        return EXIT_FAILURE;
    }
    int samples = atoi(argv[1]);

    MPI_Init(NULL, NULL); 

    int rank_id;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);   

    // init random generator
    srand(time(NULL)); 

    // every rank executes all sample points
    int in_circle = 0; 
    int in_square = 0;
    for (int i=0; i <= samples; i++){
        double x = ((double) rand() / (RAND_MAX));
        double y = ((double) rand() / (RAND_MAX));
        if (x*x + y*y <= 1){
            in_circle++;
        }
        in_square++;
    }

    // collect and reduce results
    int red_in_circle, red_in_square;
    MPI_Reduce(&in_circle,
                   &red_in_circle,
                   1,
                   MPI_INT,
                   MPI_SUM,
                   0,
                   MPI_COMM_WORLD);
    MPI_Reduce(&samples,
                   &red_in_square,
                   1,
                   MPI_INT,
                   MPI_SUM,
                   0,
                   MPI_COMM_WORLD);

    // root rank calculates and prints pi
    if (rank_id == 0){
        double pi = ((double)red_in_circle/(double)red_in_square)*4.0;
        printf("%f\n", pi);

    }
 
    MPI_Finalize(); 
	return EXIT_SUCCESS;
}
