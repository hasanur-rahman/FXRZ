#!/bin/sh

datasetPath="$1"

varList="6 14 17 19 27 28 39 41 53 56 72 90 104"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
