# Assignment 5

*by Andreas Peintner, Josef Gugglberger and Lukas Dötlinger*

## Task 1

Our implementation consists of four files:
- `nbody2d.cpp`
- `particle.cpp`
- `../shared/nbody.hpp`
- `../shared/particle.hpp`

Use the included `Makefile` to compile the program.

### Measurements

We used *20* timesteps for testing. All measurements were done on `lcc2`.

| room size | particles | sequential [s] |
| -: | -: | -: |
| 20² | 10 | 10 |
| 20² | 20 | 10 |
| 20² | 50 | 10 |
| 20² | 100 | 10 |
| 20² | 200 | 10 |
| 100² | 1000 | 10 |
| 500² | 5000 | 27 |

## Task 2

### Optimization

One approach to optimize the program, would be to only compute the forces for particles that are within a certain radius of the current particle. 

So what does this exactly mean? When we compute the force for a particle, we calculate the impact all particles have on the current one. We do this by calculating the *radius* between two particles, which we then use to determine how much impact they have on one another. We could omit the *force*-calculation if the radius is big enough, so the two particlase don't impact each other.

Our current approach computes each force twice (once for each of the two particles), so another optimization would be to compute the force just once for each particle pair.

### Parallelization

**Approach 1**

We could split the problem size (i.e. the particles) among the ranks. Therefore each rank would only update the forces of it's particles. At the end of each timestep, one rank would have to *gather* all the results and *broadcast* them to all ranks afterwards. This is necessary, since each rank needs to calculate the force for it's particles with all particles in the room.

Hence this will propably not be very efficient, since there is a lot of communication overhead. For **T** timesteps we would need **T** *gatherings* and **T** *broadcasts* for **N** particles. 

**Approach 2**

We could split the room size among the ranks. Since our simulation uses uniformly distributed particles, we can assume that each sub-room would have an almost equal amount of particles. Each rank would then compute all particles in it's room (starting with global knowledge of all particles). 

Using the method from our *optimization* approach, a rank could determine, which particles impact particles in another ranks part of the room. If we also were to use a *Cartesian Topology* for the ranks, we can easily determine the neighbours. After updating all positions and forces, a rank would then have to notify it's neighbours about the changed particles, that impact their room. For particles that leave the room a rank would have to send them over to the next rank.

This approach minimizes the needed communication, while still splitting the problem size almost evenly compared to *Approach 1*.


### Resources
Our implementation was influenced by:
* http://physics.princeton.edu/~fpretori/Nbody/code.htm
* https://codereview.stackexchange.com/questions/87341/brute-force-n-body-implementation-in-c?rq=1
