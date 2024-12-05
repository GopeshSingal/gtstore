#!/bin/bash

# File to store timing results
TIMING_FILE="timing_results.txt"

# Start timing
echo "Starting 100,000 PUT and GET operations..."
START_TIME=$(date +%s)

# Loop for 100,000 iterations
for i in $(seq 1 100000); do
    ./client.sh --put hello --val world
    ./client.sh --get hello
done

# End timing
END_TIME=$(date +%s)

# Calculate duration
DURATION=$((END_TIME - START_TIME))

# Save the result to the timing file
echo "Total time for 100,000 PUT and GET operations: $DURATION seconds" > "$TIMING_FILE"

# Print the result to the terminal
echo "Operations complete. Total time: $DURATION seconds"
