#!/bin/sh

datasetPath="$1"


varList="12 18 21 26 34 39 49 59 73 79 93 98 123"


for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
