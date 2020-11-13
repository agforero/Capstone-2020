#!/bin/bash

echo "compiling!"

# compile objects in /source/
cd ../source/
g++ -c -g -std=c++11 generationFunctions.cpp
g++ -c -g -std=c++11 graphAlgos.cpp
g++ -c -g -std=c++11 graphClassHelperFunctions.cpp

# compile driver in /bin/
cd ../bin/
g++ -c -g -std=c++11 graphOutputDriver.cpp

# link everything together, still in /bin/
g++ -o graphOutputDriver graphOutputDriver.o \
    ../source/graphMainHeader.hpp \
    ../source/nodeClass.hpp \
    ../source/globalHelperFunctions.hpp \
    ../source/generationFunctions.o \
    ../source/graphAlgos.o \
    ../source/graphClassHelperFunctions.o
