#!/bin/bash

echo "compiling!"
g++ -c -g -std=c++11 graphOutputDriver.cpp
g++ -o graphOutputDriver graphOutputDriver.o graphOutput.hpp
