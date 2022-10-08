#!/bin/sh

datasetPath="$1"


varList="16.37 19 22 25 29 34 39 46 67 89 115"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
