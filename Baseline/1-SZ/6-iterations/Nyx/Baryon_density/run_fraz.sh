#!/bin/sh

datasetPath="$1"

varList="47 85 125 180 240 256 272 291 311 322 328 336 348 357 369 374 384 391 400 406 409 419 425 436 462 467 489"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
