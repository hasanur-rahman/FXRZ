#!/bin/sh

datasetPath="$1"

varList="10 18 25 35 58 63 82 84 93 108 116 122 131 143 151 160 165 176 187 196 207 219 225 231 239 254 261 273 282 296 303 316 329 350 357 372"


for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
