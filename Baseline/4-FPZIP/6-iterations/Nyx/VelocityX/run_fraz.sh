#!/bin/sh

datasetPath="$1"

varList="3.09 6 10 13 16 24 34 35 45 78"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
