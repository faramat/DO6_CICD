#!/bin/bash

cp ../materials/linters/.clang-format .
RESULT=$(clang-format -n ../src/*/*.c 2>&1)
if [[ $RESULT ]]; then
    echo "$RESULT"
    rm -rf .clang-format
    exit 1
fi

rm -rf .clang-format
exit 0
