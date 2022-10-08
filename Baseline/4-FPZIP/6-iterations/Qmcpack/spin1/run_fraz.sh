#!/bin/sh

datasetPath="$1"


varList="3.5 5 7 10 13 14 17 29 43"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
