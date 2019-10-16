#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

int main(int argc, char* argv[])
{
	if(argc != 2){
		printf("Please give parameter: number of samples\n");
		return 0;
	}

	int samples = atoi(argv[1]);

	double x,y;
	int i, count = 0; /* # of points in the 1st quadrant of unit circle */
	double z;
	double pi;
	long unsigned seed = time(Null);

	/* initialize random numbers */
	srand(seed);

	count = 0;
	for ( i = 0; i < samples; i++) {
		x = (double)rand()/RAND_MAX;
		y = (double)rand()/RAND_MAX;
		z = x*x + y*y;
		if (z <= 1) count++;
	}

	pi = (double)count/samples*4;
	printf("(Sequential) Estimate of pi with %d samples is %g \n", samples, pi);
}