#!/bin/bash

#gcc 1.c
gcc -lpthread  main.c -lm


for j in {3,4,5,6,7,8,9,10} ; do
    python3 sudoku.py $j 16

    for i in {1..5} ; do
        sleep 1
        ./a.out
    done>time.txt

    # Name of the file
    filename="time.txt"

    # Read each line of the file

    sum=0
    while read -r line; do
      sum=$sum+$line

    done < "$filename"

    x=`echo "scale=6;(($sum)/5)" | bc`

    printf '%f\n' "$x" >> 'final1.txt'

    rm "time.txt"
done

rm ./a.out


gcc -fopenmp  main_openmp.c  -lm


for j in {3,4,5,6,7,8,9,10} ; do
    python3 sudoku.py $j 16

    for i in {1..5} ; do
        sleep 1
        ./a.out
    done>time.txt

    # Name of the file
    filename="time.txt"

    # Read each line of the file

    sum=0
    while read -r line; do
      sum=$sum+$line

    done < "$filename"

    x=`echo "scale=6;(($sum)/5)" | bc`

    printf '%f\n' "$x" >> 'final2.txt'

    rm "time.txt"
done

rm ./a.out


python3 final_plot.py
rm final2.txt
rm final1.txt