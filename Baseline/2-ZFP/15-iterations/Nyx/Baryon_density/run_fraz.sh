#!/bin/sh

datasetPath="$1"

varList="6 7 9 13 19 24 28 37 39 54 58 77 78"
for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
