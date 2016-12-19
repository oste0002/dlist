#ifndef PREALLOC_H
#define PREALLOC_H

#include <stdbool.h>
#include <sys/types.h>

typedef struct Alloc_Cell {
	unsigned int place[2];
	unsigned int next_avail[2];
	bool is_used;
	void *data;
} prealloc_cell;


typedef struct Alloc_Head {
	prealloc_cell **inv;
	void **data;
	unsigned int full_next[2];
	unsigned int avail_cell[2];
	unsigned int init_cells;
	unsigned int alloc_cells;
	unsigned int max_cells;
	unsigned int num_cells;
	size_t cell_size;
} prealloc_head;



prealloc_head *prealloc_init(unsigned int alloc_size, unsigned int max_size,
		size_t cell_size);

prealloc_cell *prealloc_new(prealloc_head *head);

int prealloc_realloc(prealloc_head *head);

void *prealloc_memget(prealloc_cell *cell);

void prealloc_del(prealloc_head *head, prealloc_cell *cell);

void prealloc_destroy(prealloc_head *head);




#endif
