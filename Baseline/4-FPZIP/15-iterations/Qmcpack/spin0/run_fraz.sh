#!/bin/sh

datasetPath="$1"

varList="3.5 5 7 8 13 14 17 26 43"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
