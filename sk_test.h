/*
 * sk_test.h
 *
 *  Created on: 26 Sep 2017
 *      Author: Mohamed Mansour
 */

#ifndef SK_TEST_H_
#define SK_TEST_H_

#define LIST_GET_HEAD(x) x->head
#define LIST_GET_NEXT(x) x->p_next

typedef struct linked_list_node{
	void* p_next;
	char* data;
}NODE;

typedef struct linked_list{
	NODE* head;
	NODE* tail;
}LIST;


int list_append(LIST* list , const char* str);

void list_free(LIST* list);

#endif /* SK_TEST_H_ */
