#!/bin/sh

datasetPath="$1"

varList="5 9 12 16 20 23 26 28 30 34 40 45 53 64 66 71 79 81 84 91 103 109 114 128 134 139 146 151 157"


for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
