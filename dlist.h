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
	struct Dlist_Link *prev;
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
	dlist_link *top;
	prealloc_head *p_head_list;
	uint32_t init_links;
	uint32_t max_links;
	uint32_t hash_links;
	size_t data_size;
	bool hash_is_active;
	dlist_hash *hash_pool;
};

/* DLIST_INIT - Initializes a linked list of preallocated items. The list will
 * automatically extend if needed.
 *
 *	uint32_t init_links		-	Number of links that is allocated on list
 *													initialization and list extension.
 *	uint32_t max_link			-	Number of links that the list is guranteed to	be able
 *													to extend into.
 *	ssize_t data_size			-	Size of the largest amount of data that could be
 *													inserted into	the list.
 *
 * Return:	A pointer to the list.
 */
dlist_list *dlist_init(uint32_t init_links,	uint32_t max_links,
		size_t data_size);

/* DLIST_INS - Inserts data into the list the. The size of the data to be
 * inserted into the list cannot exceed 'data_size' bytes, which is specified on
 * list initialization.
 *
 *	dlist_list *list			- A pointer to the list which the data will be
 *													inserted into.
 *	void *data						-	A pointer to the data which will be inserted.
 *
 *	Return:	A pointer to the link of the list which holds the inserted data.
 */
dlist_link *dlist_ins(dlist_list *list, void *data);

/* DLIST_DEL - Deletes a link from the list.
 *
 * dlist_list *list				-	A pointer to the list which hold the link that will
 *													be deleted.
 * dlist_link *link				- A pointer to the link that will be deleted. This
 *													pointer may be collected from 'dlist_ins()',
 *													'dlist_lookup()', 'dlist_next()', 'dlist_prev()' or
 *													'dlist_circ()'.
 */
void dlist_del(dlist_list *list, dlist_link *link);

dlist_link *dlist_mtf(dlist_list *list, dlist_link *link);

void *dlist_get(dlist_link *link);

uint32_t dlist_get_id(dlist_link *link);

dlist_link *dlist_next(dlist_link *link);

dlist_link *dlist_prev(dlist_link *link);

dlist_link *dlist_circ(dlist_list *list, dlist_link *link);

uint32_t dlist_links(dlist_list *list);

void dlist_destroy(dlist_list *list);

dlist_link *dlist_top(dlist_list *list);

int dlist_exist(dlist_link *link);

int dlist_is_indexed(dlist_list *list);

void dlist_index(dlist_list *list);

void dlist_drop_index(dlist_list *list);

dlist_link *dlist_lookup(dlist_list *list, hash_id id);





#endif
