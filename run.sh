#!/bin/bash

# Define the number of times to run the ls command
NUM_RUNS=20000

# Loop to run the ls command multiple times
for ((i = 1; i <= NUM_RUNS; i++)); do
    echo "Run $i:"
    ./walk
    echo "-----------------------------------"
done