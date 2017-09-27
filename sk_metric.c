/*
 * sk_metric.c
 *
 *  Created on: 26 Sep 2017
 *      Author: Mohamed Mansour
 */
#include "sk_metric.h"
#include "sk_test.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

int sk_test(char **result, SK_METRIC_LIST *header_list, int number_of_requests,
            SK_VERBOSE verbose) {

  struct curl_slist *curl_header_list, *hosts, *curl_list_node;
  char *host_ip;
  char *p_result = *result;
  int len = 0;
  long response_code = 0;
  double mean_lookup_time = 0, mean_connect_time = 0,
         mean_start_transfer_time = 0, mean_total_time = 0;

  // Validate inputs
  SK_ASSERT(p_result != NULL, "Invalid result buff");
  SK_ASSERT(header_list != NULL, "Invalid HTTP Header");
  SK_ASSERT(number_of_requests > 0, "Number of requests can't be 0");

  SK_METRIC_NODE *node = LIST_GET_HEAD(header_list);

  // Copy header list to cur list
  while (node != NULL) {
    curl_header_list = curl_slist_append(curl_header_list, node->data);

    SK_LOG("Added %s to curl_list", node->data);

    node = LIST_GET_NEXT(node);
  }

  CURL *curl_hdl = curl_easy_init();

  if (curl_hdl) {
    CURLcode res;
    double lookup_time = 0, connect_time = 0, start_transfer_time = 0,
           total_time = 0;
    int i = 0;

    // First step to set the options for the curl req
    curl_easy_setopt(curl_hdl, CURLOPT_URL, TEST_HOST_NAME);
    curl_easy_setopt(curl_hdl, CURLOPT_HTTPHEADER, curl_header_list);
    // Set curl lib verbose output
    curl_easy_setopt(curl_hdl, CURLOPT_NOBODY, !verbose);
    curl_easy_setopt(curl_hdl, CURLOPT_VERBOSE, verbose);

    if (curl_easy_perform(curl_hdl) == CURLE_OK) {

      // 1- Resolve the Host address TODO
      if (curl_easy_getinfo(curl_hdl, CURLINFO_PRIMARY_IP, &host_ip) ==
          CURLE_OK) {
        SK_LOG("IP Address %s", host_ip);
      }

      // 2- Response code
      if (curl_easy_getinfo(curl_hdl, CURLINFO_RESPONSE_CODE, &response_code) ==
          CURLE_OK) {
        SK_LOG("Resonse code %ld", response_code);
      }
    }
    for (i = 0;
         i < number_of_requests && (curl_easy_perform(curl_hdl) == CURLE_OK);
         i++) {
      // 3- Lookup time
      if (curl_easy_getinfo(curl_hdl, CURLINFO_NAMELOOKUP_TIME, &lookup_time) ==
          CURLE_OK) {
        mean_lookup_time += lookup_time;
        SK_LOG("iteration %d:Lookup time %f", i, lookup_time);
      }

      // 4- Connect time
      if (curl_easy_getinfo(curl_hdl, CURLINFO_CONNECT_TIME, &connect_time) ==
          CURLE_OK) {
        mean_connect_time += connect_time;
        SK_LOG("iteration %d:Connect time %f", i, connect_time);
      }

      // 5- Start Transfer time
      if (curl_easy_getinfo(curl_hdl, CURLINFO_STARTTRANSFER_TIME,
                            &start_transfer_time) == CURLE_OK) {
        mean_start_transfer_time += start_transfer_time;
        SK_LOG("iteration %d:Start transfer time %f", i, start_transfer_time);
      }

      // 6- Total Time
      if (curl_easy_getinfo(curl_hdl, CURLINFO_TOTAL_TIME, &total_time) ==
          CURLE_OK) {
        mean_total_time += total_time;
        SK_LOG("iteration %d:Total time %f", i, total_time);
      }
    }

    // Calculate means
    mean_connect_time /= (double)number_of_requests;
    mean_lookup_time /= (double)number_of_requests;
    mean_start_transfer_time /= (double)number_of_requests;
    mean_total_time /= (double)number_of_requests;

    SK_LOG("mean_connect_time = %f", mean_connect_time);
    SK_LOG("mean_lookup_time = %f", mean_lookup_time);
    SK_LOG("mean_start_transfer_time = %f", mean_start_transfer_time);
    SK_LOG("mean_total_time = %f", mean_total_time);

    curl_easy_cleanup(curl_hdl);
  }

  /*Append the results in the SK_TEST format as following
   *SKTEST;<IP address of HTTP server>;<HTTP response code>;<median of
   *CURLINFO_NAMELOOKUP_TIME>; <median of CURLINFO_CONNECT_TIME>;<median of
   *CURLINFO_STARTTRANSFER_TIME>; <median of CURLINFO_TOTAL_TIME>
   */
  len += sprintf(p_result + len, "%s;", "SK_TEST");
  len += sprintf(p_result + len, "%s;", host_ip);
  len += sprintf(p_result + len, "%ld;", response_code);
  len += sprintf(p_result + len, "%f;", mean_lookup_time);
  len += sprintf(p_result + len, "%f;", mean_connect_time);
  len += sprintf(p_result + len, "%f;", mean_start_transfer_time);
  len += sprintf(p_result + len, "%f;", mean_total_time);

  return EXIT_SUCCESS;
}

int sk_metric_list_append(SK_METRIC_LIST **list, const char *str) {
  SK_ASSERT(str != NULL, "Empty string");
  SK_METRIC_NODE *node;

  if (*list == NULL) {
    // Create the list
    *list = malloc(sizeof(SK_METRIC_NODE));
    (*list)->head = NULL;
    (*list)->tail = NULL;
  }

  SK_LOG("Appending %s to the list ...", str);

  node = malloc(sizeof(SK_METRIC_NODE));

  node->data = strdup(str);
  node->p_next = NULL;

  if ((*list)->head == NULL) {
    // first entry in the list
    (*list)->head = node;
  } else {
    // not first entry in the list
    (*list)->tail->p_next = node;
  }

  (*list)->tail = node;

  return EXIT_SUCCESS;
}

void sk_metric_list_free(SK_METRIC_LIST **list) {
  SK_METRIC_NODE *node, *next_node;

  node = (*list)->head;
  while (node != NULL) {
    free(node->data);
    next_node = node->p_next;
    free(node);
    node = next_node;
  }
  free(*list);
}
