#!/bin/sh

datasetPath="$1"

varList="3.28 6 8 10 15 21 23 29 40 88 129"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
