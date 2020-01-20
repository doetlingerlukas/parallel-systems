use Random;
use Time;

config const N = 2552;
config const seed = 42;

var t:Timer;

// Matrix A
var MA : [1..N, 1..N] real;
fillRandom(MA, seed);

// Matrix B
var MB : [1..N, 1..N] real;
fillRandom(MB, seed);

// Result Matrix
var MR : [1..N, 1..N] real;

t.start();

forall i in 1..N do {
  for j in 1..N do {
    for k in 1..N do {
      MB[i,j] += MA[i,k] * MR[k,j];
    }
  }
}

t.stop();
writeln("This took ", t.elapsed(), " seconds.");

// writeln("Resulting matrix is:");
// writeln(MA, "\n");