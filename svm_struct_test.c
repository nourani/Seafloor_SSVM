/*
 * svm_struct_test.cpp
 *
 *  Created on: Aug 22, 2013
 *      Author: navid
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define nC 1
#define ng 1
double Cs[] = {100};//{ 1e-2, 1e-1, 1e+0, 1e+1, 1e+2 };
double gs[] = { 1e-2, 1e-1, 1e+0, 1e+1, 1e+2, 1e+3, 1e+4 };


void print(double acc[][ng]) {
	int ig, iC;

	printf("\n    C/g ");
	for (ig = 0; ig < ng; ig++)
		printf("%7.1e ", gs[ig]);
	printf("\n");
	for (iC = 0; iC < nC; iC++) {
		printf("%7.1e ", Cs[iC]);
		for (ig = 0; ig < ng; ig++) {
			printf("%7.1f ", acc[iC][ig]);
		}
		printf("\n");
	}
	printf("\n");
}

int main(int argc, char ** argv) {

	int loss_func = 0, loss_func_stop = 0;

	char cmd[256];
	int ret = 0;
	double acc[nC][ng];
	double perf;
	int measure = 0; // 0=f1, 1=avg pre, 2=avg rec, 3=acc

	memset(acc, 0, sizeof(int) * nC * ng);

	char * exampleFolder;
	if (argc > 3) {
		loss_func_stop = atoi(argv[3]);
		loss_func = atoi(argv[2]);
	} else if (argc > 2) {
		loss_func = atoi(argv[2]);
		loss_func_stop = loss_func;
	}
	if (argc > 1) {
		size_t len = strlen(argv[1]);
		exampleFolder = (char *) malloc(sizeof(char) * len);
		strcpy(exampleFolder, argv[1]);
	} else {
		printf("Usage: \n\t%s data_dir [loss] [loss]\n", argv[0]);
		exit(1);
	}
	FILE * fp = fopen("results.txt", "a");
	if (fp == NULL) {
		printf("Error opening restuls.txt");
		exit(1);
	}

	char trainName[] = "train.dat";
	char trainAllName[] = "trainAll.dat";
	char validName[] = "valid.dat";
	char testName[] = "test.dat";

	fprintf(fp, "# data folder: %s\n", exampleFolder);
	fprintf(fp, "#\tfiles: %s, %s, %s, %s\n", trainName, validName,
			trainAllName, testName);
	printf("data folder: %s\n", exampleFolder);
	printf("\tfiles: %s, %s, %s, %s\n", trainName, validName, trainAllName,
			testName);
	/** ************************************************************
	 *
	 * Train section
	 *
	 */
	for (; loss_func <= loss_func_stop; loss_func++) {
		printf("Using loss function %d\n", loss_func);
		int bestC;
#if 1
		int ig, iC;
		if ( nC > 1) {
			for (iC = 0; iC < nC; iC++) {
				for (ig = 0; ig < ng; ig++) {

					// train
					sprintf(cmd,
							"./svm_multiclass_learn -v 0 -t 0 -l %d -c %f %s/%s %s/model%d.dat",
							loss_func, Cs[iC], exampleFolder, trainName,
							exampleFolder, loss_func);
					printf("\n%s\n", cmd);
					ret = system(cmd);

					// verify
					sprintf(cmd,
							"./svm_multiclass_classify -v 0 -r %d %s/%s %s/model%d.dat %s/results%d.dat",
							measure, exampleFolder, validName, exampleFolder,
							loss_func, exampleFolder, loss_func);
					printf("%s\n", cmd);
					ret = system(cmd) / 256;
					perf = ((ret * 100.) / 256.);
					acc[iC][ig] = perf;
					printf("perf=%2.2f\n\n", perf);
				}
			}
			print(acc);

			int bestg, first = 1;
			double best = 0;
			for (iC = 0; iC < nC; iC++) {
				for (ig = 0; ig < ng; ig++) {
					if (first || acc[iC][ig] > best) {
						first = 0;
						best = acc[iC][ig];
						bestC = iC;
						bestg = ig;
					}
				}
			}
			printf("Using best result to test iC=%f, ig=%f => Acc=%.2f...\n",
					Cs[bestC], gs[bestg], best);

		} else {
			bestC = 0;
		}

		// train the complete classifier on both test and validation data
		sprintf(cmd,
				"./svm_multiclass_learn -v 0 -t 0 -l %d -c %f %s/%s %s/model%d.dat",
				loss_func, Cs[bestC], exampleFolder, trainAllName,
				exampleFolder, loss_func);
		printf("\n%s\n", cmd);
		ret = system(cmd);


#else
		bestC = 4;
#endif

#if 1
		/** ************************************************************
		 *
		 * Test section
		 * Using the above trained classifier on the test data
		 *
		 */
		sprintf(cmd,
				"./svm_multiclass_classify -v 0 -r 0 %s/%s %s/model%d.dat %s/results%d.dat",
				exampleFolder, testName, exampleFolder, loss_func,
				exampleFolder, loss_func);
		printf("%s\n", cmd);
		ret = system(cmd) / 256;
		double f1 = ((ret * 100.) / 256.);
		sprintf(cmd,
				"./svm_multiclass_classify -v 0 -r 1 %s/%s %s/model%d.dat %s/results%d.dat",
				exampleFolder, testName, exampleFolder, loss_func,
				exampleFolder, loss_func);
		printf("%s\n", cmd);
		ret = system(cmd) / 256;
		double pre = ((ret * 100.) / 256.);
		sprintf(cmd,
				"./svm_multiclass_classify -v 0 -r 2 %s/%s %s/model%d.dat %s/results%d.dat",
				exampleFolder, testName, exampleFolder, loss_func,
				exampleFolder, loss_func);
		printf("%s\n", cmd);
		ret = system(cmd) / 256;
		double rec = ((ret * 100.) / 256.);
		sprintf(cmd,
				"./svm_multiclass_classify -v 0 -r 3 %s/%s %s/model%d.dat %s/results%d.dat",
				exampleFolder, testName, exampleFolder, loss_func,
				exampleFolder, loss_func);
		printf("%s\n", cmd);
		ret = system(cmd) / 256;
		double acc = ((ret * 100.) / 256.);

		sprintf(cmd, "loss=%2d, C=%10.6f, F1=%2f, Pre=%2f, Rec=%2f, Acc=%2f\n",
				loss_func, Cs[bestC], f1, pre, rec, acc);
		printf("%s", cmd);
		fputs(cmd, fp);

#endif
	}

	fclose(fp);

	return 0;
}

// FLAT classifier BIOTA/SPONGES/ALGAE
//    C/g 1.0e-03 1.0e-02 1.0e-01 1.0e+00 1.0e+01 1.0e+02 1.0e+03
//1.0e-02      33      33      33      33      33      40      56
//1.0e-01      33      33      33      33      33      40      56
//1.0e+00      33      33      46      46      46      40      56
//1.0e+01      33      46      46      46      50      65      56
//1.0e+02      46      46      51      51      63      65      71
//1.0e+03      33      55      55      58      61      63      68
//1.0e+04      55      43      53      60      68      68      66
// STRUCTURED classifier
//    C/g 1.0e-03 1.0e-02 1.0e-01 1.0e+00 1.0e+01 1.0e+02 1.0e+03
//1.0e-02      33      33      33      33      33      40      56
//1.0e-01      33      33      33      33      33      40      56
//1.0e+00      33      33      33      33      33      40      56
//1.0e+01      33      33      33      33      33      33      56
//1.0e+02      33      33      33      33      33      33      33
//1.0e+03      33      33      33      33      33      33      33
//1.0e+04      33      33      33      33      33      33      33
