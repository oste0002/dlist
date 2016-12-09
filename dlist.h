#ifndef DLIST_H
#define DLIST_H

typedef struct Dlist_Link {
	struct Dlist_Link *next;
	void *data;
	unsigned int storage_pos;
} dlist_link;

typedef struct Dlist_List{
	dlist_link *top;
	unsigned int *avail_arr;
	unsigned int avail_link;
	unsigned int init_links;
	unsigned int alloc_links;
	unsigned int num_links;
	void (*free_func)(void *data);
} dlist_list;




dlist_list *dlist_init(unsigned int init_links, void *free_func);

void dlist_ins(dlist_list *list, dlist_link *pos_link, void *data);

void dlist_del(dlist_list *list, dlist_link *pos_link);

void *dlist_get(dlist_link *pos_link);

dlist_link *dlist_nxt(dlist_link *pos_link);

void dlist_destroy(dlist_list *list);

dlist_link *dlist_top(dlist_list *list);

int dlist_end(dlist_link *pos_link);

int dlist_exist(dlist_link *pos_link);

void dlist_realloc(dlist_list *list);


#endif

