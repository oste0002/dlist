#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dlist.h"


dlist_list *dlist_init(unsigned int init_links, void *free_func) {

	dlist_list *list = (dlist_list *)malloc(sizeof(dlist_list));

	list->top = (dlist_link *)calloc(init_links+1, sizeof(dlist_link));

	list->avail_arr  = (unsigned int *)calloc(init_links+1, sizeof(unsigned int));
	list->avail_link = 0;

	list->init_links = init_links;
	list->alloc_links = init_links;
	list->num_links = 0;

	if ( free_func != NULL )
		list->free_func = free_func;

	return list;
}


void dlist_ins(dlist_list *list, dlist_link *pos_link, void *data) {
	if ( list->num_links == list->alloc_links )
		dlist_realloc(list);

	dlist_link *nxt_link = pos_link->next;
	dlist_link *new_link;

	if ( list->avail_link == 0 ) {
		new_link = &(list->top[list->num_links +1]);
		memset(new_link,0,sizeof(dlist_link));
		new_link->storage_pos = list->num_links;
	}	else {
		new_link = &(list->top[list->avail_link]);
		memset(new_link,0,sizeof(dlist_link));
		new_link->storage_pos = list->avail_link;
		list->avail_link = list->avail_arr[list->avail_link];
	}

	pos_link->next = new_link;
	new_link->next = nxt_link;
	new_link->data = data;

	list->num_links++;

	printf("avail: %u \n", list->avail_link);
	for (unsigned int i=0;i<list->alloc_links;i++) {
		printf("%d: %u %s\n",i+1, list->avail_arr[i], (char *)(list->top[i+1].data));
	}
}


void dlist_del(dlist_list *list, dlist_link *pos_link) {

	list->num_links--;

	if ( list->free_func != NULL )
		list->free_func(pos_link->next->data);

	list->avail_arr[pos_link->next->storage_pos] = list->avail_link;
	list->avail_link = pos_link->next->storage_pos;

	pos_link->next = pos_link->next->next;


	//printf("avail: %u \n", list->avail_link);
	//for (int i=0;i<5;i++) {
	//	printf("%d: %u %s\n",i+1, list->avail_arr[i+1], (char *)list->top[i+1].data);
	//}
}


void *dlist_get(dlist_link *pos_link) {

	return pos_link->next->data;
}


dlist_link *dlist_nxt(dlist_link *pos_link) {

	return pos_link->next;
}


void dlist_destroy(dlist_list *list) {

	if ( list->free_func != NULL )
		for ( dlist_link *pos = list->top; pos->next != NULL; pos = pos->next)
			list->free_func(pos->next->data);
	free(list->top);
	free(list->avail_arr);
	free(list);
}


dlist_link *dlist_top(dlist_list *list) {

	return list->top;
}


int dlist_end(dlist_link *pos_link) {

	return(pos_link->next== NULL);
}

int dlist_exist(dlist_link *pos_link) {

	return(pos_link != NULL);
}


void dlist_realloc(dlist_list *list) {

	//for (unsigned int i=0;i<list->alloc_links+1;i++) {
	//	printf("%d: %p -> %p\n",i,list->top[i+1], list->top[i].next);
	//}	

	list->top = realloc(list->top,
			(list->init_links + list->alloc_links + 1) * sizeof(dlist_link));
	list->avail_arr = realloc(list->avail_arr,
			(list->init_links + list->alloc_links + 1) * sizeof(unsigned int));

	//list->top[list->alloc_links-1].next = &(list->top[list->alloc_links+1]);
	//list->top->next->next->next = &(list->top[list->alloc_links+1]);
	//printf("list->top->next->next->next:   %p\n",&list->top->next->next->next);
	//list->top[3].next = &list->top[4];

	list->alloc_links = list->init_links + list->alloc_links;

	//for (unsigned int i=0;i<list->alloc_links+1;i++) {
	//	printf("%d: %p -> %p\n",i,&(list->top[i]), list->top[i].next);
	//}	


	//list->avail_link = 0;
}

