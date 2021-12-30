#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#include "automaton.h"
#include "automaton_uichar.h"
#include "automaton_color.h"
#include "automaton_file.h"

typedef struct {
	state_t s;
} heap_element_t ;

#define MAX_HEAP 256

void dumpAutomaton(automaton_t *paut) {
	int j;
	uichar_t i;

	printf("Automaton [%s], nb_states=%d, nb_characters=%d\n",
			automaton_filename(paut),
			automaton_nb_states(paut),
			automaton_nb_characters(paut));
	printf("========== actions =============\n");
	for (j=0; j<automaton_nb_states(paut);j++) {
		printf("%d|",j);
		for (i=0; i<automaton_nb_characters(paut);i++) {
			if (automaton_action(paut,j,i) != REJECT) printf("%02x=%0x,",i,automaton_action(paut,j,i));
		}
		printf("\n");
	}
	printf("========== reduit =============\n");
	for (j=0; j<automaton_nb_states(paut);j++) {
		printf("%d|",j);
		if (automaton_reduit_n(paut,j) != AUTOMATON_UNKNOWN_REDUIT_N) printf("(%d,%02x)",automaton_reduit_n(paut,j),automaton_reduit_c(paut,j));
		printf("\n");
	}
	printf("========== decale =============\n");
	for (j=0; j<automaton_nb_states(paut);j++) {
		printf("%d|",j);
		for (i=0; i<automaton_nb_characters(paut);i++) {
			if (automaton_decale(paut,j,i) != AUTOMATON_UNKNOWN_STATE) printf("%02x=>%02x,",i,automaton_decale(paut,j,i));
		}
		printf("\n");
	}
	printf("========== branch =============\n");
	for (j=0; j<automaton_nb_states(paut);j++) {
		printf("%d|",j);
		for (i=0; i<automaton_nb_characters(paut);i++) {
			if (automaton_branch(paut,j,i) != AUTOMATON_UNKNOWN_STATE) printf("%02x=>%02x,",i,automaton_branch(paut,j,i));
		}
		printf("\n");
	}
}

int isword(automaton_t *paut, uichar_t *str) {
	int i, ix;
	int heapix;
	heap_element_t heap[MAX_HEAP];

	state_t ns;
	action_t act;
	char reduit_n;
	char reduit_alpha;

	ix=0;
	heapix=0;
	heap[heapix].s=0;
	while (1) {
		
		if (str[ix] > automaton_nb_characters(paut) )
			act=REJECT;
		else
			act=automaton_action(paut,heap[heapix].s,str[ix]);
		
// printf("action=%d,ix=%d", act, ix);
//		for (i=0; i<heapix;i++) {
//			printf("heap[%d]=%d,", i, heap[i].s);
//		}
//		printf("\n");

		switch(act) {
			case REJECT:
				printf("   Word "BOLDRED"rejected"ANSI_COLOR_RESET" => \""ANSI_COLOR_GREEN);
				for (i=0; i<ix; i++) 
					printCharacter(str[i]);
				printf(ANSI_COLOR_RED);
				printCharacter(str[ix]);
				printf(ANSI_COLOR_YELLOW);
				for (i=ix+1; i<(int)uic_strlen(str); i++) 
					printCharacter(str[i]);
				printf(ANSI_COLOR_RESET"\"\n");
				printf(ANSI_COLOR_GREEN"                     ");
				for (i=0; i<ix; i++) printf("_");
				printf(ANSI_COLOR_RED"^\n"ANSI_COLOR_RESET);
				return (1);
				break;
			case ACCEPT:
				printf("   Word "BOLDGREEN"accepted"ANSI_COLOR_RESET" => \""ANSI_COLOR_GREEN);
				for (i=0; i<(int)uic_strlen(str) ; i++) 
					if (isprint(str[i])) printCharacter(str[i]);
				printf(ANSI_COLOR_RESET"\"\n");
				return(0);
				break;
			case DECALE: 
				ns=automaton_decale(paut, heap[heapix].s,str[ix]);
				heap[++heapix].s=ns;
				ix++;
				break;
			case REDUCE:
				reduit_n=automaton_reduit_n(paut,(int)heap[heapix].s);
				reduit_alpha= automaton_reduit_c(paut,(int)heap[heapix].s);
				heapix-=reduit_n;
				ns=automaton_branch(paut,heap[heapix].s,reduit_alpha);
				heap[++heapix].s=ns;
				break;
		}
	}	
}
