#!/bin/sh

datasetPath="$1"

varList="35 50 62 87 99 117 190 244 270 292"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
