# Assignment 1

*by Andreas Peintner, Josef Gugglberger and Lukas Dötlinger*

## Exercise 1

### How to submit jobs, check status and delete jobs

* `qsub <script_file>`: submit a job to SGE
* `qstat`: check current state of a submitted job
* `qdel <job_id>`: delete submitted job from queue 

### Submission script

```
#!/bin/bash

#$ -q std.q
#$ -cwd
#$ -N my_test_job
#$ -o output.dat
#$ -j yes
#$ -pe openmpi-2perhost 8

module load openmpi/4.0.1
mpiexec -n 8 /bin/hostname
```

### 5 most important qsub parameters

1. `-pe` to set up parallel environment 
2. `-q` to submit job to specefic queue
3. `-N` to name a job
4. `-o` to specify the output file
5. `-cwd` to execute job in current working directory (defaults to `$HOME`)

### How to run a MPI program in parallel

The line `#$ -pe openmpi-2perhost 8` in the job script sets up the parallel environment. The line as it is means: reserve 8 cores in total, with each used node having a maximum of 2 CPU/core slots. To actually start your program in parallel, the job script needs to be executed with the command `mpiexec -n 8 <script_file>`. The `-n` flag is set to 8, which executes the command/program 8 times (i.e. starts 8 processes).

## Exercise 2

### The modified experiment
To specify bindings, we use the `--map-by` flag for `mpiexec`. We modified our scripts, so we can use the same flag with the same syntax (or shortended `-mb`) on the script.

To run the two processes (MPI ranks) on **different cores of the same socket**, we use
```shell
qsub -pe openmpi-2perhost 2 <bw.sh or latency.sh>
```
No binding necessary since `qsub` allocates in a linear fashion. We could add `--map-by core` at the end of the command to force a binding.

To run the two processes (MPI ranks) on **different sockets of the same node**, we use
```shell
qsub -pe openmpi-2perhost 2 <bw.sh or latency.sh> --map-by socket
```

To run the two processes (MPI ranks) on **different nodes**, we use
```shell
qsub -pe openmpi-1perhost 2 <bw.sh or latency.sh>
```


### Verify rank placement

To verify rank placement, we use the parameter `-display-allocation` and `-display-map` with `mpiexec`.

* `-display-allocation` shows the allocated resources on each of the used nodes
* `-display-map` shows all the mapped locations (socket and cpu core) for each process prior to launch

### Measured data

#### Bandwidth (MB/s)

| Size      | same socket | same node | different nodes |
| --------: | -----------------: | -----------------: | -----------------: |
| 1       |    4.26 |    4.39 |    0.63 |
| 2       |    8.67 |    9.00 |    1.27 |
| 4       |   17.33 |   18.07 |    2.53 |
| 8       |   34.21 |   35.88 |    5.11 |
| 16      |   61.20 |   67.01 |   10.25 |
| 32      |  123.70 |  135.60 |   20.49 |
| 64      |  207.78 |  199.87 |   40.68 |
| 128     |  349.22 |  193.91 |   77.88 |
| 256     |  592.99 |  295.22 |  153.29 |
| 512     | 1324.06 |  474.67 |  294.33 |
| 1024    | 2450.11 |  643.15 |  537.06 |
| 2048    | 4156.24 |  822.11 |  745.94 |
| 4096    | 1586.01 | 1219.65 |  924.45 |
| 8192    | 2429.82 | 1976.58 | 1064.08 |
| 16384   | 3151.97 | 2740.98 | 1104.19 |
| 32768   | 4147.91 | 3785.42 | 1306.28 |
| 65536   | 4973.10 | 4691.42 | 1420.98 |
| 131072  | 5554.64 | 5366.83 | 1485.48 |
| 262144  | 5932.29 | 5678.31 | 1502.98 |
| 524288  | 6107.04 | 5995.15 | 1520.07 |
| 1048576 | 6214.19 | 6181.23 | 1529.99 |
| 2097152 | 5933.88 | 5995.51 | 1535.60 |
| 4194304 | 1634.46 | 1620.56 | 1537.98 |

#### Latency (us)

| Size | same socket | same node | different nodes |
| --------: | -----------------: | -----------------: | -----------------: |
| 0       |    0.41 |     0.63 |    3.50 |
| 1       |    0.46 |     0.69 |    3.55 |
| 2       |    0.46 |     0.69 |    3.55 |
| 4       |    0.46 |     0.69 |    3.55 |
| 8       |    0.47 |     0.69 |    3.61 |
| 16      |    0.49 |     0.82 |    3.63 |
| 32      |    0.48 |     0.83 |    3.67 |
| 64      |    0.53 |     0.93 |    3.84 |
| 128     |    0.56 |     1.16 |    4.84 |
| 256     |    0.60 |     1.42 |    5.35 |
| 512     |    0.88 |     2.21 |    6.25 |
| 1024    |    1.01 |     2.99 |    7.43 |
| 2048    |    1.30 |     4.75 |    9.97 |
| 4096    |    3.94 |     4.84 |   12.61 |
| 8192    |    4.86 |     5.86 |   18.46 |
| 16384   |    6.57 |     7.62 |   26.24 |
| 32768   |    9.18 |    10.23 |   36.68 |
| 65536   |   14.50 |    16.31 |   57.71 |
| 131072  |   24.88 |    28.16 |  100.57 |
| 262144  |   45.64 |    51.99 |  187.17 |
| 524288  |   87.21 |    97.16 |  358.09 |
| 1048576 |  177.90 |   192.50 |  698.74 |
| 2097152 | 1287.09 |   384.03 | 1379.51 |
| 4194304 | 2894.17 |  2603.40 | 2746.77 |

### Observed effects

When increasing the number of messages in the test, the latency also increases on a linear basis. While the bandwidth overall increases aswell, it doesn't really do that in a linear way. 

Although this effect is to be expected, we can see that the results for latency don't really change until we hit an amount of 100 messages or so. Another interesting behaviour we observed is the fluctuation in bandwith-measurements, when running both processes on the same node.

As expected the latency between two cores of the same node is smaller than between two nodes. But with increasing size the difference becomes less (see chart below). While at a low size, the latency between two cores is roughly ten times smaller than between two nodes, at a large size the factor between them is only ~1.1. A similar effect can also be seen in the bandwith measurements.

![](img/latency.png)

![](img/bandwidth.png)
