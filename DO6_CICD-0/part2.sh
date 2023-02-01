#!/bin/bash

CAT="src/cat"
GREP="src/grep"
ERROR=0

files=("$CAT" "$GREP")

for file in ${files[*]}; do
    cp materials/linters/CPPLINT.cfg "$file"
    python3 materials/linters/cpplint.py --extensions=c "$file"/*.c
    if [[ $? -ne 0 ]]; then
        ERROR=1
    fi
    python3 materials/linters/cpplint.py --extensions=h "$file"/*.h
    if [[ $? -ne 0 ]]; then
        ERROR=1
    fi
    rm -f "$file"/CPPLINT.cfg
done

exit $ERROR
