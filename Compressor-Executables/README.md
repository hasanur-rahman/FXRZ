# Instructions

All the compressors are publicly available.

Here, we provide the executables and the libraries (.so files). Please make sure that your system can find those libraries in the path.

## SZ compressor

We use SZ2.1 which is the latest publicly available version.

For a dataset, say *A.dat*, which has 100x500x500 and an error bound setting 1E-4, we can run the SZ with the following command:
```
sz -z -f -M REL -R 1E-4 -i A.dat -3 500 500 100
sz -x -f -M REL -R 1E-4 -s A.dat.sz -i A.dat -3 500 500 100 -a
```


## ZFP compressor

For a dataset, say *A.dat*, which has 100x500x500 and an error bound setting 1E-4, we can run the ZFP with the following command:
```
zfp -s -i A.dat -z A.dat.zfp -o A.dat.zfp.out -f -a 1E-4 -3 500 500 100
```

## MGARD+ compressor

For a dataset, say *A.dat*, which has 100x500x500 and an error bound setting 1E-4, we can run the MGARD+ with the following command:
```
mgardx A.dat 0 1E-4 3 1 3 100 500 500
```


## FPZIP compressor

For a dataset, say *A.dat*, which has 100x500x500 and an error bound setting as precision 10, we can run the FPZIP with the following command:
```
fpzip -i A.dat -o A.dat.fpzip -t float -p 10 -3 500 500 100
```

