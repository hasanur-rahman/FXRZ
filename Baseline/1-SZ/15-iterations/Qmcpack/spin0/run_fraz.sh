#!/bin/sh

datasetPath="$1"

varList="28 39 80 108 166 244 257 301 320 348 402 415 452 477 492 523 561 583"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
