/*
 ============================================================================
 Name        : sk_test.c
 Author      : Mohamed Mansour
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <unistd.h>
#include <strings.h>
#include "sk_test.h"
#include "sk_metric.h"


void sk_test_usage(void);

int main(int argc, char *argv[]) {
	SK_METRIC_LIST* linked_list = NULL;
	int number_of_requests = 0;
	int opt;
	char *result;
	/* Test user input right number of arguments */
	if(argc <3){
		sk_test_usage();
		return 0;
	}

	result = malloc(1000);

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
			return 0;
			break;
		}
	}

	//Call My API to run the test
	sk_test(&result,linked_list,number_of_requests);

	sk_metric_list_free(&linked_list);

	printf("\nTest results for sk_mertic:\n==================");
	printf("%s\n",result);

	return EXIT_SUCCESS;
}
void sk_test_usage(void){
	printf("Usage:\n"
						"-H \"Header-name: Header-value\" can be used multiple times, each time specifying an extra HTTP header to add to your request\n"
						"-n <integer>                     number of HTTP requests to make \n");
}

