#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: ./run.sh <test_number>"
    exit 1
fi

TEST_NUMBER=$1

run_test() {
    case $TEST_NUMBER in
        1)
            ./test1.sh
            ;;
        2)
            ./test2.sh
            ;;
        3)
            ./test3.sh
            ;;
        *)
            echo "Unknown test number: $TEST_NUMBER"
            echo "Available tests: 1, 2, 3"
            exit 1
            ;;
    esac
}

run_test
