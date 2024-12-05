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

NUM_NODES=${NUM_NODES:-5}
REPLICATION=${REPLICATION:-1}

EXECUTABLE="./build/src/storage"


for i in $(seq 0 $((NUM_NODES - 1))); do
    $EXECUTABLE $i &
done

./build/src/manager 