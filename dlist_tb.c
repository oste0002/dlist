#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "dlist.h"
#include "psutils.h"

#define STRING_LEN 20


void dlist_debug(dlist_list *list);
void print_menu();

int main() {

	bool loop = true;
	char c;
	char s[STRING_LEN];
	int i, j;
	dlist_list *l = dlist_init(4U, 12U, sizeof(s));
	dlist_link *p = NULL;


	print_menu();
	while(loop == true) {
		pgetc(&c);

		switch(c) {

			/* Insert */
			case '1' :
				printf("Insert: ");
				pgets(s, sizeof(char)*STRING_LEN);
				printf("\n");
				if ( dlist_ins(l, dlist_top(l), s) != 0 ) {
					fprintf(stderr,"Could not insert into dlist\n");
					exit(EXIT_FAILURE);
				}
				break;

				/* Remove */
			case '2' :
				printf("Remove: ");
				while (pgetd(&j) != 0 || j <= 0)
					printf("INVALID INPUT: Please try again\n");
				printf("Removing %d\n",j);

				for (p=dlist_top(l),i=1;
						(!dlist_end(p) && (j>i));
						p=dlist_next(p),i++);

				if (!dlist_end(p))
					dlist_del(l,p);
				break;

				/* List */
			case '3' :
				printf("\n");
				i=1;
				for (p=dlist_top(l);!dlist_end(p);p=dlist_next(p)) {
					printf("%d: %s\n",i,(char *)dlist_get(p));
					i++;
				}
#ifdef DLIST_DEBUG
				dlist_debug(l);
#endif
				break;

				/* Move to front */
			case '4' :
				printf("Move to front: ");
				while (pgetd(&j) != 0 || j <= 0)
					printf("INVALID INPUT: Please try again\n");

				for (p=dlist_top(l),i=1;
						(j>i) && (!dlist_end(p));
						p=dlist_next(p),i++);

				if (!dlist_end(p))
					dlist_mtf(l,p);

				break;

			case '5' :
				printf("Insert a number larger than 0\n");
				while (pgetd(&j) != 0 || j <= 0) {
					printf("INVALID INPUT: Please try again\n");
				}
				printf("Perfect! : %d\n",j);
				break;

				/* Quit */
			case '0' :
				loop = false;
				break;

			default :
				print_menu();
		}


	}
	dlist_destroy(l);
	exit(EXIT_SUCCESS);
}


void dlist_debug(dlist_list *list) {
  prealloc_cell p_cell;
  unsigned int i,x,y;

  printf("\nnum_cells: %d\n",list->p_head->num_cells);
  printf("avail: [%d %d]\n",
      list->p_head->avail_cell[1],list->p_head->avail_cell[0]);
  for (i=0;i<list->p_head->alloc_cells;i++) {
    x = i % list->p_head->init_cells;
    y = i / list->p_head->init_cells;
    p_cell = list->p_head->inv[y][x];
    printf("[%d %d]: %s\t", y, x, (char *)((dlist_link *)p_cell.data)->data);
		printf("avail_next: [%d %d]\t",p_cell.next_avail[1],p_cell.next_avail[0]);
		printf("addr: %p\n",((dlist_link *)p_cell.data)->data);
  }
}


void print_menu() {
	printf("\n");
	printf("1: Insert\n");
	printf("2: Remove\n");
	printf("3: List\n");
	printf("4: Move to front\n");
	printf("0: Quit\n");
}
