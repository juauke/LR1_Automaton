#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#include "automaton.h"
#include "automaton_uichar.h"
#include "automaton_file.h"

/*  @requires: paut is a valid adress to an automaton type, printed is an array of boolean_t and first is the current position in the alphabet of the automaton;
	@assigns: tags as printed the characters already displayed for this edge between the two states;
	@ensures: prints a synthetic array of char for all the characters associated with the edge; */
void makeCharacterListWithSameAction(automaton_t* paut, boolean_t* printed, int first, state_t cur_state, state_t shift_state) {
	unsigned int i, same;
	unsigned int prec_same_i;
	unsigned int nbsame;
	action_t cur_action;

	printCharacterDOT(first) ;
	cur_action=automaton_action(paut,cur_state,first);
	printed[cur_state*automaton_nb_characters(paut)+first]=1;
	for (i=first+1, prec_same_i=first, nbsame=0 ; i<automaton_nb_characters(paut); i++) {
		switch(automaton_action(paut,cur_state,i)) {
			case ACCEPT:
			case REDUCE:
				same=((automaton_action(paut,cur_state,i)) == cur_action);

				break;		
			case SHIFT:
				same= ((automaton_action(paut,cur_state,i) == cur_action) &&
					(automaton_shift(paut,cur_state,i) == shift_state)) ;
				break;		
			default:
				same=0;
				break;		
		}
		if (prec_same_i == (i-1)) {
			if (same) nbsame++;
			else if (nbsame>0) {
				if (nbsame>1) printf("-") ; else printf(",");
				printCharacterDOT(prec_same_i) ;
			}
		} else {
			if (same) {printf(","); printCharacterDOT(i);}
		}
		if (same) {printed[cur_state*automaton_nb_characters(paut)+i]=1; prec_same_i=i;}
		else nbsame=0;
	}
	if (same) {
		if (nbsame>1) printf("-"); else printf(",");
		printCharacterDOT(prec_same_i) ;
	} 
}

void DOTaut(automaton_t *paut)  {
	uichar_t i;
	state_t j;
	boolean_t* alreadyPrinted;

	if (paut == NULL) return;

	alreadyPrinted=calloc(sizeof(boolean_t),automaton_nb_states(paut)*automaton_nb_characters(paut));
	printf("digraph DOTaut {\n");
	printf(" start [ style=invis ];\n");
	printf(" start -> Q0 [ color=black];\n");
	printf(" Accepted [shape=none, fontcolor=green];\n");
	for( j=0; j < automaton_nb_states(paut); j++) {
		for (i=0; i<automaton_nb_characters(paut); i++) {
			if (alreadyPrinted[j*automaton_nb_characters(paut)+i]==1) continue;
			switch (automaton_action(paut,j,i)) {
				case REJECT:
					break;
				case ACCEPT:
					printf("   Q%d -> Accepted [ color=green, fontcolor=green, label = \"", j);
					makeCharacterListWithSameAction(paut, alreadyPrinted, i, j,0);
					printf("\"];\n");
					break;
				case SHIFT:
					printf("   Q%d -> Q%d [ color=black,  fontcolor=black,label = \"", j , automaton_shift(paut,j,i));
					makeCharacterListWithSameAction(paut, alreadyPrinted, i,j,automaton_shift(paut,j,i));
					printf("\"];\n");
					break;
				case REDUCE:
					printf("   \"(%d, %c)\" [shape=none];\n", automaton_reduce_n(paut,j), automaton_reduce_c(paut,j));
					printf("   Q%d -> \"(%d, %c)\" [ color=royalblue1, fontcolor=royalblue1, label = \"", j, automaton_reduce_n(paut,j), automaton_reduce_c(paut,j));
					makeCharacterListWithSameAction(paut,alreadyPrinted, i, j, 0);
					printf("\"];\n");
					break;
				default:
					break;
			}
		}
	}
	for(j=0; j < automaton_nb_states(paut); j++) {
		for (i=0; i<automaton_nb_characters(paut); i++) {
			if (automaton_goto(paut,j,i) != AUTOMATON_UNKNOWN_STATE) {
				printf("   Q%d -> Q%d [ color=red, fontcolor=red, label = \"", j, automaton_goto(paut,j,i));
				printCharacterDOT(i) ;
				printf("\"];\n");
			}
		}
	}

	printf("}\n");
	free(alreadyPrinted);
}
