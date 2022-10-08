
# Model Evaluation

## Package required
- python3 sklearn

## Instructions

- *random_forest_model.py* is needed to predict the error bound setttins based on given target compression ratios
	- Two arguments are needed to run this python script
		- train data
		- test data
	- we provide train and test data in *train* and *test* data folder respectively
	- For example, if the train data is **BaryonDensity_train.csv** and test data is **BaryonDensity_test.csv**, then the command is as follows:
	```	
		python3 random_forest_model.py train/BaryonDensity_train.csv test/BaryonDensity_test.csv
 	```
- There is a *model-evaluation* folder in every subdirectory in which we run the corresponding compressor based on predicted error bound settings by FXRZ.

- We provide a bash script *batch_run.sh* to evaluate the FXRZ.
	- One input argument is needed 
		- the dataset path
	- For example, the dataset path is **./BaryonDensity.f32**, then the command is as follows:
		- ./batch_run "./BaryonDensity.f32"

## Results

- We also provide all the *random forest model* results in the corresponding *Output* folder
- We provide *model evaluation* results in the *compression-ratio-results.csv* file inside *model-evaluation* folder
