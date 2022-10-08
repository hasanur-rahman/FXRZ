#!/bin/sh

datasetPath="$1"

varList="117 133 156 181 211 224 243 263 342 359 375 391 412 455 467 491"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
