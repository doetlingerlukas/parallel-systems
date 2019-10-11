#!/bin/bash

# Run benchmark on different sockets (default proceedure)
qsub /ex2/latency.sh
qsub /ex2/bw.sh

# Run benchmark on different cores of same socket
qsub /ex2/latency_core.sh
qsub /ex2/bw_core.sh

# Run benchmark on differen nodes
qsub /ex2/latency_node.sh
qsub /ex2/bw_node.sh