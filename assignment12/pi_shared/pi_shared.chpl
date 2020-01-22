use Random;
use Time;

const startTime = getCurrentTime();

config const n = 100000000;// number of random points to try
config const seed = 41; // seed for random number generator

writeln("This program is running on ", numLocales, " locales");
writeln("Number of points    = ", n);
writeln("Random number seed  = ", seed);

var rs = new RandomStream(real, seed, parSafe=false);

var D = {1..n};

var count = + reduce [(x,y) in zip(rs.iterate(D), rs.iterate(D))] (x**2 + y**2) <= 1.0;

var res = count;
writeln("Approximation of pi = ", res*4.0 / n);

const stopTime = getCurrentTime();
writeln("Elapsed time was: ", stopTime - startTime);