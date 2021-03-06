/*
 ============================================================================
 Name        : sk_test.c
 Author      : Mohamed Mansour
 Version     :
 Copyright   : Your copyright notice
 Description : Test function for the sk_metric library
 ============================================================================
 */

#include "sk_test.h"
#include "sk_metric.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#define MAX_BUFF_SIZE 500
void sk_test_usage(void);

int main(int argc, char *argv[]) {
  SK_METRIC_LIST *linked_list = NULL;
  int number_of_requests = 0;
  int opt;
  char *result;
  SK_VERBOSE verbose = SK_DISABLE_VERBOSE;

  /* Test user input right number of arguments */
  if (argc < 3) {
    sk_test_usage();
    return EXIT_SUCCESS;
  }

  result = malloc(MAX_BUFF_SIZE);

  while ((opt = getopt(argc, argv, "H:n:v")) != -1) {
    switch (opt) {
    case 'H':
      sk_metric_list_append(&linked_list, optarg);
      break;
    case 'n':
      number_of_requests = atoi(optarg);
      printf("Number of requests %d\n", number_of_requests);
      break;
    case 'v':
      verbose = SK_ENABLE_VERBOSE;
      break;
    default:
      sk_test_usage();
      return EXIT_SUCCESS;
      break;
    }
  }

  // Call My API to run the test
  if(sk_test(&result, linked_list, number_of_requests, verbose) == EXIT_FAILURE){
	  printf("\nsk_metric_test failed !\n");
	  return EXIT_FAILURE;
  }

  printf("\nTest results for sk_mertic:\n"
         "===========================\n");
  printf("%s\n", result);

  sk_metric_list_free(&linked_list);

  return EXIT_SUCCESS;
}
void sk_test_usage(void) {
  printf("Usage: skTest [options ...]\n"
         "Options:\n"
         "\t-H \"Header-name: Header-value\"\tcan be used multiple times, each "
         "time specifying an extra HTTP header to add to your request\n"
         "\t-n <integer> \t\t\tnumber of HTTP requests to make \n"
         "\t-v            \t\t\tEnables detailed prints from HTTP requests\n");
}
