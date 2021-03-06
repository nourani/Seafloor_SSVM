# Seafloor_SSVM - **Structural SVM for Hierarchical Classification**

##Introduction
Seafloor_SSVM is a repository with an implementation of a Structural SVM for hierarchical classification. The source code provided contains the implementation described in our IbPRIA 2015 paper for the particular problem of seafloor imagery taxonomic categorization. If you find this code useful in your research, please consider citing:

Navid Nourani-Vatani, Roberto J. Lopez-Sastre, and Stefan Williams. *Structured Output Prediction with Hierarchical Loss Functions for Seafloor Imagery Taxonomic Categorization*. **IPRIA 2015** [PDF](http://agamenon.tsc.uah.es/Personales/rlopez/docs/ibpria2015-nourani.pdf)


## How to compile the code?
Simple! Just run the script `./compile.sh` . The code is developed and tested under Ubuntu 14.04 and the following libraries are required to build the code: gcc, cmake.

## How to reproduce the experiments in the paper with the Tasmania Coral Point Count dataset?

In order to reproduce the results reported in our IbPRIA publication one simply has to type:
`./svm_struct_test data/TasmaniaCoralPointCountDataset results 0 10`


This will complete a training, validation and testing process for the Structural SVM model proposed. All the hierarchical loss functions described in the paper will be evaluated.

## How to run my own experiments?
Type: `./svm_struct_test` for the online help.

## I want to use a different dataset and taxonomy. How should I proceed?
Follow these steps:

### Creating your own hierarchy
You can generate your own hierarchy by using the C++ class Hierarchy (or its corresponding C-wrapper) found in the hierarchy sub-folder.

1. Add the total number of nodes by calling `addNodes(unsigned int n)`
2. Connect children with their parents by calling `connectParentChield(int p, int c)`
3. Call `reconnect()` to create the actual connections

This is done in the function `void svm_struct_init_hierarchy(STRUCT_LEARN_PARM * sparm)` in `svm_struct_api.c`.

#### example
To create the hierarchy presented in the paper you need to do the following:

```
Hierarchy h;
h.addNodes(20);
h.connectParentChield(0, 1);
h.connectParentChield(0, 4);
h.connectParentChield(1, 2);
h.connectParentChield(1, 3);
h.connectParentChield(4, 5);
h.connectParentChield(4, 6);
h.connectParentChield(4, 7);
h.connectParentChield(4, 8);
h.connectParentChield(4, 9);
h.connectParentChield(4, 10);
h.connectParentChield(10, 11);
h.connectParentChield(10, 14);
h.connectParentChield(10, 17);
h.connectParentChield(11, 12);
h.connectParentChield(11, 13);
h.connectParentChield(14, 15);
h.connectParentChield(14, 16);
h.connectParentChield(17, 18);
h.connectParentChield(17, 19);
h.reconnect();
```

### Using a different dataset
Using your own dataset is easy, but you need to structure it to perform cross-validation. 

* First of all, you need to separete your training and test data. The test set should **only** be used to generate your final result and should **not** be part of the training phase. You can choose the ratio between training and test yourself.
  * Create a `test.dat` file (according to the SVM-Light package http://svmlight.joachims.org/) containing the test set.
 * Create a `trainAll.dat` file containing all the (remainig) training data. 
* Secondly, the training set (`trainAll.dat`) itself also needs to be separated for the purpose of cross-validation.
 * Create a `validate.dat` file containing a subset of the `trainAll.dat` data.
 * Create a `train.dat` file with the remaining of the `trainAll.dat` data.
* Put your data inside `<your data dir>`.
* Make sure your hierarchy is correct (see above about generating your own hierarchy).
* Make the code
* Run `svm_struct_test <your data dir> <your result dir> [loss 1] [loss n]` to perform complete train-validation-test with the given loss functions
 * Call `svm_struct_test` without arguments to see the help
 * Information about the generated SVM model and the final results are put in `<your result dir>`
  
 
 



 


