#!/bin/sh

datasetPath="$1"


varList="91 103 119 140 154 177 191 204 215 232 247 260 275 292 304 327 352 370 385 389 396 417 423 428 431 444 466 476 488 490 497 506"


for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
