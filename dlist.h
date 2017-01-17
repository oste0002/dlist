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
 * Return:	A pointer to the link that holds the inserted data.
 * Note:		The memory is copied from the provided data pointer into internal
 *					memory. Therefore, the dlist does not keep track of the original
 *					memory address.
 */
dlist_link *dlist_ins(dlist_list *list, void *data);


/* DLIST_DEL - Deletes a link from the list.
 *
 *	dlist_list *list				-	A pointer to the list which hold the link that will
 *													be deleted.
 *	dlist_link *link				- A pointer to the link that will be deleted. This
 *													pointer may be collected from 'dlist_ins()',
 *													'dlist_lookup()', 'dlist_next()', 'dlist_prev()' or
 *													'dlist_circ()'.
 */
void dlist_del(dlist_list *list, dlist_link *link);


/* DLIST_MTF - Move To Front of list.
 *
 *	dlist_list *list		- A pointer to the list.
 *	dlist_list *link		- A pointer to the link to be moved to front.
 *
 * Return:	A pointer to the link that is moved to front.
 */
dlist_link *dlist_mtf(dlist_list *list, dlist_link *link);


/* DLIST_GET - Get the value of a specific link
 *
 *	dlist_link *link	-	The link that contains data.
 *
 * Return:	A pointer to a memory segment that holds the inserted data.
 * Note:		The returned pointer is not the same pointer as was inserted.
 */
void *dlist_get(dlist_link *link);


/* DLIST_GET_ID - Get the id of a link if the list is indexed.
 *
 *	dlist_link *link	- A pointer to a link of an indexed list.
 *
 * Return:
 *	uint32_t		SUCCESS:	ID >= 1, corresponding to the inserted link pointer.
 *							FAILURE:	0, This may occur if the link is not indexed.
 */
uint32_t dlist_get_id(dlist_link *link);


/* DLIST_LOOKUP - Searches for a link pointer by its unique ID. The 
 *
 *	dlist_list *list	-	A list pointer.
 *	uint32_t id				-	An ID of a link that is present in the list.
 *
 * Return:
 *		NULL					-	There is no pointer int the list that	owns the id, or the
 *										list is not indexed.
 *	  dlist_list *	-	The pointer to the link which owns the provided id.
 */
dlist_link *dlist_lookup(dlist_list *list, uint32_t id);


/* DLIST_NEXT - Next link in list.
 *
 *	dlist_link *link	-	A pointer to a link.
 *
 * Return:	The link next of the input link. Or NULL if the link is last in list.
 */
dlist_link *dlist_next(dlist_link *link);


/* DLIST_PREV - Previous link in list.
 *
 *	dlist_link *link	-	A pointer to a link.
 *
 * Return:	The link previous to the input link. Or NULL if the link is first
 *					in list.
 */
dlist_link *dlist_prev(dlist_link *link);


/* DLIST_CIRC - Same as 'DLIST_NEXT()' except that the first link is returned
 * if the input link is the last link of the list.
 *
 *	dlist_list *list	-	A pointer to the list which holds the link.
 *	dlist_link *link	-	A pointer to a link.
 *
 * Return:	The link pointer next of the input link. Or the first link pointer
 *					of 'list' if the input link is last in list.
 */
dlist_link *dlist_circ(dlist_list *list, dlist_link *link);


/* DLIST_LINKS - The number of inserted links in a list.
 *
 *	dlist_list *list	-	A list pointer.
 *
 * Return: The number of inserted links.
 */
uint32_t dlist_links(dlist_list *list);


/* DLIST_DESTROY - Destroy the list. Should be called when the list is not used
 * any longer.
 *
 *	dlist_list *list	-	A list pointer.
 */
void dlist_destroy(dlist_list *list);


/* DLIST_TOP - The address of the first pointer of the list.
 *
 *	dlist_list *list	-	A list pointer
 *
 * Return: A pointer of the first link in the list. May be NULL if the list
 * is empty.
 */
dlist_link *dlist_top(dlist_list *list);


/* DLIST_EXIST - Tests if a link exists or not. This function should only be
 * used directly on the return of 'DLIST_NEXT()' and 'DLIST_PREV()' functions.
 *
 *	dlist_link *link	-	A pointer to the link which will be tested.
 *
 * Return:	0	-	The link does not exist.
 *					1	-	The link exists.
 *
 * Note:		This function basically tests if the link pointer is NULL or not.
 *					A pointer to something other than NULL will be returned as 1 even
 *					though it is not a valid link.
 */
int dlist_exist(dlist_link *link);


/* DLIST_IS_INDEXED - Tests if the list is indexed
 *
 *	dlist_link *link	-	A pointer to the link which will be tested.
 *
 * Return:	0	-	The list is not indexed.
 *					1	-	The list is indexed.
 */
int dlist_is_indexed(dlist_list *list);


/* DLIST_INDEX - Indexes the list
 *
 *	dlist_link *link	-	A pointer to the list which will be indexed.
 *
 * Note: Indexation of a list takes up some space, proportional to the maximum
 *			 number of links allowed in the list. This is more than, or equal to,
 *			 8 bytes per link allowed.
 */
void dlist_index(dlist_list *list);


/* DLIST_DROP_INDEX - Drops the indexing of the list in order
 *
 *	dlist_link *link	-	A pointer to the list which will be unindexed.
 */
void dlist_drop_index(dlist_list *list);





#endif
