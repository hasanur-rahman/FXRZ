#!/bin/sh

datasetPath="$1"


varList="84 150 184 222 302 319 357 432 491 528 556 572 612 632 648 660 688 710 751 777 811 837"
for var in $varList; do
        mpirun -np 1 ./run_fraz $datasetPath $var
done


echo 'finished'
