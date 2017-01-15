#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <stdint.h>
#include <time.h>
#include <sys/time.h>

#include "dlist.h"
#include "prealloc.h"

#define NEW_HASH_MAX_ITERATIONS 1000

/* INTERNAL FUNCTION DEFINITIONS */
static int32_t intern_new_hash(dlist_list *list, dlist_link *link);
static int32_t intern_del_hash(dlist_list *list, hash_id *id);
static int32_t intern_hash_exists(const dlist_list *list,
		const hash_id id, uint32_t *index);
static int32_t intern_avail_hash(const dlist_list *list, hash_id *id,
		uint32_t *index);


dlist_list *dlist_init(uint32_t init_links,
		uint32_t max_links, size_t data_size) {

	size_t carrier_size = sizeof(dlist_link) + data_size;

	dlist_list *list = (dlist_list *)calloc(1,sizeof(dlist_list));
	list->p_head_list = prealloc_init(init_links, max_links, carrier_size);
	list->top.next = NULL;
	list->init_links = init_links;
	list->max_links = max_links;
	list->hash_links = max_links*2;

	list->data_size = data_size;
	list->hash_is_active = false;
	return list;
}


dlist_link *dlist_ins(dlist_list *list, void *data) {
	prealloc_cell *p_cell;
	dlist_link *link;
	dlist_linkCarrier *carrier;

	if ( (p_cell = prealloc_new(list->p_head_list)) == NULL )
		return(NULL);

	carrier = (dlist_linkCarrier *)prealloc_memget(p_cell);
	link = &carrier->link;
	link->data = &carrier->data;
	link->p_cell = p_cell;
	link->next = list->top.next;
	if (link->next != NULL)
		link->next->prev = link;
	link->prev = &list->top;
	memcpy(link->data,data,list->data_size);
	list->top.next = link;

	if ( list->hash_is_active == true )
		if (intern_new_hash(list, link) == 0 )
			fprintf(stderr,"Error: Hash table is full!\n");

	return(link);
}


void dlist_del(dlist_list *list, dlist_link *link) {

	if ( list->hash_is_active == true ) {
		intern_del_hash(list, link->next->id);
		link->next->id = NULL; }

	prealloc_del(list->p_head_list, link->next->p_cell);
	link->next = link->next->next;
	if (link->next != NULL)
		link->next->prev = link;
	return;
}


dlist_link *dlist_mtf(dlist_list *list, dlist_link *link) {
	dlist_link *link2 = link->next;

	if (link->next->next != NULL)
		link->next->next->prev = link;
	link->next = link->next->next;


	link2->next = list->top.next;
	if (link2->next != NULL)
		link2->next->prev = link2;

	link2->prev = &list->top;
	list->top.next = link2;

	return(link2);
}


void *dlist_get(dlist_link *link) {
	return(((link != NULL) && (link->next != NULL)) ? link->next->data : NULL);
}


uint32_t dlist_get_id(dlist_link *link) {
	return(link->next->id != NULL ? *link->next->id : 0);
}


dlist_link *dlist_next(dlist_link *link) {
	return(link != NULL ? link->next : NULL);
}


dlist_link *dlist_prev(dlist_link *link) {
	return(link != NULL ? link->prev : NULL);
}


dlist_link *dlist_circ(dlist_list *list, dlist_link *link) {
	return(link != NULL ? link->next : &list->top);
}


uint32_t dlist_links(dlist_list *list) {
	return(list->p_head_list->num_cells);
}


void dlist_destroy(dlist_list *list) {
	prealloc_destroy(list->p_head_list);
	if (list->hash_is_active == true)
		free(list->hash_pool);
	free(list);
	return;
}


dlist_link *dlist_top(dlist_list *list) {
	return(&list->top);
}


int dlist_end(dlist_link *link) {
	return(link->next== NULL);
}


int dlist_exist(dlist_link *link) {
	return(link != NULL);
}


int dlist_is_indexed(dlist_list *list) {
	return(list->hash_is_active == true ? 1 : 0);
}


void dlist_index(dlist_list *list) {
	dlist_link *link;

	if (list->hash_is_active == false)
		list->hash_pool = (dlist_hash *)calloc(list->hash_links,
				sizeof(dlist_hash));
	else
		memset(list->hash_pool, 0, list->hash_links * sizeof(dlist_hash));

	for (link=dlist_top(list);!dlist_end(link);link=dlist_next(link))
		if ( intern_new_hash(list, link->next) == 0 )
			fprintf(stderr,"Error: Hash table is full!\n");

	list->hash_is_active = true;
	return;
}


void dlist_drop_index(dlist_list *list) {
	dlist_link *link;

	if (list->hash_is_active == true) {
		free(list->hash_pool);
		list->hash_is_active = false; }

	for (link=dlist_top(list);!dlist_end(link);link=dlist_next(link))
		link->next->id = NULL;

	list->hash_is_active = false;
	return;
}


dlist_link *dlist_lookup(dlist_list *list, hash_id id) {
	uint32_t index;

	if (list->hash_is_active != true)
		return(NULL);

	intern_hash_exists(list, id, &index);
	return(index != 0U ? list->hash_pool[index].ptr->prev : NULL);
}



/* INTERNAL FUNCTIONS */


/* intern_new_hash
 * Initializes hash for the dlist_link that 'link' points at.
 */
static int32_t intern_new_hash(dlist_list *list, dlist_link *link) {
	dlist_hash *hash;
	hash_id id = 0;
	struct timeval tim;
	uint32_t index;
	int i=0;

	gettimeofday(&tim, NULL);
	srand(tim.tv_usec | tim.tv_sec);
	id = rand();
	if ( (i = intern_avail_hash(list, &id, &index)) == -1 )
		return(-1);

	hash = &list->hash_pool[index];
	hash->id = id;
	hash->ptr = link;
	link->id = &hash->id;
#ifdef DLIST_DEBUG
	printf("Hash iterations: %u\n", i);
	printf("id: %u\tindex: %u\n", *link->id, index);
#endif
	return(i);
}


static int32_t intern_del_hash(dlist_list *list, hash_id *id) {
	uint32_t index;

	if ( intern_hash_exists(list, *id, &index) == 0 )
		return(1);
	list->hash_pool[index].ptr = NULL;
	list->hash_pool[index].id = 0;
	return(0);
}


/* intern_hash_exists
 *
 * Return:	0		- Hash does not exist
 *					1		- Hash exists
 */
static int32_t intern_hash_exists(const dlist_list *list, const hash_id id,
		uint32_t *index) {

	*index = id % list->hash_links;
	if ( list->hash_pool[*index].id != id ) {
		*index = 0;
		return(1); }
	else
		return(0);
}


/* intern_avail_hash
 * Finds a id and corresponding hash index, runs at most
 * NEW_HASH_MAX_ITERATIONS times.
 *
 * Return:	0		- Hash does not exist
 *					i		- Number of iteratins performed in order to find available hash
 */
static int32_t intern_avail_hash(const dlist_list *list, hash_id *id,
		uint32_t *index) {
	uint32_t i;

	for (*index = *id % list->hash_links, i=1;
			(list->hash_pool[*index].id != 0U || *index == 0) &&
			i<NEW_HASH_MAX_ITERATIONS;
			*index = *id % list->hash_links, i++ )
		*id = rand_r(id);

	if (i >= NEW_HASH_MAX_ITERATIONS) {
		id = NULL;
		index = NULL;
		return(-1); }
	else
		return(i);
}





