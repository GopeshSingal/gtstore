#!/bin/bash

# Parse the command-line arguments
while [[ $# -gt 0 ]]; do
    case "$1" in
        --nodes)
            NUM_NODES="$2"
            shift 2
            ;;
        --rep)
            REPLICATION="$2"
            shift 2
            ;;
        *)
            echo "Unknown flag: $1"
            exit 1
            ;;
    esac
done

# Use default values if flags are not provided
NUM_NODES=${NUM_NODES:-5}
REPLICATION=${REPLICATION:-1}

EXECUTABLE="./build/src/storage"

DEFAULT_COUNT=5

TIMEOUT_DURATION=5

for i in $(seq 1 $((NUM_NODES))); do
    $EXECUTABLE $i &
done

./build/src/manager --nodes $NUM_NODES --rep $REPLICATION