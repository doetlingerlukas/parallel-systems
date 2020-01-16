use Random;
config const n = 100000;// number of random points to try
config const seed = 42; // seed for random number generator

writeln("Number of points    = ", n);
writeln("Random number seed  = ", seed);

var rs = new RandomStream(uint, seed, parSafe=false);

var count = 0;
for i in 1..n do
    if (rs.getNext()**2 + rs.getNext()**2) <= 1.0 then 
        count += 1;

writeln("Approximation of pi = ", count*4.0 / n);
