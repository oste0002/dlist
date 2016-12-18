#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "dlist.h"
#include "psutils.h"

#define STRING_LEN 20

void print_menu();

int main() {

	dlist_list *l = dlist_init(4U, free);
	dlist_link *p = NULL;
	bool loop = true;
	char c;
	char *s;
	int i, j;


	print_menu();
	while(loop == true) {
		pgetc(&c);

		switch(c) {

			/* Insert */
			case '1' :
				printf("Insert: ");
				s = (char *) malloc(sizeof(char)*STRING_LEN);
				pgets(s, sizeof(char)*STRING_LEN);
				printf("\n");
				dlist_ins(l, dlist_top(l), s);
				break;

				/* Remove */
			case '2' :
				printf("Remove: ");
				while (pgetd(&j) != 0 || j <= 0)
					printf("INVALID INPUT: Please try again\n");
				printf("Removing %d\n",j);

				for (p=dlist_top(l),i=1;
						(!dlist_end(p) && (j>i));
						p=dlist_nxt(p),i++);

				if (!dlist_end(p))
					dlist_del(l,p);
				break;

				/* List */
			case '3' :
				printf("\n");
				i=1;
				for (p=dlist_top(l);!dlist_end(p);p=dlist_nxt(p)) {
					printf("%d: %s\n",i,(char *)dlist_get(p));
					i++;
				}
#ifdef DLIST_DEBUG
				dlist_debug(l);
#endif
				break;

				/* Move to front */
				/*
			case '4' :
				printf("Move to front: ");
				while (pgetd(&j) != 0 || j <= 0)
					printf("INVALID INPUT: Please try again\n");

				for (p=dlist_first(l),i=1;
						(j>i) && (!dlist_isEnd(p));
						p=dlist_next(p),i++);

				if (!dlist_isEnd(p))
					dlist_moveToFront(l,p);

				break;
				*/

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




void print_menu() {
	printf("\n");
	printf("1: Insert\n");
	printf("2: Remove\n");
	printf("3: List\n");
	printf("4: Move to front\n");
	printf("0: Quit\n");
}
