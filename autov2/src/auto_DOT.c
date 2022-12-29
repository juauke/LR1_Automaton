#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#include "auto.h"
#include "auto_file.h"

void printCharacter(char c) {
       	if (c == ' ') printf("' '"); 
	else if (c == ',') printf("','"); 
	else if (isprint(c)) printf("%c",c);
       	else if (c == '\n') printf("'\\\\n'"); 
	else printf("'0x%02x'",c); 
}

void makeCharacterListWithSameAction(aut_t *paut, char *printed,int first, state_t cur_state, state_t decale_state) {
	int i, same;
	int prec_same_i;
	int nbsame;
	action_t cur_action;

	printCharacter(first) ;
	cur_action=action(paut,cur_state,first);
	printed[cur_state*AUT_ALPHABET_SIZE+first]=1;
	for (i=first+1, prec_same_i=first, nbsame=0 ; i<AUT_ALPHABET_SIZE;i++) {
		switch(action(paut,cur_state,i)) {
			case ACCEPT:
			case REDUCE:
				same=((action(paut,cur_state,i)) ==  cur_action);

				break;		
			case DECALE:
				same= ( (action(paut,cur_state,i) ==  cur_action) &&
					(decale(paut,cur_state,i) == decale_state) ) ;
				break;		
			default:
				same=0;
				break;		
		}
		if (prec_same_i == (i-1)) {
			if (same) nbsame++;
			else if ( nbsame>0 ) {
				if (nbsame>1) printf("-") ; else printf(",");
				printCharacter(prec_same_i) ;
			}
		} else {
			if (same) { printf(","); printCharacter(i) ;}
		}
		if (same) {
			printed[cur_state*AUT_ALPHABET_SIZE+i]=1;
			prec_same_i=i;
		} else {
			nbsame=0;
		}
	}
	if (same) {
		if (nbsame>1) printf("-") ; else printf(",");
		printCharacter(prec_same_i) ;
	} 
}

void DOTaut(aut_t *paut)  {
	int i,j;
	char *alreadyPrinted;

	if (paut == NULL) return;

	alreadyPrinted=calloc(paut->nb_states*AUT_ALPHABET_SIZE,1);
	printf("digraph DOTaut {\n");
	printf(" start [ style=invis ];\n");
	printf(" start -> Q0 [ color=black];\n");
	printf(" Accepted [shape=none, fontcolor=green];\n");
	for( j=0; j < paut->nb_states; j++) {
		for (i=0; i<AUT_ALPHABET_SIZE; i++) {
			if (alreadyPrinted[j*AUT_ALPHABET_SIZE+i]==1) continue;
			switch (paut->actions[j*AUT_ALPHABET_SIZE+i]) {
				case REJECT:
					break;
				case ACCEPT:
					printf("   Q%d -> Accepted [ color=green, fontcolor=green, label = \"", j);
					makeCharacterListWithSameAction(paut,alreadyPrinted, i, j,0);
					printf("\"];\n");
					break;
				case DECALE:
					printf("   Q%d -> Q%d [ color=black,  fontcolor=black,label = \"", j , paut->decale[j*AUT_ALPHABET_SIZE+i]);
					makeCharacterListWithSameAction(paut,alreadyPrinted, i,j,decale(paut,j,i));
					printf("\"];\n");
					break;
				case REDUCE:
					printf("   \"(%d, %c)\" [shape=none];\n", paut->reduit_n[j], paut->reduit_c[j]);
					printf("   Q%d -> \"(%d, %c)\" [ color=royalblue1, fontcolor=royalblue1, label = \"", j, paut->reduit_n[j], paut->reduit_c[j]);
					makeCharacterListWithSameAction(paut,alreadyPrinted, i, j, 0);
					printf("\"];\n");
					break;
				default:
					break;
			}
		}
	}
	for( j=0; j < paut->nb_states; j++) {
		for (i=0; i<AUT_ALPHABET_SIZE; i++) {
			if ( branch(paut,j,i) !=0 && branch(paut,j,i) < paut->nb_states ) {
				printf("   Q%d -> Q%d [ color=red, fontcolor=red, label = \"", j, branch(paut,j,i));
				//makeCharacterListWithSameAction(paut,alreadyPrinted, i, j, 0);
				printCharacter(i) ;
				printf("\"];\n");
			}
		}
	}
	printf("}\n");
	free(alreadyPrinted);
}
