#!/bin/sh

datasetPath="$1"

varList="32 45 52 61 65 71 80 85 102 109 125 145 153 170 187 195 206 222 234 259 267 288 292"


for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
