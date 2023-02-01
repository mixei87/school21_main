#!/bin/bash
flags="flags/flags.txt"
FAILURE=0
SUCCESS=0
ERR=""
PLUS=""
RES=test_result
DIFF=test_diff

IFS=:
for flag in $(cat "$flags"); do
    prefix="$(echo "${flag}" | sed 's/ //g' | sed 's/^-//')"
    if [ -n "$flag" ]; then
        prefix+="_"
    fi
    IFS=" "
    grep ""$flag$@"" &> "${RES}"/"${prefix}"origin.txt
    ./s21_grep ""$flag$@"" &> "${RES}"/"${prefix}"my.txt
    diff -u "${RES}"/"${prefix}"my.txt "${RES}"/"${prefix}"origin.txt > "${DIFF}"/"${prefix}"diff.txt
    if [ -s "${DIFF}"/"${prefix}"diff.txt ]; then 
        FAILURE=$(( $FAILURE + 1 ))
        flag="$(echo "${flag}" | sed 's/[[:space:]]*$//')"
        ERR+="${flag}; " 
    else
        SUCCESS=$(( $SUCCESS + 1 ))
        PLUS+="${flag}; " 
    fi
done
if [ $FAILURE -ne 0 ]; then
    echo -e '\033[1;31m'"FLAGS: ${ERR}"'\033[0m'
fi
echo -e '\033[1;31m'"FAILURE: $FAILURE"'\033[0m'
echo -e '\033[1;32m'"SUCCESS: $SUCCESS"'\033[0m'
if [ $FAILURE -ne 0 ]; then
    echo -e '\033[1;32m'"FLAGS: ${PLUS}"'\033[0m'
fi
