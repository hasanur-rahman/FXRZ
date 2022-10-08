#!/bin/sh

datasetPath="$1"

varList="31 53 84 113 142 171 197 217 232 253 277 295 306 323 343 358 373 395 403 421 442 449 472 489 503 519 536 547 562 577 591 600"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
