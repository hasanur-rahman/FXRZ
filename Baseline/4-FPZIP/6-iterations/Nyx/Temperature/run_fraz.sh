#!/bin/sh

datasetPath="$1"


varList="2.64 4 7 12 17 19 20 29 45 46 104"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
