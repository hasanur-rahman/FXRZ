#!/bin/sh

datasetPath="$1"


varList="60 84 100 130 150 170 184 200 222 250 302 319 357 390 420 432 450 480 491 510 528 556 572"
for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
