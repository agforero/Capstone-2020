#!/bin/bash

echo "compiling!"
g++ -c -g -std=c++11 graphOutputDriver.cpp
g++ -c -g -std=c++11 generationFunctions.cpp
g++ -c -g -std=c++11 graphAlgos.cpp
g++ -c -g -std=c++11 graphClassHelperFunctions.cpp
g++ -o graphOutputDriver graphOutputDriver.o graphOutputHeader.hpp nodeClass.hpp globalHelperFunctions.hpp generationFunctions.o graphAlgos.o graphClassHelperFunctions.o
