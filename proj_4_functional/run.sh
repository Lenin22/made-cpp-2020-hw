#!/bin/bash

set -e

g++ -std=c++17 ./src/main.cpp -o compose_test
./compose_test

echo All tests passed!