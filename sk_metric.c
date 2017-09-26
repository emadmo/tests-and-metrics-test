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

int sk_test(char* result , LIST* header_list , int number_of_requests){
	struct curl_slist *p_list;

	NODE* node = LIST_GET_HEAD(header_list);

	//Copy header list to cur list
	while(node != NULL){
		p_list = curl_slist_append(p_list , node->data);
		printf("Added %s to curl_list\n",node->data);

		node = LIST_GET_NEXT(node);
	}

	CURL *curl_hdl = curl_easy_init();
	if(curl_hdl){
		CURLcode res;
		char *ct;
		int time;

		curl_easy_setopt(curl_hdl, CURLOPT_URL, "http://google.com");
		curl_easy_setopt(curl_hdl, CURLOPT_HTTPHEADER, list );

		res = curl_easy_perform(curl_hdl);
		printf("res = %d",res);

		res = curl_easy_getinfo(curl_hdl,CURLINFO_TOTAL_TIME, &time);
		if((res == CURLE_OK) && time)
		{
			printf("We received :%d",time);
		}

		curl_easy_cleanup(curl_hdl);
	}

	return 0;
}

