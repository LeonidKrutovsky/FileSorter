g++ main.cpp -std=gnu++1z -lstdc++fs -fopenmp -O3

time sort input.txt > out_sort.txt
time ./a.out input.txt out.txt

if diff out.txt out_sort.txt
  echo Error
else
  echo Looks good
 
