#!/bin/sh

datasetPath="$1"

varList="2.28 3 5 6 7 10"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
