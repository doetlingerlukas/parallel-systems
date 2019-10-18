#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int main(int argc, char **argv) {

    auto samples = 1000000000;
	if (argc > 1) {
        samples = strtol(argv[1], NULL, 10);
    }

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
    
    cout << "\rπ ≈ " << pi << endl;
    cout << "was calculated with " << samples << " samples!" << endl;

	return EXIT_SUCCESS;
}
