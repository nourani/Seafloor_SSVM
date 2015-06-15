/*
 * svm_struct_test.cpp
 *
 *  Created on: Aug 22, 2013
 *      Authors: navid, rober
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

//C parameter of the Structural SVM (useful for cross validation)
double Cs[] = { 1e-2, 1e-1, 1e+0, 1e+1, 1e+2 };
#define nC 5 //number of C's to be used


int main(int argc, char ** argv) {

  int loss_func = 0, loss_func_stop = 0;
  
  char cmd[256];
  int ret = 0;
  double acc[nC];
  double perf;
  //Different evaluation metrics considered
  // 0=f1, 1=avg pre, 2=avg rec, 3=acc
  int measure = 0; 
  
  memset(acc, 0, sizeof(int) * nC);
  
  char * exampleFolder;
  char * resultsFolder;
  if (argc > 4) {
    loss_func_stop = atoi(argv[4]);
    loss_func = atoi(argv[3]);
  } else if (argc > 3) {
    loss_func = atoi(argv[3]);
    loss_func_stop = loss_func;
  }
  if (argc > 2) {
    size_t len = strlen(argv[1]);
    exampleFolder = (char *) malloc(sizeof(char) * len);
    strcpy(exampleFolder, argv[1]);
    len = strlen(argv[2]);
    resultsFolder = (char *) malloc(sizeof(char) * len);
    strcpy(resultsFolder, argv[2]);
  } else {
    printf("Usage: \n\t%s data_dir results_dir [loss 1] [loss n]\n\n\n", argv[0]);
    printf("data_dir: path with the training, validation and testing data.\n\n");
    printf("results_dir: path to save the results.\n\n");
    printf("Different loss functions can be used:\n");
    printf("\t 0 : 0/1 loss\n");
    printf("\t 1 : squared difference\n");
    printf("\t 2 : weighted hierarchical difference\n");
    printf("\t 3 : hierarchical f1-loss \n");
    printf("\t 4 : hierarchical precision \n");
    printf("\t 5 : hierarchical recall \n");
    printf("\t 6 : hierarchical accuracy \n");
    printf("\t 7 : hierarchical hamming \n");
    printf("\t 8 : distance to nearest ancestor \n");
    printf("\t 9 : distance through the tree \n");
    printf("\t 10 : std hamming \n\n");				
    printf("Examples:\n\n");			
    printf("\t Run experiment with loss 2: ./svm_struct_test data results 2\n");
    printf("\t Run experiment evaluating all loss functions: ./svm_struct_test data results 0 10\n");
			
    exit(1);
  }
	
  FILE * fp = fopen("log.txt", "a");
  if (fp == NULL) {
    printf("Error opening log.txt");
    exit(1);
  }

  char trainName[] = "train.dat"; // training data
  char validName[] = "valid.dat"; //validation data
  char trainAllName[] = "trainAll.dat"; // trai + valid
  char testName[] = "test.dat"; // test data
  
  fprintf(fp, "# data folder: %s\n", exampleFolder);
  fprintf(fp, "#\tfiles: %s, %s, %s, %s\n", trainName, validName,
	  trainAllName, testName);
  printf("data folder: %s\n", exampleFolder);
  printf("\tfiles: %s, %s, %s, %s\n", trainName, validName, trainAllName,
	 testName);
  /* ************************************************************
   *
   * Train section
   *
   * ***********************************************************/
  for (; loss_func <= loss_func_stop; loss_func++) {
    printf("Using loss function %d\n", loss_func);
    
    int iC;
    
    for (iC = 0; iC < nC; iC++) {
      // train
      sprintf(cmd,
	      "./svm_multiclass_learn -v 0 -t 0 -l %d -c %f %s/%s %s/model%d.dat",
	      loss_func, Cs[iC], exampleFolder, trainName,
	      resultsFolder, loss_func);
      printf("\n%s\n", cmd);
      ret = system(cmd);
      
      // validation
      sprintf(cmd,
	      "./svm_multiclass_classify -v 0 -r %d %s/%s %s/model%d.dat %s/results%d.dat",
	      measure, exampleFolder, validName, resultsFolder,
	      loss_func, resultsFolder, loss_func);
      printf("%s\n", cmd);
      ret = system(cmd) / 256;
      perf = ((ret * 100.) / 256.);
      acc[iC] = perf;
      printf("perf=%2.2f\n\n", perf);
    }    
    
    
    int first = 1;
    double best = 0;
    int bestC;
    for (iC = 0; iC < nC; iC++) {
      if (first || acc[iC] > best) {
	first = 0;
	best = acc[iC];
	bestC = iC;
      }
    }
    printf("Using best result to test iC=%f  => Acc=%.2f...\n",
	   Cs[bestC], best);    
    
    
    // train the complete classifier on both training and validation data
    sprintf(cmd,
	    "./svm_multiclass_learn -v 0 -t 0 -l %d -c %f %s/%s %s/model%d.dat",
	    loss_func, Cs[bestC], exampleFolder, trainAllName,
	    resultsFolder, loss_func);
    printf("\n%s\n", cmd);
    ret = system(cmd);

    
    /* ************************************************************
     *
     * Test section
     * Using the above trained classifier on the test data
     *
     * ***********************************************************/

    //different evaluations metrics are reported
    // Option -r 0 =>  f1
    sprintf(cmd,
	    "./svm_multiclass_classify -v 0 -r 0 %s/%s %s/model%d.dat %s/results%d.dat",
	    exampleFolder, testName, resultsFolder, loss_func,
	    resultsFolder, loss_func);
    printf("%s\n", cmd);
    ret = system(cmd) / 256;
    double f1 = ((ret * 100.) / 256.);
    
    // Option -r 1 =>  precision
    sprintf(cmd,
	    "./svm_multiclass_classify -v 0 -r 1 %s/%s %s/model%d.dat %s/results%d.dat",
	    exampleFolder, testName, resultsFolder, loss_func,
	    resultsFolder, loss_func);
    printf("%s\n", cmd);
    ret = system(cmd) / 256;
    double pre = ((ret * 100.) / 256.);

    // Option -r 2 => recall
    sprintf(cmd,
	    "./svm_multiclass_classify -v 0 -r 2 %s/%s %s/model%d.dat %s/results%d.dat",
	    exampleFolder, testName, resultsFolder, loss_func,
	    resultsFolder, loss_func);
    printf("%s\n", cmd);
    ret = system(cmd) / 256;
    double rec = ((ret * 100.) / 256.);

    // Option -r 3 => accuracy
    sprintf(cmd,
	    "./svm_multiclass_classify -v 0 -r 3 %s/%s %s/model%d.dat %s/results%d.dat",
	    exampleFolder, testName, resultsFolder, loss_func,
	    resultsFolder, loss_func);
    printf("%s\n", cmd);
    ret = system(cmd) / 256;
    double acc = ((ret * 100.) / 256.);
    
    sprintf(cmd, "loss=%2d, C=%10.6f, F1=%2f, Pre=%2f, Rec=%2f, Acc=%2f\n",
	    loss_func, Cs[bestC], f1, pre, rec, acc);
    printf("%s", cmd);
    fputs(cmd, fp);    
  }
  
  fclose(fp);
  
  return 0;
}
