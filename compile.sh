#!/bin/bash
# Compile the project

echo "Compiling Hierarchical library"
cd hierarchical/
mkdir build
cd build
cmake ../
make
cp libHierarchical.a ../../libHierarchical.a
cd ../../

echo "Compiling the Structured SVM code for hierarchical classification"
make clean
make all
