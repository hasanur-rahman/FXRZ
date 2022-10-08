#!/bin/sh

datasetPath="$1"


varList="101 120 132 146 150 162 180 203 230 267 300 346 352 399"
for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
