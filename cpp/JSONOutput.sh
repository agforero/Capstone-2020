#!/bin/bash

if [ ! -f graphOutputDriver.exe ]; then
    if [ ! -f graphOutputDriver ]; then
        ./make.sh
    fi
fi

if [ $# -eq 2 ]; then
    ./graphOutputDriver $1 -nodes $2 > graphOutput.json
else
    ./graphOutputDriver $1 > graphOutput.json
fi