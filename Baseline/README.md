# FraZ: Baseline

## Installation
Baseline is FraZ, which can be installed from LibPressio [here](https://github.com/robertu94/libpressio).

More specifically, FraZ can be built using Spack and LibPressio (a helper tool to run Fraz):
```
git clone https://github.com/robertu94/spack_packages robertu94_packages
source ./spack/share/spack/setup-env.sh
spack compiler find
spack repo add robertu94_packages
spack install libpressio-tools+opt+mpi ^ libpressio+sz+zfp+mgardx+fpzip+lua+mpi+libdistributed
```

One needs to have **openmpi** on the machine to install and run FraZ. openmpi 4 or above should be fine. Also, `gcc` version should be 10.2 or above.

## How to use

After installing libpressio with spack, it needs to be loaded before FraZ can be used. Following commands are necessary:

```
source ./spack/share/spack/setup-env.sh
spack load libpressio-tools
``` 
