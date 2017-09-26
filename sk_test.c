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

#define SK_ASSERT(x) if(!(x)){printf("Assert %d: exit failure!\n",__LINE__);return 1;}

LIST* linked_list;


int main(int argc, char *argv[]) {

	int number_of_requests = 0;
	int opt;
	char *result;
	/* Test user input right number of arguments */
	SK_ASSERT(argc >= 3);

	result = malloc(1000);
	linked_list = malloc(sizeof(NODE));
	linked_list->head = NULL;
	linked_list->tail = NULL;

	while((opt = getopt(argc, argv, "H:n:")) != -1){
		switch (opt) {
		case 'H':
			printf("received header %s\n",optarg);
			list_append(linked_list , optarg);
		break;
		case 'n':
			number_of_requests = atoi(optarg);
			printf("Number of requests %d\n",number_of_requests);
			break;
		default:
			printf("Usage:\n"
					"-H \"Header-name: Header-value\" can be used multiple times, each time specifying an extra HTTP header to add to your request\n"
					"-n <integer>                     number of HTTP requests to make \n");
			return 0;
			break;
		}
	}

	sk_test(result,linked_list,number_of_requests);
	return EXIT_SUCCESS;
}

int list_append(LIST* list , const char* str){

	SK_ASSERT(list != NULL );
	SK_ASSERT(str != NULL );

	printf("Appending %s to the list ...\n",str);

	NODE* node;
	node = malloc(sizeof(NODE));

	node->data = strdup(str);
	node->p_next = NULL;

	if(list->head == NULL){
		//first entry in the list
		list->head = node;
	}
	else{
		//not first entry in the list
		list->tail->p_next = node;
	}

	list->tail = node;

	return 0;
}
