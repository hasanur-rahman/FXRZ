## Instructions for running FraZ

The command is as follows:
```
./run_fraz.sh datasetPath
```

One input arguments are needed:
- datasetPath: Dataset File Path

For example, if datasetPath is './BaryonDensity.f32' , then the example command is as follows:
```
./run_fraz.sh  "./BaryonDensity.f32" > Output.log
```

After saving the output in the Output.log file, we can extract the necessary information for FraZ as follows:
python3 extract_fraz_result.py Output.log


**Note:** we have already provided a output file in the Output folder that we obtained by running the FraZ with sampled compression ratios. You can check it. 
The columns description of the output csv file is as follows: TargetCompressionRatio | FraZ provided Compression Ratio | EstimationError | Runtime | FraZ estimated Error Bound 

