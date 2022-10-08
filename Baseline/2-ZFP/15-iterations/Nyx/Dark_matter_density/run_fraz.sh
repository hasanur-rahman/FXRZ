#!/bin/sh

datasetPath="$1"


varList="7 8 12 13 21 22"
for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
