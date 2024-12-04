#!/bin/bash

# Initialize variables
PUT=""
VAL=""
GET=""

while [[ $# -gt 0 ]]; do
    case "$1" in
        --put)
            PUT="$2"
            shift 2
            ;;
        --val)
            VAL="$2"
            shift 2
            ;;
        --get)
            GET="$2"
            shift 2
            ;;
        *)
            echo "Unknown argument: $1"
            exit 1
            ;;
    esac
done

# Check for `put` operation
if [[ -n "$PUT" && -n "$VAL" ]]; then
    ./build/src/client --put $PUT --val $VAL
elif [[ -n "$GET" ]]; then
    ./build/src/client --get $GET
else
    echo "Invalid or incomplete arguments."
    echo "Usage:"
    echo "  --put <key> --val <value>  Perform a put operation."
    echo "  --get <key>                Perform a get operation."
    exit 1
