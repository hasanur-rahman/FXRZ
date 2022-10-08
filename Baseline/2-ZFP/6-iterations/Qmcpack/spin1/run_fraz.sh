#!/bin/sh

datasetPath="$1"


varList="18 21 24 26 32 38 41 47 49 58 63 74 82 96 98 125"
for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
