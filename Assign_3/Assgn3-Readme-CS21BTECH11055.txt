/*-----------------------------------------------------------------------------------------*/
                        Input Instructions
/*-----------------------------------------------------------------------------------------*/
1)Create a file with name inp-params.txt ,this file should be in same directory as the source code files

2)consisting of all the parameters : n, k, λ1, λ2. 
 
  where,
  n is no of threads.
  k is no of iterations.
  λ1, λ2 are averages of exponential ditribution.(in milliseconds)
  
 A sample input file is: 
   10 10 5 20.

3)Note: the input file name should be inp-params.txt ,with any other name the code will not be executed.

4)save the file   



(This is done on  Ubuntu commands, may vary for mac-os)
/*-----------------------------------------------------------------------------------------*/
                        Compile And execution Instructions for source files
/*-----------------------------------------------------------------------------------------*/
1)Compile commands:
    g++ -o TAS Assgn3-Src-tas-CS21BTECH11055.cpp -lpthread
    g++ -o CAS Assgn3-Src-cas-CS21BTECH11055.cpp -lpthread
    g++ -o cas-bounded Assgn3-Src-cas-bounded-CS21BTECH11055.cpp -lpthread

2)execution commands:(do not forget to create input file and give pramaters before execution)
    ./TAS
    ./CAS
    ./cas-bounded

3)output files will be generated please be paitent until the execution is completed

  output_TAS.txt
  output_CAS.txt
  output_BWCAS.txt

  will be produced.   .




    
