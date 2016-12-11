#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dlist.h"


dlist_list *dlist_init(unsigned int init_links, void *free_func) {

	dlist_list *list = (dlist_list *)malloc(sizeof(dlist_list));

	list->inv = (dlist_link **)calloc(1000, sizeof(dlist_link *));
	list->inv[0] = (dlist_link *)calloc(init_links, sizeof(dlist_link));

	list->avail_arr = (unsigned int **)calloc(1000,
			sizeof(unsigned int *));
	list->avail_arr[0] = (unsigned int *)calloc(init_links, sizeof(unsigned int));
	list->avail_link = -1;

	list->init_links = init_links;
	list->alloc_links = init_links;
	list->num_links = 0;

	list->top.next = NULL;

	if ( free_func != NULL )
		list->free_func = free_func;

	return list;
}


void dlist_ins(dlist_list *list, dlist_link *pos_link, void *data) {
	if ( list->num_links == list->alloc_links )
		dlist_realloc(list);

	dlist_link *new_link;

	if ( list->avail_link == -1U ) {
		new_link = &(list->inv[list->num_links / list->init_links][list->num_links % list->init_links]);
		new_link->storage_pos = list->num_links;
	}	else {
		new_link = &(list->inv[list->avail_link/list->init_links][list->avail_link % list->init_links]);
		new_link->storage_pos = list->avail_link;
		list->avail_link = list->avail_arr[list->avail_link / list->init_links][list->avail_link % list->init_links];
	}

	new_link->next = pos_link->next;
	new_link->data = data;
	pos_link->next = new_link;

	list->num_links++;

	printf("avail: %u \n", list->avail_link);
	for (unsigned int i=0;i<list->alloc_links;i++) {
		printf("%d: %u %s\n",i,
				list->avail_arr[i / list->init_links][i % list->init_links],
				(char *)(list->inv[i / list->init_links][i % list->init_links].data));
	}
}


void dlist_del(dlist_list *list, dlist_link *pos_link) {

	list->num_links--;

	if ( list->free_func != NULL )
		list->free_func(pos_link->next->data);

	list->avail_arr[pos_link->next->storage_pos	/ list->init_links]
		[pos_link->next->storage_pos % list->init_links]
		= list->avail_link;
	list->avail_link = pos_link->next->storage_pos;

	pos_link->next = pos_link->next->next;

	printf("avail: %u \n", list->avail_link);
	for (unsigned int i=0;i<list->alloc_links;i++) {
		printf("%d: %u %s\n",i,
				list->avail_arr[i / list->init_links][i % list->init_links],
				(char *)(list->inv[i / list->init_links][i % list->init_links].data));
	}
}


void *dlist_get(dlist_link *pos_link) {

	return pos_link->next->data;
}


dlist_link *dlist_nxt(dlist_link *pos_link) {

	return pos_link->next;
}


void dlist_destroy(dlist_list *list) {

	for ( unsigned int i=1;i < (list->num_links / list->alloc_links);i++) {

		if ( list->free_func != NULL ) {
			for ( dlist_link *pos = list->inv[list->num_links % list->alloc_links];
					dlist_exist(pos); pos = dlist_nxt(pos))
				list->free_func(pos->next->data);
		}
		free(list->inv[list->num_links / list->alloc_links]);
		free(list->avail_arr[list->num_links / list->alloc_links]);
	}
	free(list->inv);
	free(list->avail_arr);
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


void dlist_realloc(dlist_list *list) {

	//for (unsigned int i=0;i<list->alloc_links+1;i++) {
	//	printf("%d: %p -> %p\n",i,list->inv[i+1], list->inv[i].next);
	//}	

	//list->inv = realloc(list->inv,
	//		(list->init_links + list->alloc_links + 1) * sizeof(dlist_link));
	//list->avail_arr = realloc(list->avail_arr,
	//		(list->init_links + list->alloc_links + 1) * sizeof(unsigned int));

	list->inv[list->num_links / list->init_links]
		= (dlist_link *)calloc(list->init_links, sizeof(dlist_link));

	list->avail_arr[list->num_links / list->init_links]
		= (unsigned int *)calloc(list->init_links, sizeof(unsigned int));

	//list->inv[list->alloc_links-1].next = &(list->inv[list->alloc_links+1]);
	//list->inv->next->next->next = &(list->inv[list->alloc_links+1]);
	//printf("list->inv->next->next->next:   %p\n",&list->inv->next->next->next);
	//list->inv[3].next = &list->inv[4];

	list->alloc_links = list->init_links + list->alloc_links;

	printf("list->alloc_links: %d\nlist->init_links: %d\n", list->alloc_links, list->init_links);
	//for (unsigned int i=0;i<list->alloc_links+1;i++) {
	//	printf("%d: %p -> %p\n",i,&(list->inv[i]), list->inv[i].next);
	//}	


	//list->avail_link = 0;
}


