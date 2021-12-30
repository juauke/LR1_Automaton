#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#include "auto.h"
#include "auto_file.h"

void DOTaut(aut_t *paut)  {
	int i,i2,j;
	state_t cur_s;

	if (paut == NULL) {perror("nothing-to-print"); return;}

	printf("digraph DOTaut {\n");
	printf(" Accepted [shape=none, fontcolor=green];\n");
	for( j=0; j < paut->nb_states; j++) {
		for (i=0; i < AUT_ALPHABET_SIZE; i++) {
			switch (paut->actions[j*AUT_ALPHABET_SIZE+i]) {
				case REJECT:
					break;
				case ACCEPT:
					for (i2=1; paut->actions[j*AUT_ALPHABET_SIZE+i+i2] == ACCEPT ; i2++);
					printf("   Q%d -> Accepted [ color=green, fontcolor=green, label = \"", j);
					if (isprint(i)) printf("%c",i); else printf("0x%02x",i); 
					if (i2>1) {
						if (i2>2) printf("-");
						if (i2==2) printf(" ");
						if (isprint(i+i2-1)) printf("%c",i+i2-1) ; else printf("0x%02x",i+i2-1);
					}
					printf("\"];\n");
					i+=i2-1;
					break;
				case DECALE:
					for (i2=0, cur_s=paut->decale[j*AUT_ALPHABET_SIZE+i];
					     (paut->actions[j*AUT_ALPHABET_SIZE+i+i2] == DECALE ) && ( paut->decale[j*AUT_ALPHABET_SIZE+i+i2] == cur_s) ; i2++);
					printf("   Q%d -> Q%d [ color=black,  fontcolor=black,label = \"", j , paut->decale[j*AUT_ALPHABET_SIZE+i]);
					if (isprint(i)) printf("%c",i); else printf("0x%02x",i); 
					if (i2>1) {
						if (i2>2) printf("-");
						if (i2==2) printf(" ");
						if (isprint(i+i2-1)) printf("%c",i+i2-1) ; else printf("0x%02x",i+i2-1);
					}
					printf("\"];\n");
					i+=i2-1;
					break;
				case REDUCE:
					for (i2=1; paut->actions[j*AUT_ALPHABET_SIZE+i+i2] == REDUCE ; i2++);
					printf("   \"(%d, %c)\" [shape=none];\n", paut->reduit_n[j], paut->reduit_c[j]);
					printf("   Q%d -> \"(%d, %c)\" [ color=royalblue1, fontcolor=royalblue1, label = \"", j, paut->reduit_n[j], paut->reduit_c[j]);
					if (isprint(i)) printf("%c",i); else printf("0x%02x",i); 
					if (i2>1) {
						if (i2>2) printf("-");
						if (i2==2) printf(" ");
						if (isprint(i+i2-1)) printf("%c",i+i2-1) ; else printf("0x%02x",i+i2-1);
					}
					printf("\"];\n");
					i+=i2-1;
					break;
				default:
					break;
			}
		}
	}
	for( j=0; j < paut->nb_states; j++) {
		for (i=0; i < AUT_ALPHABET_SIZE; i++) {
			if (branch(paut,j,i) != 0 && branch(paut,j,i) < paut->nb_states) {
				printf("   Q%d -> Q%d [ color=red, fontcolor=red, label = \"", j, branch(paut,j,i));
				if (isprint(i)) printf("%c",i); else printf("0x%02x",i); 
				printf("\"];\n");
			}
		}
	}
	printf("}\n");
}
