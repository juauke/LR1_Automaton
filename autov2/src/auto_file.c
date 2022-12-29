#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#include "auto.h"
#include "auto_file.h"

void freeAut(aut_t **ppaut) {
	if  (*ppaut != NULL) {
		if ((*ppaut)->branch	!= NULL) free((*ppaut)->branch);
		if ((*ppaut)->decale	!= NULL) free((*ppaut)->decale);
		if ((*ppaut)->reduit_c	!= NULL) free((*ppaut)->reduit_c);
		if ((*ppaut)->reduit_n	!= NULL) free((*ppaut)->reduit_n);
		if ((*ppaut)->actions	!= NULL) free((*ppaut)->actions);
		free(*ppaut);
	}
}

aut_t *allocAut(int nb_states) {
	aut_t *paut;
	if ((paut=calloc(1,sizeof(aut_t))) == NULL) return (NULL);
	paut->nb_states=nb_states;
	if ((paut->actions=calloc(1,paut->nb_states* AUT_ALPHABET_SIZE * sizeof(action_t))) == NULL) {
		freeAut(&paut) ; return (NULL);
	}
	if ((paut->reduit_n=calloc(1,paut->nb_states*sizeof(AUT_REDUIT_NB_T))) == NULL) {
		freeAut(&paut) ; return (NULL);
	}
	if ((paut->reduit_c=calloc(1,paut->nb_states*sizeof(AUT_CHAR_T))) == NULL) {
		freeAut(&paut) ; return (NULL);
	}
	if ((paut->decale=calloc(1,paut->nb_states*AUT_ALPHABET_SIZE*sizeof(state_t))) == NULL) {
		freeAut(&paut) ; return (NULL);
	}
	if ((paut->branch=calloc(1,paut->nb_states*AUT_ALPHABET_SIZE*sizeof(state_t))) == NULL) {
		freeAut(&paut) ; return (NULL);
	}
	return(paut);
}

aut_t *loadAutomateFromFile(char* filename) {
	FILE *aut_file;
	int nb_states;
	int result;
	aut_t *paut;
	triplet_t triplet;

	if ( (aut_file = fopen( filename, "r") ) == NULL ) return(NULL);

	result=fscanf(aut_file,AUT_HEADER,&(nb_states));
	if (result == EOF) return(NULL) ;
	if (result!=1) return(NULL) ;
	
	if ((paut=allocAut(nb_states)) == NULL) return(NULL);
	strcpy(paut->filename,filename);

/* populate actions */
	result=fread(paut->actions,sizeof(action_t),paut->nb_states* AUT_ALPHABET_SIZE, aut_file);
	if (result != paut->nb_states* AUT_ALPHABET_SIZE) goto FAIL;
	if ((result=fgetc(aut_file)) != AUT_EOL) goto FAIL;


/* populate reduit_n */
	result=fread(paut->reduit_n,sizeof(AUT_REDUIT_NB_T),paut->nb_states, aut_file);
	if (result != paut->nb_states) goto FAIL;
	if ((result=fgetc(aut_file)) != AUT_EOL) goto FAIL;

/* populate reduit_c */
	result=fread(paut->reduit_c,sizeof(AUT_CHAR_T),paut->nb_states, aut_file);
	if (result != paut->nb_states) goto FAIL;
	if ((result=fgetc(aut_file)) != AUT_EOL) goto FAIL;

/* populate decale */
	while ( fread(&triplet, sizeof(triplet), 1, aut_file) != 0 ) {
		if (memcmp((char *)&triplet, AUT_END_OF_SEQUENCE, sizeof(triplet)) == 0 )  break;
		paut->decale[triplet.s*AUT_ALPHABET_SIZE+triplet.c]=triplet.sprime;
	}

/* populate branch */
	while ( fread(&triplet, sizeof(triplet), 1, aut_file) != 0 ) {
		if (memcmp((char *)&triplet, AUT_END_OF_SEQUENCE, sizeof(triplet)) == 0 )  break;
		paut->branch[triplet.s*AUT_ALPHABET_SIZE+triplet.c]=triplet.sprime;
	}

	for( int j=0; j < paut->nb_states; j++) {
		for (int i=0; i<AUT_ALPHABET_SIZE; i++) {
                       if (paut->actions[j*AUT_ALPHABET_SIZE+i] != 0) {
				paut->allow_character[i]=1;
			}
		}
	}

	return (paut);

FAIL:
	freeAut(&paut);
	return (NULL);
}
