#!/bin/sh

datasetPath="$1"

varList="91 108 119 137 151 168 183 198 222 246 262 292 306 335 357 383 410 435 451 470 487"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
