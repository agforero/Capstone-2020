#!/bin/bash

if [ ! -f graphOutputDriver.exe ]; then
    if [ ! -f graphOutputDriver ]; then
        ./make.sh
    fi
fi

if [ $# -eq 1 ]; then
    ./graphOutputDriver $1 > graphOutput.json
else
    ./graphOutputDriver > graphOutput.json
fi