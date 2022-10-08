#!/bin/sh

datasetPath="$1"

varList="2.5 6 7 9 13 14 15 25 37"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
