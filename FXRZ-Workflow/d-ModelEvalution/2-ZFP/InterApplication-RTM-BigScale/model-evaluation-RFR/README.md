## Instructions
To calculate the estimation error, one need run the compressor with FXRZ estimated error bound settings for target compression ratios and then compare the measured compression ratios and target compression ratios.

## Usage
One need to run *batch_run.sh* which takes one input argument -- the dataset file path. One also need to place the *test_data_with_prediction_rf.csv* file outputted by RFR model, which can be found inside *Output-RFR* folder. 

```
./batch_run.sh <dataset file path>
```
