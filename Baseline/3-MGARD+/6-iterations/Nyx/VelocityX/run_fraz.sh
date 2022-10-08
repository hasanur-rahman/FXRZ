#!/bin/sh

datasetPath="$1"

varList="19 30 44 65 71 80 89 105 112 119 122 128 135 140 158 170 183 188 198 207 211 225 231 243 250 262 282 295 306 319 326 339 342 352 362 371 379 391 399 406 413 428"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
