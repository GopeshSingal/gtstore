#!/bin/bash

# CSV file to save the output
OUTPUT_FILE="output.csv"

# Add the CSV header
echo "Key,Value,Response" > "$OUTPUT_FILE"

# Loop from 0 to 99999
for i in $(seq 0 99999); do
    # Run the client.sh command and capture the output
    RESPONSE=$(./client.sh --put "$i" --val "Value")
    # Save the result into the CSV file
    echo "$i,Value,\"$RESPONSE\"" >> "$OUTPUT_FILE"
done
