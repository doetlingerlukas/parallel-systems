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
| 1       |    4.26 |    4.35 |    0.63 |
| 2       |    8.67 |    8.76 |    1.27 |
| 4       |   17.33 |   17.62 |    2.53 |
| 8       |   34.21 |   35.14 |    5.11 |
| 16      |   61.20 |   60.84 |   10.25 |
| 32      |  123.70 |  123.72 |   20.49 |
| 64      |  207.78 |  208.77 |   40.68 |
| 128     |  349.22 |  357.32 |   77.88 |
| 256     |  592.99 |  612.12 |  153.29 |
| 512     | 1324.06 | 1364.25 |  294.33 |
| 1024    | 2450.11 | 2502.65 |  537.06 |
| 2048    | 4156.24 | 4215.30 |  745.94 |
| 4096    | 1586.01 | 1589.86 |  924.45 |
| 8192    | 2429.82 | 2417.88 | 1064.08 |
| 16384   | 3151.97 | 3096.59 | 1104.19 |
| 32768   | 4147.91 | 4128.32 | 1306.28 |
| 65536   | 4973.10 | 4965.63 | 1420.98 |
| 131072  | 5554.64 | 5533.68 | 1485.48 |
| 262144  | 5932.29 | 5894.48 | 1502.98 |
| 524288  | 6107.04 | 6059.00 | 1520.07 |
| 1048576 | 6214.19 | 6187.72 | 1529.99 |
| 2097152 | 5933.88 | 6156.45 | 1535.60 |
| 4194304 | 1634.46 | 1616.92 | 1537.98 |

#### Latency (us)

| Size | same socket | same node | different nodes |
| --------: | -----------------: | -----------------: | -----------------: |
| 0       |    0.41 |     0.43 |    3.50 |
| 1       |    0.46 |     0.48 |    3.55 |
| 2       |    0.46 |     0.48 |    3.55 |
| 4       |    0.46 |     0.48 |    3.55 |
| 8       |    0.47 |     0.48 |    3.61 |
| 16      |    0.49 |     0.50 |    3.63 |
| 32      |    0.48 |     0.50 |    3.67 |
| 64      |    0.53 |     0.55 |    3.84 |
| 128     |    0.56 |     0.57 |    4.84 |
| 256     |    0.60 |     0.61 |    5.35 |
| 512     |    0.88 |     0.88 |    6.25 |
| 1024    |    1.01 |     1.03 |    7.43 |
| 2048    |    1.30 |     1.32 |    9.97 |
| 4096    |    3.94 |     4.03 |   12.61 |
| 8192    |    4.86 |     4.97 |   18.46 |
| 16384   |    6.57 |     6.64 |   26.24 |
| 32768   |    9.18 |     9.23 |   36.68 |
| 65536   |   14.50 |    14.52 |   57.71 |
| 131072  |   24.88 |    25.06 |  100.57 |
| 262144  |   45.64 |    45.94 |  187.17 |
| 524288  |   87.21 |    90.42 |  358.09 |
| 1048576 |  177.90 |   185.25 |  698.74 |
| 2097152 | 1287.09 |  1307.03 | 1379.51 |
| 4194304 | 2894.17 |  2901.13 | 2746.77 |

### Observed effects

When increasing the number of messages in the test, the latency also increases on a linear basis. While the bandwidth overall increases aswell, it doesn't really do that in a linear way. 

Although this effect is to be expected, we can see that the results for latency don't really change until we hit an amount of 100 messages or so. Another interesting behaviour we observed is the fluctuation in bandwith-measurements, when running both processes on the same node.

As expected the latency between two cores of the same node is smaller than between two nodes. But with increasing size the difference becomes less (see chart below). While at a low size, the latency between two cores is roughly ten times smaller than between two nodes, at a large size the factor between them is only ~1.1. A similar effect can also be seen in the bandwith measurements.

![](img/latency.png)

![](img/bandwidth.png)
