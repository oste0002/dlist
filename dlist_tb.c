#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "dlist.h"
#include "psutils.h"

#define STRING_LEN 20


void print_menu();

#ifdef DLIST_DEBUG
void intern_listPrealloc(dlist_list *list);
#endif

int main() {
	bool loop = true;
	char c;
	char s[STRING_LEN];
	char *s_ptr;
	int i, j;
	dlist_list *l = dlist_init(4U, 8U, sizeof(s));
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
				if ( dlist_ins(l, s) == NULL )
					fprintf(stderr,"Could not insert into dlist\n");
				memset(s,0,sizeof(s));
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
					strncpy(s,(char *)dlist_get(p),sizeof(s));
					if (strnlen(s,sizeof(s))<5)
						printf("%d: %s\t\t id: %u\n",i,(char *)dlist_get(p), dlist_getId(p));
					else
						printf("%d: %s\t id: %u\n",i,(char *)dlist_get(p), dlist_getId(p));
					i++;
				}
#ifdef DLIST_DEBUG
				intern_listPrealloc(l);
#endif
				break;

				/* Lookup */
			case '4' :
				printf("Lookup ID: ");
				while (pgetd(&i) != 0 || i <= 0)
					printf("INVALID INPUT: Please try again\n");
				printf("\n");
				//if (memcpy(s,dlist_lookup(l, i),sizeof(s)) == NULL) {
				if ( (s_ptr = (char *)dlist_lookup(l, i)) == NULL ) {
					printf("Could not find item!\n");
					break; }

				memcpy(s,s_ptr,sizeof(s));

				printf("%s\n",s);
				memset(s,0,sizeof(s));
				break;

				/* Move to front */
			case '5' :
				printf("Move to front: ");
				while (pgetd(&j) != 0 || j <= 0)
					printf("INVALID INPUT: Please try again\n");

				for (p=dlist_top(l),i=1;
						(j>i) && (!dlist_end(p));
						p=dlist_next(p),i++);

				if (!dlist_end(p))
					dlist_mtf(l,p);

				break;

				/* Index the list */
			case '6' :
				dlist_index(l);
				break;

				/* Show if list is indexed */
			case '7' :
				if ( dlist_isIndexed(l) == 1 )
					printf("List is indexed\n");
				else
					printf("List is not indexed\n");
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


#ifdef DLIST_DEBUG
void intern_listPrealloc(dlist_list *list) {
	prealloc_cell p_cell;
	unsigned int i,x,y;

	printf("\nnum_cells: %u\n",dlist_links(list));
	printf("avail: [%d %d]\n",
			list->p_head_list->avail_cell[1],list->p_head_list->avail_cell[0]);
	for (i=0;i<list->p_head_list->alloc_cells;i++) {
		x = i % list->p_head_list->init_cells;
		y = i / list->p_head_list->init_cells;
		p_cell = list->p_head_list->inv[y][x];
		printf("[%d %d]: %s\t", y, x, (char *)((dlist_link *)p_cell.data)->data);
		printf("avail_next: [%d %d]   ",p_cell.next_avail[1],p_cell.next_avail[0]);
		printf("link addr: %p   ",(dlist_link *)p_cell.data);
		printf("string addr: %p\n",((dlist_link *)p_cell.data)->data);
	}
}
#endif


void print_menu() {
	printf("\n");
	printf("1: Insert\n");
	printf("2: Remove\n");
	printf("3: List\n");
	printf("4: Lookup\n");
	printf("5: Move to front\n");
	printf("6: Index the list\n");
	printf("7: Test if list is indexed\n");
	printf("0: Quit\n");
}
