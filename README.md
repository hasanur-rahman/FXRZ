A Feature-Driven Fixed-Ratio Lossy Compression Framework for Real-World Scientific Datasets
=====

* Published in 39th IEEE International Conference on Data Engineering (ICDE), 2023. 
* Authors: [Md Hasanur Rahman](https://hasanur-rahman.github.io), Sheng Di, Kai Zhao, Robert Underwood, Guanpeng Li and Franck Cappello. 
* This project was done with the collaboration of Argonne National Laboratory.

## Research Goal
This paper proposes a feature-driven compressor-agnostic fixed-ratio framework **FXRZ**, which can efficiently estimate the expected error bound setting based on a user-specified compression ratio. Please check our ICDE'23 [paper](https://doi.org/10.1109/ICDE55515.2023.00116) for more details.

## Paper Abstract
Today’s scientific applications and advanced instruments are producing extremely large volumes of data everyday, so that error-controlled lossy compression has become a critical technique to the scientific data storage and management. Existing lossy scientific data compressors, however, are designed mainly based on error-control driven mechanism, which cannot be efficiently applied in the fixed-ratio use-case, where a desired compression ratio needs to be reached because of the restricted data processing/management resources such as limited memory/storage capacity and network bandwidth. To address this gap, we propose a low-cost compressor-agnostic feature-driven fixed-ratio lossy compression framework (FXRZ). The key contributions are three-fold. (1) We perform an in-depth analysis of the correlation between diverse data features and compression ratios based on a wide range of application datasets, which is a fundamental work for our framework. (2) We propose a series of optimization strategies that can enable the framework to reach a fairly high accuracy in identifying the expected error configuration with very low computational cost. (3) We comprehensively evaluate our framework using 4 state-of-the-art error-controlled lossy compressors on 10 different snapshots and simulation configuration-based real-world scientific datasets from 4 different applications across different domains. Our experiment shows that FXRZ outperforms the state-of-the-art related work by 108×. The experiments with 4,096 cores on a supercomputer show a performance gain of 1.18∼8.71× than the related work in overall parallel data dumping.

## How to Cite
If you want to include our paper in your work, please cite our paper. You can find the bibtex citation [**here**](https://hasanur-rahman.github.io/cites/ICDE23-FXRZ.bib). 

## Introduction to Different Sections of This Repo
* Baseline: this folder contains the code and evaluations of the baseline [FRaZ](https://ieeexplore.ieee.org/abstract/document/9139812), which is a high-cost generic fixed-ratio framework.
* Compressor-executables: this folder describes how to execute each of our evaluated compressors.
* FXRZ-Workflow: this folder provides code and step-by-step instructions of how to install and run our framework FXRZ.
* Testing-Dataset: we provide a sample testing dataset to test our framework FXRZ.

## License

This work is licensed under a [Creative Commons Attribution 4.0 International License](https://creativecommons.org/licenses/by/4.0/).

[![CC BY 4.0](https://licensebuttons.net/l/by/4.0/88x31.png)](https://creativecommons.org/licenses/by/4.0/)
