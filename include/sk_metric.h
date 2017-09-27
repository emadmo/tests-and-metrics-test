/*
 * sk_metric.h
 *
 *  Created on: 26 Sep 2017
 *      Author: Mohamed
 */

#ifndef SK_METRIC_H_
#define SK_METRIC_H_

#define TEST_HOST_NAME "http://www.google.com/"

#ifdef SK_METRIC_LOG
#define SK_LOG(...) printf(__VA_ARGS__)
#else
#define SK_LOG(...)
#endif

#define SK_ASSERT(x,msg) if(!(x)){printf("Assert %s:%d:%s\n exit failure!\n",__FILE__,__LINE__,msg);return 1;}


#define LIST_GET_HEAD(x) x->head
#define LIST_GET_NEXT(x) x->p_next

typedef struct linked_list_node{
	void* p_next;
	char* data;
}SK_METRIC_NODE;

typedef struct linked_list{
	SK_METRIC_NODE* head;
	SK_METRIC_NODE* tail;
}SK_METRIC_LIST;

/*
 * Takes a likst of HTTP headers and number of requests
 * Calculates
 */
int sk_test(char** result , SK_METRIC_LIST* header_list , int number_of_requests);

// sk_metric_list functions
int sk_metric_list_append(SK_METRIC_LIST** list , const char* str);

void sk_metric_list_free(SK_METRIC_LIST** list);

#endif /* SK_METRIC_H_ */
