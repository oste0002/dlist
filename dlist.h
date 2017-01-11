#ifndef DLIST_H
#define DLIST_H

#include <stdbool.h>
#include <limits.h>
#include <stdint.h>
#include "prealloc.h"


typedef uint32_t hash_id;

typedef struct Dlist_List dlist_list;
typedef struct Dlist_LinkCarrier dlist_linkCarrier;
typedef struct Dlist_Link dlist_link;
typedef struct Dlist_Hash dlist_hash;


struct Dlist_Link {
	struct Dlist_Link *next;
	prealloc_cell *p_cell;
	hash_id *id;
	void *data;
};

struct Dlist_LinkCarrier {
	dlist_link link;
	void *data;
};

struct Dlist_Hash {
	struct Dlist_Link *ptr;
	hash_id id;
};

struct Dlist_List{
	dlist_link top;
	prealloc_head *p_head_list;
	uint32_t init_links;
	uint32_t max_links;
	uint32_t hash_links;
	size_t data_size;
	bool hash_is_active;
	dlist_hash *hash_pool;
};


dlist_list *dlist_init(uint32_t init_links,	uint32_t max_links,
		size_t data_size);

dlist_link *dlist_ins(dlist_list *list, void *data);

void dlist_del(dlist_list *list, dlist_link *pos_link);

dlist_link *dlist_mtf(dlist_list *list, dlist_link *pos_link);

void *dlist_get(dlist_link *pos_link);

uint32_t dlist_getId(dlist_link *pos_link);

dlist_link *dlist_next(dlist_link *pos_link);

uint32_t dlist_links(dlist_list *list);

void dlist_destroy(dlist_list *list);

dlist_link *dlist_top(dlist_list *list);

int dlist_end(dlist_link *pos_link);

int dlist_exist(dlist_link *pos_link);

int dlist_isIndexed(dlist_list *list);

void dlist_index(dlist_list *list);

void dlist_dropIndex(dlist_list *list);

void *dlist_lookup(dlist_list *list, hash_id id);





#endif
