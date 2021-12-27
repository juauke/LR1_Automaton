#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#include "auto.h"
#include "auto_file.h"
void printCharacter(char c) {
	if (isprint(c)) printf("%c",c); else printf("0x%02x",c); 
}
char *makeCharacterListWithSameAction(aut_t *paut, int first, state_t cur_state, state_t decale_state) {
	int i, same;
	int prec_same_i;
	int nbsame;
	action_t cur_action;

	printCharacter(first) ;
	cur_action=action(paut,cur_state,first);
	for (i=first+1, prec_same_i=first ; i<AUT_ALPHABET_SIZE;i++) {
	printf("%c => %c cur=%d = act=%d\n",first,i,cur_action, action(paut,cur_state,i)) ;
		switch(action(paut,cur_state,i)) {
			case ACCEPT:
			case REDUCE:
				same=((action(paut,cur_state,i) ==  cur_action));
				break;		
			case DECALE:
				same= ( (action(paut,cur_state,i) ==  cur_action) &&
					(decale(paut,cur_state,i) == decale_state) ) ;
				break;		
			default:
				break;		
		}
		if (prec_same_i == (i-1)) {
			if (!same) {
				if (nbsame>1) printf("-") ; else printf(",");
					printCharacter(prec_same_i) ;
			}
		} else {
			if (same) printCharacter(i) ;
		}
		if (same) {
			nbsame++;
		} else 
			nbsame=0;
		prec_same_i=i;
	}
	if (same) {
		if (nbsame>1) printf("-") ; else printf(",");
		printCharacter(prec_same_i) ;
	} 
}

void DOTaut(aut_t *paut)  {
	int i,i2,j;
	state_t cur_s;

	if (paut == NULL) return;

	printf("digraph DOTaut {\n");
	printf(" Accepted [shape=none, fontcolor=green];\n");
	for( j=0; j < paut->nb_states; j++) {
		for (i=0; i<AUT_ALPHABET_SIZE; i++) {
			switch (paut->actions[j*AUT_ALPHABET_SIZE+i]) {
				case REJECT:
					break;
				case ACCEPT:
					for (i2=1; paut->actions[j*AUT_ALPHABET_SIZE+i+i2] == ACCEPT ; i2++);
					printf("   Q%d -> Accepted [ color=green, fontcolor=green, label = \"", j);
					makeCharacterListWithSameAction(paut, i, action(paut,j,i),0);
					// if (isprint(i)) printf("%c",i); else printf("0x%02x",i); 
					// if (i2>1) {
					//	if (i2>2) printf("-");
					//	if (i2==2) printf(" ");
					//	if (isprint(i+i2-1)) printf("%c",i+i2-1) ; else printf("0x%02x",i+i2-1);
					//}
					printf("\"];\n");
					i+=i2-1;
					break;
				case DECALE:
					for (i2=0, cur_s=paut->decale[j*AUT_ALPHABET_SIZE+i];
					     (paut->actions[j*AUT_ALPHABET_SIZE+i+i2] == DECALE ) && ( paut->decale[j*AUT_ALPHABET_SIZE+i+i2] == cur_s) ; i2++);
					printf("   Q%d -> Q%d [ color=black,  fontcolor=black,label = \"", j , paut->decale[j*AUT_ALPHABET_SIZE+i]);
					makeCharacterListWithSameAction(paut, i, action(paut,j,i), decale(paut,j,i));
					// if (isprint(i)) printf("%c",i); else printf("0x%02x",i); 
					// if (i2>1) {
					//	if (i2>2) printf("-");
					//	if (i2==2) printf(" ");
					//	if (isprint(i+i2-1)) printf("%c",i+i2-1) ; else printf("0x%02x",i+i2-1);
					// }
					printf("\"];\n");
					i+=i2-1;
					break;
				case REDUCE:
					makeCharacterListWithSameAction(paut, i, action(paut,j,i), 0);
					for (i2=1; paut->actions[j*AUT_ALPHABET_SIZE+i+i2] == REDUCE ; i2++);
					printf("   \"(%d, %c)\" [shape=none];\n", paut->reduit_n[j], paut->reduit_c[j]);
					printf("   Q%d -> \"(%d, %c)\" [ color=royalblue1, fontcolor=royalblue1, label = \"", j, paut->reduit_n[j], paut->reduit_c[j]);
					// if (isprint(i)) printf("%c",i); else printf("0x%02x",i); 
					// if (i2>1) {
					//	if (i2>2) printf("-");
					//	if (i2==2) printf(" ");
					//	if (isprint(i+i2-1)) printf("%c",i+i2-1) ; else printf("0x%02x",i+i2-1);
					// }
					printf("\"];\n");
					i+=i2-1;
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
				if (isprint(i)) printf("%c",i); else printf("0x%02x",i); 
				printf("\"];\n");
			}
		}
	}
	printf("}\n");
}
