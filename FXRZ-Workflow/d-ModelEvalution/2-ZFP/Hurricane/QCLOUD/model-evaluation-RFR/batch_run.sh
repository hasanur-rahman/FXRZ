#!/bin/bash

datafile="$1"
csvfile="test_data_with_prediction_rf.csv"
./run_compressor $csvfile compression-ratio-results.csv $datafile

