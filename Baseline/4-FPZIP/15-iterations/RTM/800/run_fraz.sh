#!/bin/sh

datasetPath="$1"

varList="30 38 48 59 66 72 81 91 107 133 177 214 260"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
