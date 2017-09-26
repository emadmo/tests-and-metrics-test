/*
 * sk_metric.c
 *
 *  Created on: 26 Sep 2017
 *      Author: Mohamed Mansour
 */
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "sk_test.h"
#include "sk_metric.h"

int sk_test(char* result , LIST* header_list , int number_of_requests){
	struct curl_slist *curl_header_list,*hosts,*curl_list_node;

	NODE* node = LIST_GET_HEAD(header_list);

	//Copy header list to cur list
	while(node != NULL){
		curl_header_list = curl_slist_append(curl_header_list , node->data);

		printf("Added %s to curl_list\n",node->data);

		node = LIST_GET_NEXT(node);
	}

	CURL *curl_hdl = curl_easy_init();
	if(curl_hdl){
		CURLcode res;
		char *ct;
		int time;
		long response_code=0,lookup_time=0;
		double mean_lookup_time=0 ;
		int i = 0;

		curl_easy_setopt(curl_hdl, CURLOPT_URL, TEST_HOST_NAME);

		//1- Resolve the Host address
		curl_easy_setopt(curl_hdl, CURLOPT_RESOLVE, hosts );

		//2- Response code
		curl_easy_setopt(curl_hdl, CURLOPT_URL, TEST_HOST_NAME);

		curl_easy_setopt(curl_hdl, CURLINFO_RESPONSE_CODE, &response_code );

		curl_easy_setopt(curl_hdl, CURLOPT_HTTPHEADER, curl_header_list );

		//3- Lookup time
		for(i=0;i<1;i++){

			curl_easy_setopt(curl_hdl, CURLINFO_NAMELOOKUP_TIME, &lookup_time );

			mean_lookup_time += lookup_time;
			printf("\nmean lookup time %f\n",mean_lookup_time);
		}
		res = curl_easy_perform(curl_hdl);

		curl_list_node = hosts;

		while(curl_list_node!=NULL){
			printf("\nresolved list %s\n",curl_list_node->data);
			curl_list_node = curl_list_node->next;
		}
		printf("res = %d",res);
		printf("\nResonse code %ld\n",response_code);

		mean_lookup_time = mean_lookup_time / (double)number_of_requests;
		printf("\nmean lookup time %f\n",mean_lookup_time);

		//res = curl_easy_getinfo(curl_hdl,CURLINFO_TOTAL_TIME, &time);
		//if((res == CURLE_OK) && time)
		//{
		//	printf("We received :%d",time);
		//}





		curl_easy_cleanup(curl_hdl);
	}

	return 0;
}

