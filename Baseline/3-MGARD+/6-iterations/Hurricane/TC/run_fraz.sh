#!/bin/sh

datasetPath="$1"

varList="18 31 40 52 68 81 92 109 120 135 146 161 181 199 211 226 237 255 263 282 292 308 321 334 349 361 380 397"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
