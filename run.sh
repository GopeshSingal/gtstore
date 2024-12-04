#!/bin/bash

# Name of the compiled executable
EXECUTABLE="./build/src/storage"
$EXECUTABLE 1 &
$EXECUTABLE 2 &
$EXECUTABLE 3 &
$EXECUTABLE 4 &
$EXECUTABLE 5