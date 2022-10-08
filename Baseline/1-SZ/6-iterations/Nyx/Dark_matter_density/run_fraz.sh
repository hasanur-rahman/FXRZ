#!/bin/sh

datasetPath="$1"

varList="8 11 19 39 46 53 82 93 121 139 160 183 195 225 251 275 292 302 319 329 340 362 377"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
