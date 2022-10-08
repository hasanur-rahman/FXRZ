#!/bin/sh

datasetPath="$1"

varList="15 30 45 60 74 96 122 146 183 215 241 265 280 301 313 321 333 344 351 361 375 385 395 407 426 434 443 450"

for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
