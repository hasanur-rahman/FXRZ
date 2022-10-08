## Instructions for running FraZ

- Run *make* to work with FRaZ
```
make
```

- Run FraZ with below command:
```
./run_fraz.sh datasetPath
```

- One input arguments are needed:
	- datasetPath: Dataset File Path

	- For example, if datasetPath is './BaryonDensity.f32' , then the example command is as follows:
```
		./run_fraz.sh  "BaryonDensity.f32" 2>&1 Output.log
```

- After saving the output in the Output.log file, we can extract the necessary information for FraZ as follows:
```
python3 extract_fraz_result.py Output.log
```


