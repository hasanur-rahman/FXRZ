#!/bin/sh

datasetPath="$1"


varList="32 61 72 115 182 198 234 266 304 317 338 352 371 397 402 417 430 444 451 459 465 469 478 487 490 497 503 514 522 527 533"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
