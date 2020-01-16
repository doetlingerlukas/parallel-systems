use Random;
use Time;

const startTime = getCurrentTime();

config const n = 100000000;// number of random points to try
config const seed = 42; // seed for random number generator

writeln("Number of points    = ", n);
writeln("Random number seed  = ", seed);

var rs = new RandomStream(real, seed, parSafe=false);

var count : sync int = 0;
forall i in 0..n do
    if (rs.getNext()**2 + rs.getNext()**2) <= 1.0 then 
        count += 1;

var res = count;
writeln("Approximation of pi = ", res*4.0 / n);

const stopTime = getCurrentTime();
writeln("Elapsed time was: ", stopTime - startTime);