#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: ./run.sh <file.sb | file.sa>"
    exit 1
fi

FILE="$1"
BASENAME="${FILE%.*}"
EXTENSION="${FILE##*.}"

if [ "$EXTENSION" == "sb" ]; then
    echo "Compiling Basic..."
    ./sbc "$FILE" "${BASENAME}.sa"
    if [ $? -ne 0 ]; then
        echo "Basic compilation failed"
        exit 1
    fi
    echo "Assembling..."
    ./sat "${BASENAME}.sa" "${BASENAME}.o"
    if [ $? -ne 0 ]; then
        echo "Assembly failed"
        exit 1
    fi
elif [ "$EXTENSION" == "sa" ]; then
    echo "Assembling..."
    ./sat "$FILE" "${BASENAME}.o"
    if [ $? -ne 0 ]; then
        echo "Assembly failed"
        exit 1
    fi
else
    echo "Unknown extension: $EXTENSION"
    exit 1
fi

echo "Running..."
./console/console "${BASENAME}.o"
