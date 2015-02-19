/***********************************************************************/
/*                                                                     */
/*   svm_struct_classify.c                                             */
/*                                                                     */
/*   Classification module of SVM-struct.                              */
/*                                                                     */
/*   Author: Thorsten Joachims                                         */
/*   Date: 03.07.04                                                    */
/*                                                                     */
/*   Copyright (c) 2004  Thorsten Joachims - All rights reserved       */
/*                                                                     */
/*   This software is available for non-commercial use only. It must   */
/*   not be modified and distributed without prior permission of the   */
/*   author. The author is not responsible for implications from the   */
/*   use of this software.                                             */
/*                                                                     */
/************************************************************************/

#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
#include "../svm_light/svm_common.h"
#ifdef __cplusplus
}
#endif
#include "../svm_struct_api.h"
#include "svm_struct_common.h"

char testfile[200];
char modelfile[200];
char predictionsfile[200];
int return_f1 = 0;

void read_input_parameters(int, char **, char *, char *, char *,
		STRUCT_LEARN_PARM *, long*, long *);
void print_help(void);


int num_classes = -1;
void printAdj(int adj[][num_classes]) {
	int i, j;
	int sumV[num_classes], sumH[num_classes], sum;
	printf("    ");
	for (j = 0; j < num_classes; j++) {
		sum = 0;
		for (i = 0; i < num_classes; i++) {
			sum += adj[i][j];
		}
		sumV[j] = sum;
		printf("%5d ", j);
	}
	printf(" | Rec (Tot)\n");
	printf("    ");
	for (i = 0; i < num_classes; i++) {
		printf("------");
	}
	printf("-|---------\n");

	for (i = 0; i < num_classes; i++) {
		sum = 0;
		printf(" %2d|", i);
		for (j = 0; j < num_classes; j++) {
			printf("%5d ", adj[i][j]);
			sum += adj[i][j];
		}
		sumH[i] = sum;
		printf(" | %2.2f (%d)\n", ((double) adj[i][i]) / sum, sumH[i]);
	}
	printf("----");
	for (i = 0; i < num_classes; i++) {
		printf("------");
	}
	printf("-|---------\n");

	printf("Pre|");
	for (i = 0; i < num_classes; i++) {
		if (sumV[i])
			printf(" %2.2f ", ((double) adj[i][i]) / sumV[i]);
		else
			printf("      ");
	}
	printf(" | \n");
	printf("Tot|");
	sum = 0;
	for (i = 0; i < num_classes; i++) {
		printf("%5d ", sumV[i]);
		sum += sumV[i];
	}
	printf(" | %d\n", sum);
}

void calcAverageClassX(int adj[][num_classes], double *p, double *r, double *a) {
	double sumVTot = 0, sumHTot = 0;
	int i, j, num_class = 0;
	long sumV = 0, sumH = 0, sumDiag = 0, sumTot = 0;

	// precision is calculated as percentage of correctly retrieved data
	// recall is calculated as percentage of possible retrieved data
	// accuracy is calculated as percentage of true over all

	for (i = 0; i < num_classes; i++) {
		sumH = 0;
		sumV = 0;

		for (j = 0; j < num_classes; j++) {
			// Calculate total number of class instances
			sumH += adj[i][j];
			// Calculate total number of predictions
			sumV += adj[j][i];
		}
		// Only calculate average for classes that have instances
		if (sumH > 0) {
			sumHTot += (double) (adj[i][i]) / sumH;
			num_class++;
		}
		if (sumV > 0) {
			sumVTot += (double) (adj[i][i]) / sumV;
		}

		// totals
		sumTot += sumH;
		sumDiag += adj[i][i];
	}
	*p = (double) (sumHTot) / num_class;
	*r = (double) (sumVTot) / num_class;
	*a = (double) (sumDiag) / sumTot;
	printf("acc=%f, avg p=%f, avg r=%f\n", *a, *p, *r);
}

int main(int argc, char* argv[]) {
	long correct = 0, incorrect = 0, no_accuracy = 0;
	long i;
	double t1, runtime = 0;
	double avgloss = 0, l;
	FILE *predfl;
	STRUCTMODEL model;
	STRUCT_LEARN_PARM sparm;
	STRUCT_TEST_STATS teststats;
	SAMPLE testsample;
	LABEL yP, yT;

	svm_struct_classify_api_init(argc, argv);

	read_input_parameters(argc, argv, testfile, modelfile, predictionsfile,
			&sparm, &verbosity, &struct_verbosity);
	svm_struct_init_hierarchy(&sparm);

	if (struct_verbosity >= 1) {
		printf("Reading model...");
		fflush(stdout);
	}
	model = read_struct_model(modelfile, &sparm);
	if (struct_verbosity >= 1) {
		fprintf(stdout, "done.\n");
	}

	if (model.svm_model->kernel_parm.kernel_type == LINEAR) { /* linear kernel */
		/* compute weight vector */
		add_weight_vector_to_linear_model(model.svm_model);
		model.w = model.svm_model->lin_weights;
	}

	if (struct_verbosity >= 1) {
		printf("Reading test examples...\n");
		fflush(stdout);
	}
	testsample = read_struct_examples(testfile, &sparm);
	if (struct_verbosity >= 1) {
		printf("done.\n");
		fflush(stdout);
	}

	if (struct_verbosity >= 1) {
		printf("Classifying test examples...\n");
		fflush(stdout);
	}

	if ((predfl = fopen(predictionsfile, "w")) == NULL ) {
		perror(predictionsfile);
		exit(1);
	}

	weighted_loss = 0;
	num_classes = hierarchy_get_num_classes(sparm.hierarchy);
	int adj[num_classes][num_classes], tpT = 0, fpT = 0, fnT = 0, tnT = 0;
	memset(adj, 0, sizeof(int)*num_classes*num_classes);
	int class_instances[num_classes];
	memset(class_instances, 0, sizeof(int)*num_classes);
	int max_of_each = 65536;

	for (i = 0; i < testsample.n; i++) {
		t1 = get_runtime();
		// yP is prediction, yT is true
		yP = classify_struct_example(testsample.examples[i].x, &model, &sparm);
		yT = testsample.examples[i].y;
		runtime += (get_runtime() - t1);

		if( ++class_instances[yT.label] > max_of_each )
			continue;

		int pC = 0, tC = 0, j;
		for (j = 0; j < sparm.num_classes; j++) {
			if (yP.label == (j + 1))
				pC = j + 1;
			if (yT.label == (j + 1))
				tC = j + 1;
		}
		if (struct_verbosity >= 2) {
			printf("%d (%d) <> %d (%d)\n", yT.label, tC, yP.label, pC);
		}
		adj[tC][pC]++;

		int tp=0, fp=0, fn=0, tn=0;
		hierarchy_get_stats(sparm.hierarchy, yT.label, yP.label, &tp, &fp, &fn, &tn);
		tpT+=tp;
		fpT+=fp;
		fnT+=fn;
		tnT+=tn;

		write_label(predfl, yP);
		l = loss(yT, yP, &sparm);
		avgloss += l;
		if (l == 0) {
			correct++;
		} else {
			incorrect++;
		}
		eval_prediction(i, testsample.examples[i], yP, &model, &sparm,
				&teststats);

		if (empty_label(testsample.examples[i].y)) {
			no_accuracy = 1;
		} /* test data is not labeled */
		if (struct_verbosity >= 2) {
			if ((i + 1) % 100 == 0) {
				printf("%ld..", i + 1);
				fflush(stdout);
			}
		}
		free_label(yP);
		free_label(yT);
	}
	avgloss /= testsample.n;
	fclose(predfl);

	if (struct_verbosity >= 0) {
		printf("done\n");
		printf("Runtime (without IO) in cpu-seconds: %.2f\n",
				(float) (runtime / 100.0));
	}
	if ((!no_accuracy) && (struct_verbosity >= 1)) {
		printf("Average loss on test set: %.4f\n", (float) avgloss);
		printf(
				"Zero/one-error on test set: %.2f%% (%ld correct, %ld incorrect, %d total)\n",
				(float) 100.0 * incorrect / testsample.n, correct, incorrect,
				testsample.n);
	}
	print_struct_testing_stats(testsample, &model, &sparm, &teststats);
	free_struct_sample(testsample);
	free_struct_model(model);

	svm_struct_classify_api_exit();

	double f1 = 2. * tpT / (2. * tpT + fpT + fnT);
	printf("F1-score on test set: %4f\n", f1);
	double p, r, a;
	calcAverageClassX(adj, &p, &r, &a);
	int ret;
	switch (return_f1) {
	case 1:
		ret = (int) round(256 * p);
		break;
	case 2:
		ret = (int) round(256 * r);
		break;
	case 3:
		ret = (int) round(256 * a);
		break;
	default:
		ret = (int) round(256 * f1);
		break;
	}
	printAdj(adj);
	return (ret);
}

void read_input_parameters(int argc, char *argv[], char *testfile,
		char *modelfile, char *predictionsfile, STRUCT_LEARN_PARM *struct_parm,
		long *verbosity, long *struct_verbosity) {
	long i;

	/* set default */
	strcpy(modelfile, "svm_model");
	strcpy(predictionsfile, "svm_predictions");
	(*verbosity) = 0;/*verbosity for svm_light*/
	(*struct_verbosity) = 1; /*verbosity for struct learning portion*/
	struct_parm->custom_argc = 0;

	for (i = 1; (i < argc) && ((argv[i])[0] == '-'); i++) {
		switch ((argv[i])[1]) {
		case 'h':
			print_help();
			exit(0);
		case '?':
			print_help();
			exit(0);
		case '-':
			strcpy(struct_parm->custom_argv[struct_parm->custom_argc++],
					argv[i]);
			i++;
			strcpy(struct_parm->custom_argv[struct_parm->custom_argc++],
					argv[i]);
			break;
		case 'v':
			i++;
			(*struct_verbosity) = atol(argv[i]);
			break;
		case 'y':
			i++;
			(*verbosity) = atol(argv[i]);
			break;
		case 'r':
			i++;
			(return_f1) = atol(argv[i]);
			break;
		default:
			printf("\nUnrecognized option %s!\n\n", argv[i]);
			print_help();
			exit(0);
		}
	}
	if ((i + 1) >= argc) {
		printf("\nNot enough input parameters!\n\n");
		print_help();
		exit(0);
	}
	strcpy(testfile, argv[i]);
	strcpy(modelfile, argv[i+1]);
	if ((i + 2) < argc) {
		strcpy(predictionsfile, argv[i+2]);
	}

	parse_struct_parameters_classify(struct_parm);
}

void print_help(void) {
	printf("\nSVM-struct classification module: %s, %s, %s\n", INST_NAME,
			INST_VERSION, INST_VERSION_DATE);
	printf("   includes SVM-struct %s for learning complex outputs, %s\n",
			STRUCT_VERSION, STRUCT_VERSION_DATE);
	printf("   includes SVM-light %s quadratic optimizer, %s\n", VERSION,
			VERSION_DATE);
	copyright_notice();
	printf(
			"   usage: svm_struct_classify [options] example_file model_file output_file\n\n");
	printf("options: -h         -> this help\n");
	printf("         -v [0..3]  -> verbosity level (default 2)\n\n");

	print_struct_help_classify();
}

