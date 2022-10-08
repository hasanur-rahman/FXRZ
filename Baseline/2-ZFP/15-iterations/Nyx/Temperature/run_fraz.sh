#!/bin/sh

datasetPath="$1"

varList="8 10 11 14 17 21 23 26 41 54 58"


for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
