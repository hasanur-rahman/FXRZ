#!/bin/sh

datasetPath="$1"

varList="11 13 17 19 23 25 33 40 46 58 64"


for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
