#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {

	if (argc != 2){
        printf("invalid number of arguments\n");
        return EXIT_FAILURE;
    }
    int samples = atoi(argv[1]);

    // init random generator
    srand(time(NULL)); 

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

    double pi = (4.0 * in_circle) / in_square; 
    
    printf("%f\n", pi);

	return EXIT_SUCCESS;
}
