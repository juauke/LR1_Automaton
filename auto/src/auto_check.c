#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#include "auto.h"
#include "auto_color.h"

typedef struct {
	state_t s;
} heap_element_t ;

#define MAX_HEAP 256

int isword(aut_t *paut, char *str) {
	int i, ix;
	int heapix;
	heap_element_t heap[MAX_HEAP];

	state_t ns;
	char reduit_n;
	char reduit_alpha;

	ix=0;
	heapix=0;
	heap[heapix].s=0;
	while (1) {
		switch(action(paut,heap[heapix].s,str[ix])) {
			case REJECT:
				printf("   Word rejected => \""ANSI_COLOR_GREEN"%.*s"ANSI_COLOR_RED, ix,str);
				if (isprint(str[ix]))
					printf("%c", str[ix]);
				else if (str[ix] == '\n') 
					printf("\\n");
				else
					printf("0x%02x", str[ix]);
				printf(ANSI_COLOR_YELLOW);
//				if (ix <= (int)strlen(str))
//					printf("%s",&str[ix+1]);
				for (i=ix+1; i<(int)strlen(str) ; i++) {
					if (isprint(str[i]))
						printf("%c", str[i]);
					else if (str[i] == '\n')
						printf("\\n");
					else
						printf("\\x%02x", str[i]);
				}
				printf("\"\n");
				printf(ANSI_COLOR_GREEN"                     ");
				for (i=0; i<ix; i++) printf("_");
				printf(ANSI_COLOR_RED"^\n"ANSI_COLOR_RESET);
				return (1);
				break;
			case ACCEPT:
				printf("   Word accepted => \""ANSI_COLOR_GREEN);
				for (i=0; i<(int)strlen(str) ; i++) {
					if (isprint(str[i]))
						printf("%c", str[i]);
					else if (str[i] != '\n')
						printf("\\x%02x", str[i]);
				}
				printf(ANSI_COLOR_RESET"\"\n");
				return(0);
				break;
			case DECALE: 
				ns=decale(paut, heap[heapix].s,str[ix]);
				heap[++heapix].s=ns;
				ix++;
				break;
			case REDUCE:
				reduit_n=paut->reduit_n[(int)heap[heapix].s];
				reduit_alpha= paut->reduit_c[(int)heap[heapix].s];
				heapix-=reduit_n;
				ns=branch(paut,heap[heapix].s,reduit_alpha);
				heap[++heapix].s=ns;
				break;
		}
	}	
}
