#!/bin/bash

gcc  Assgn2Srcpthread-cs21btech11055.c -lpthread -lm

for j in 2 4 8 16 32 64
do
gawk -i inplace -v k=$j 'FNR==1{$1=k}1' input.txt
for i in {1..10} 
    do 
      ./a.out
      sleep 1
    done >bash_output_$j.txt
    
awk '{sum+=$1} END {print sum/NR}' bash_output_$j.txt >>bash_output_pthread_final.txt
done
  ./op_test.sh
  python3 plot.py plot_final.png
  rm *bash_output*
  echo completed




