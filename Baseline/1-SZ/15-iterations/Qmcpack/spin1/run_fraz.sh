#!/bin/sh

datasetPath="$1"

varList="21 36 63 93 144 174 204 238 269 291 324 348 396 419 443 466 480 509 551 578 593"


for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
