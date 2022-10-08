#!/bin/sh

datasetPath="$1"

varList="10 21 25 27 40 60 71 85 93 101 110 123 126 137 152 156 164 176 187 195 209 231 245 257 266 287 298 316 331 336"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
