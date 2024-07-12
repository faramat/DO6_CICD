#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF=""

s21_command=(
    "./s21_cat"
    )
sys_command=(
    "cat"
    )

tests=(
"FLAGS test_files/test_case_cat.txt"
"FLAGS test_files/test_case_cat.txt test_files/test_1_cat.txt"
)
flags=(
    "e"
    "t"
    "v"
)
manual=(
"-t test_files/test_3_cat.txt"
"no_file.txt"
"-v test_files/test_5_cat.txt"
"-- test_files/test_5_cat.txt"
)

gnu=(
"-T test_files/test_1_cat.txt"
"-E test_files/test_1_cat.txt"
"-vT test_files/test_3_cat.txt"
"--number test_files/test_2_cat.txt"
"--squeeze-blank test_files/test_1_cat.txt"
"--number-nonblank test_files/test_4_cat.txt"
"test_files/test_1_cat.txt --number --number"
"-bnvste test_files/test_6_cat.txt"
)
run_test() {
    param=$(echo "$@" | sed "s/FLAGS/$var/")
    "${s21_command[@]}" $param > "${s21_command[@]}".log
    "${sys_command[@]}" $param > "${sys_command[@]}".log
    DIFF="$(diff -s "${s21_command[@]}".log "${sys_command[@]}".log)"
    let "COUNTER++"
    if [ "$DIFF" == "Files "${s21_command[@]}".log and "${sys_command[@]}".log are identical" ]
    then
        let "SUCCESS++"
        echo "$COUNTER - Success $param"
    else
        let "FAIL++"
        echo "$COUNTER - Fail $param"
    fi
    rm -f "${s21_command[@]}".log "${sys_command[@]}".log
}

echo "^^^^^^^^^^^^^^^^^^^^^^^"
echo "TESTS WITH NORMAL FLAGS"
echo "^^^^^^^^^^^^^^^^^^^^^^^"
printf "\n"
echo "#######################"
echo "MANUAL TESTS"
echo "#######################"
printf "\n"
for i in "${manual[@]}"
do
    var="-"
    run_test "$i"
done
printf "\n"
echo "#######################"
echo "AUTOTESTS"
echo "#######################"
printf "\n"
echo "======================="
echo "1 PARAMETER"
echo "======================="
printf "\n"

for var1 in "${flags[@]}"
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        run_test "$i"
    done
done
printf "\n"

printf "\n"
echo "FAILED: $FAIL"
echo "SUCCESSFUL: $SUCCESS"
echo "ALL: $COUNTER"
printf "\n"
