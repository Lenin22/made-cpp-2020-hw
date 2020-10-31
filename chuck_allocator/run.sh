#!/bin/bash

set -e

g++ -std=c++17 -I./src test/test.cpp -o out
./out

echo All tests passed!

