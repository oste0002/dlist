#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dlist.h"
#include "prealloc.h"



dlist_list *dlist_init(unsigned int init_links,
		unsigned int max_links, size_t data_size) {

	dlist_list *list = (dlist_list *)calloc(1,sizeof(dlist_list));

	list->p_head = prealloc_init(init_links, max_links,
			sizeof(dlist_link)+data_size);
	list->top.next = NULL;

	list->data_size = data_size;

	return list;
}


dlist_link *dlist_ins(dlist_list *list, void *data) {

	prealloc_cell *p_cell;
	dlist_link *link;
	dlist_carrier *carrier;

	if ( (p_cell = prealloc_new(list->p_head)) == NULL )
		return(NULL);
	carrier = (dlist_carrier *)prealloc_memget(p_cell);
	link = &carrier->link;
	link->data = &carrier->data;
	memcpy(link->data,data,list->data_size);
	link->p_cell = p_cell;

	link->next = list->top.next;
	list->top.next = link;
	return(link);
}


void dlist_del(dlist_list *list, dlist_link *pos_link) {

	prealloc_del(list->p_head, pos_link->next->p_cell);
	pos_link->next = pos_link->next->next;
}


dlist_link *dlist_mtf(dlist_list *list, dlist_link *pos_link) {

	dlist_link *link = pos_link->next;

	pos_link->next = pos_link->next->next;
	link->next = list->top.next;
	list->top.next = link;
	return(link);
}

void *dlist_get(dlist_link *pos_link) {

	return pos_link->next->data;
}


dlist_link *dlist_next(dlist_link *pos_link) {

	return pos_link->next;
}


void dlist_destroy(dlist_list *list) {

	prealloc_destroy(list->p_head);
	free(list);
}


dlist_link *dlist_top(dlist_list *list) {

	return &list->top;
}


int dlist_end(dlist_link *pos_link) {

	return(pos_link->next== NULL);
}


int dlist_exist(dlist_link *pos_link) {

	return(pos_link != NULL);
}

