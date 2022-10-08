#!/bin/sh

datasetPath="$1"

varList="39 54 74 102 126 161 179 202 222 242 262 286 304 323 342 357 379 399 412 427 446 463 475 500 510 526 542 562 584 597"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
