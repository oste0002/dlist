#ifndef DLIST_H
#define DLIST_H

#include "prealloc.h"

typedef struct Dlist_Carrier dlist_carrier;
typedef struct Dlist_Link dlist_link;
typedef struct Dlist_List dlist_list;



struct Dlist_Link {
	struct Dlist_Link *next;
	prealloc_cell *p_cell;
	void *data;
};

struct Dlist_Carrier {
	dlist_link link;
	void *data;
};

struct Dlist_List{
	dlist_link top;
	prealloc_head *p_head;
	size_t data_size;
};



dlist_list *dlist_init(unsigned int init_links,
		unsigned int max_links, size_t data_size);

int dlist_ins(dlist_list *list, void *data);

void dlist_del(dlist_list *list, dlist_link *pos_link);

void dlist_mtf(dlist_list *list, dlist_link *pos_link);

void *dlist_get(dlist_link *pos_link);

dlist_link *dlist_next(dlist_link *pos_link);

void dlist_destroy(dlist_list *list);

dlist_link *dlist_top(dlist_list *list);

int dlist_end(dlist_link *pos_link);

int dlist_exist(dlist_link *pos_link);



#endif

