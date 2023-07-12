#!/bin/bash

g++ -o TAS  TAS.cpp -lpthread -std=c++11 
g++ -o CAS CAS.cpp  -lpthread -std=c++11 
g++ -o CAS_W CAS_W.cpp -lpthread -std=c++11 

for i in 1 2 3 4 5 
    do
    iter=`echo "scale=6;(($i)*10)"| bc`
    ./CAS  $iter 10  >CAS.txt
    ./CAS_W $iter 10 >CAS_W.txt
    ./TAS  $iter 10  >TAS.txt
     paste TAS.txt CAS.txt CAS_W.txt -d "\t" >bash_output_$i.txt
     rm TAS.txt CAS.txt CAS_W.txt
    done 

python3 plot.py
rm *bash_output_*
