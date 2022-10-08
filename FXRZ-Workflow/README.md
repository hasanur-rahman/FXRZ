# FXRZ: Feature-driven Compressor-agnostic Lossy Compression Framework

## Required Packages
- python3 sklearn, scipy

## Steps to Use
- Features Extraction
	- This step extracts the 5 features we described in the paper. Also, this step implements the adjustment of a target compression ratio by calculating percentage of non-constant blocks
- Data Augmentation
	- This step provides the augmentation of data based on given stationary points
- Training and Testing Data Generation
	- This step inputs previous two steps' outputs and provide training or testing data as required.
- *Random Forest Regressor* Evaluation
	- This is the final step which actually train and test the model accuracy with the output from the previous step to provide compression ratio estimation error. 


