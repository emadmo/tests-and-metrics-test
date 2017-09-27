/*
 * sk_metric.h
 *
 *  Created on: 26 Sep 2017
 *      Author: Mohamed Mansour
 */

#ifndef SK_METRIC_H_
#define SK_METRIC_H_

#define TEST_HOST_NAME "http://www.google.com/"

#ifdef SK_METRIC_LOG
#define SK_LOG(...)                                                            \
  printf("\n%s:%d:\t", __FUNCTION__, __LINE__);                                \
  printf(__VA_ARGS__)
#else
#define SK_LOG(...)
#endif

#define SK_ASSERT(x, msg)                                                      \
  if (!(x)) {                                                                  \
    printf("Assert %s:%d:%s\n exit failure!\n", __FILE__, __LINE__, msg);      \
    return 1;                                                                  \
  }

#define LIST_GET_HEAD(x) x->head
#define LIST_GET_NEXT(x) x->p_next

typedef struct linked_list_node {
  void *p_next;
  char *data;
} SK_METRIC_NODE;

typedef struct linked_list {
  SK_METRIC_NODE *head;
  SK_METRIC_NODE *tail;
} SK_METRIC_LIST;

typedef enum sk_verbose {
  SK_DISABLE_VERBOSE = 0,
  SK_ENABLE_VERBOSE = 1
} SK_VERBOSE;

/*
 * Takes a list of HTTP headers and number of requests and calculates :
 * - IP address of Server
 * - HTTP response code
 * - median of Name lookup time
 * - median of start transfer time
 * - median of total time
 *
 * arguments
 *  result                   - Pointer to a string that will be filled with the calculated parameters
 *  results header_list      - Linked list of type SK_METRIC_LIST contains the
 *                             HTTP headers
 *  number_of_requests       - Number of times the requests will be sent affecting
 *                             the calculated means
 * verbose                   - If set to SK_ENABLE_VERBOSE , details about the HTTP
 *                             operations will be printed IF set
 *                             to SK_DISABLE_VERBOSE nothing will be printed
 */
int sk_test(char **result, SK_METRIC_LIST *header_list, int number_of_requests,
            SK_VERBOSE verbose);

// sk_metric_list functions
/*
 * Append passed string to a passed list. Will create the list if not already
 * allocated
 * arguments list      - Pointer to the list to be appended/created
 *  str       - string to be added in the appended list node
 */
int sk_metric_list_append(SK_METRIC_LIST **list, const char *str);

/*
 * Frees the allocated list and all the nodes inside
 * arguments
 *  list      - Pointer to the list to be freed
 */
void sk_metric_list_free(SK_METRIC_LIST **list);

#endif /* SK_METRIC_H_ */
