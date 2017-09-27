/*
 ============================================================================
 Name        : sk_test.c
 Author      : Mohamed Mansour
 Version     :
 Copyright   : Your copyright notice
 Description : Test function for the sk_metric library
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <unistd.h>
#include <strings.h>
#include "sk_test.h"
#include "sk_metric.h"

#define MAX_BUFF_SIZE 500
void sk_test_usage(void);

int main(int argc, char *argv[]) {
	SK_METRIC_LIST* linked_list = NULL;
	int number_of_requests = 0;
	int opt;
	char *result;
	/* Test user input right number of arguments */
	if(argc <3){
		sk_test_usage();
		return EXIT_SUCCESS;
	}

	result = malloc(MAX_BUFF_SIZE);

	while((opt = getopt(argc, argv, "H:n:")) != -1){
		switch (opt) {
		case 'H':
			printf("received header %s\n",optarg);
			sk_metric_list_append(&linked_list , optarg);
		break;
		case 'n':
			number_of_requests = atoi(optarg);
			printf("Number of requests %d\n",number_of_requests);
			break;
		default:
			sk_test_usage();
			return EXIT_SUCCESS;
			break;
		}
	}

	//Call My API to run the test
	sk_test(&result,linked_list,number_of_requests);

	printf("\nTest results for sk_mertic:\n"
		   "\n===========================\n");
	printf("%s\n",result);

	sk_metric_list_free(&linked_list);

	return EXIT_SUCCESS;
}
void sk_test_usage(void){
	printf("Usage: skTest [options ...]\n"
			"Options:\n"
			"\t-H \"Header-name: Header-value\"\tcan be used multiple times, each time specifying an extra HTTP header to add to your request\n"
			"\t-n <integer> \t\t\tnumber of HTTP requests to make \n");
}

