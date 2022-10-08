#!/bin/sh

datasetPath="$1"


varList="14 16 19 23 30 35 42 52 54 64 66 82"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
