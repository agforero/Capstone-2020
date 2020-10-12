#!/bin/bash

if [ ! -f graphOutputDriver.exe ]; then
    if [ ! -f graphOutputDriver ]; then
        ./make.sh
    fi
fi

./graphOutputDriver > graphOutput.json