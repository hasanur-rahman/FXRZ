#!/bin/sh

datasetPath="$1"

varList="56 71 85 108 123 143 150 164 178 190 205 210 222 234 246 259 268 277 286 306 315 337 356 367 375 388 393"


for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
