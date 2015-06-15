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

This is done in the function `void svm_struct_init_hierarchy(STRUCT_LEARN_PARM * sparm)` in `svm_strut_api.c`.

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


