
For this python file to run, one needs to have following python3 packages installed:
- python3 scipy


After necessary packages are installed, One has to provide to two input arguments:
- A CSV filepath of Stationary points for this compressor and dataset
	- CSV file has two columns
		- First: Measured Compression Ratio
		- Second: Error bound setting that is used to get the Measured Compression Ratio
- A text filepath that contains sampled target compression ratios we want to evaluate

For example, if the first argument filepath is "Input/stationary_points.csv", and the second argument filepath is  "Input/sampled_target_compression_ratios.txt",
then the sample command would be as follows:

```
python3 interpolateRatioErrorBound.py "Input/stationary_points.csv" "Input/sampled_target_compression_ratios.txt"
```
