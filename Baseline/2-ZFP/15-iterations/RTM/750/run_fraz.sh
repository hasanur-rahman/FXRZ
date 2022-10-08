#!/bin/sh

datasetPath="$1"

varList="115 121 125 144 158 173 192 212 237 256 283 305 347"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
