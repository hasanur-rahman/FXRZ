#!/bin/sh

datasetPath="$1"

varList="96 131 178 190 205 218 247 287 309 314 333 356 362 380 394 411 419 428 432 447"


for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
