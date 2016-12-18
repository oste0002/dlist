#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dlist.h"
#include "prealloc.h"

#ifdef DLIST_DEBUG

#include <stdio.h>

#endif




dlist_list *dlist_init(unsigned int init_links, void *free_func) {

	dlist_list *list = (dlist_list *)malloc(sizeof(dlist_list));

	list->p_head =
		prealloc_init(init_links, init_links * 1000U, sizeof(dlist_link));
	list->top.next = NULL;

	if ( free_func != NULL )
		list->free_func = free_func;

	return list;
}


void dlist_ins(dlist_list *list, dlist_link *pos_link, void *data) {

	prealloc_cell *p_cell = prealloc_new(list->p_head);
	dlist_link *new_link = prealloc_memget(p_cell);
	new_link->p_cell = p_cell;

	new_link->next = pos_link->next;
	new_link->data = data;
	pos_link->next = new_link;
}


void dlist_del(dlist_list *list, dlist_link *pos_link) {

	if ( list->free_func != NULL )
		list->free_func(pos_link->next->data);

	prealloc_del(list->p_head, pos_link->next->p_cell);
	pos_link->next = pos_link->next->next;
}


void *dlist_get(dlist_link *pos_link) {

	return pos_link->next->data;
}


dlist_link *dlist_nxt(dlist_link *pos_link) {

	return pos_link->next;
}


void dlist_destroy(dlist_list *list) {


	if ( list->free_func != NULL ) {
		for (dlist_link *link = dlist_top(list); !dlist_end(link);
				link = dlist_nxt(link))
			list->free_func(link->next->data);

	}
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

#ifdef DLIST_DEBUG
void dlist_debug(dlist_list *list) {
	void *p_data;
	unsigned int i,x,y;
	unsigned int **a = list->p_head->avail_arr;

	printf("\nnum_cells: %d\n",list->p_head->num_cells);
	printf("avail: [%d %d]\n",
			list->p_head->avail_cell[1],list->p_head->avail_cell[0]);
	for (i=0;i<list->p_head->alloc_cells;i++) {
		x = i % list->p_head->init_cells;
		y = i / list->p_head->init_cells;
		p_data = list->p_head->inv[y][x].data;
		printf("[%d %d]: %s\tavail_next: [%d %d]\n", y, x,
				((dlist_link *)p_data)->data, a[y][2*x+1], a[y][2*x]);
	}
}
#endif

