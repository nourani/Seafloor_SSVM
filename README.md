# Seafloor_SSVM - **Structural SVM for Hierarchical Classification**

##Introduction
Seafloor_SSVM is a repository with an implementation of a Structural SVM for hierarchical classification. The source code provided contains the implementation described in our IbPRIA 2015 paper for the particular problem of seafloor imagery taxonomic categorization. If you find this code useful in your research, please consider citing:

Navid Nourani-Vatani, Roberto J. Lopez-Sastre, and Stefan Williams. *Structured Output Prediction with Hierarchical Loss Functions for Seafloor Imagery Taxonomic Categorization*. **IPRIA 2015** [PDF](http://agamenon.tsc.uah.es/Personales/rlopez/docs/ibpria2015-nourani.pdf)


## How to compile the code?
Simple! Just run the script compile.sh. The code is developed and tested under Ubuntu 14.04 and the following libraries are required to build the code: gcc, cmake.

## How to reproduce the experiments in the paper with the Tasmania Coral Point Count dataset?

In order to reproduce the results reported in our IbPRIA publication one simply has to type:
<./svm_struct_test data/TasmaniaCoralPointCountDataset results 0 10> 

This will complete a training, validation and testing process for the Structural SVM model proposed. All the hierarchical loss functions described in the paper will be evaluated.

## How to run my own experiments?
Type: ./svm_struct_test for the online help.

## I want to use a different dataset and taxonomy. How should I proceed?
Follow these steps:
TODO: explain this.

