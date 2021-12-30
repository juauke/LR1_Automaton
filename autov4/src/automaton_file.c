#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#include "automaton_file.h"

void freeAut(automaton_t **ppaut) {
	if  (*ppaut != NULL) {
		if ((*ppaut)->branch			!= NULL) free((*ppaut)->branch);
		if ((*ppaut)->decale			!= NULL) free((*ppaut)->decale);
		if ((*ppaut)->reduit_c			!= NULL) free((*ppaut)->reduit_c);
		if ((*ppaut)->reduit_n			!= NULL) free((*ppaut)->reduit_n);
		if ((*ppaut)->actions			!= NULL) free((*ppaut)->actions);
		if ((*ppaut)->filename			!= NULL) free((*ppaut)->filename);
		if ((*ppaut)->allowed_characters	!= NULL) free((*ppaut)->allowed_characters);
		free(*ppaut);
	}
}

automaton_t *allocAutomaton(int nb_states, int nb_chars, char *filename) {
	automaton_t *paut;
	if ((paut=calloc(1,sizeof(automaton_t))) == NULL) return (NULL);
	automaton_nb_states(paut)=nb_states;
	automaton_nb_characters(paut)=nb_chars;
	if ((paut->filename=calloc(sizeof(char),strlen(filename)+1)) == NULL) {
		freeAut(&paut) ; return (NULL);
	}
	strcpy(automaton_filename(paut), filename);
	if ((paut->allowed_characters=calloc(sizeof(boolean_t),automaton_nb_characters(paut))) == NULL) {
		freeAut(&paut) ; return (NULL);
	}
	if ((paut->actions=calloc(sizeof(action_t),automaton_nb_states(paut)*automaton_nb_characters(paut))) == NULL) {
		freeAut(&paut) ; return (NULL);
	}
	if ((paut->reduit_n=calloc(sizeof(reduit_n_t),automaton_nb_states(paut))) == NULL) {
		freeAut(&paut) ; return (NULL);
	}
	memset(paut->reduit_n, AUTOMATON_UNKNOWN_REDUIT_N, automaton_nb_states(paut));
	if ((paut->reduit_c=calloc(sizeof(uichar_t),automaton_nb_states(paut))) == NULL) {
		freeAut(&paut) ; return (NULL);
	}
	if ((paut->decale=calloc(sizeof(state_t),automaton_nb_states(paut)*automaton_nb_characters(paut))) == NULL) {
		freeAut(&paut) ; return (NULL);
	}
	memset(paut->decale, AUTOMATON_UNKNOWN_STATE, sizeof(state_t)*automaton_nb_states(paut)*automaton_nb_characters(paut));
	if ((paut->branch=calloc(sizeof(state_t),automaton_nb_states(paut)*automaton_nb_characters(paut))) == NULL) {
		freeAut(&paut) ; return (NULL);
	}
	memset(paut->branch, AUTOMATON_UNKNOWN_STATE, sizeof(state_t)*automaton_nb_states(paut)*automaton_nb_characters(paut));
	return(paut);
}

#define MAX_ALPHABET_SIZE	0x110000

automaton_t *loadAutomatonFromFile(char* filename) {
	FILE *automaton_file;
	int nb_states,nb_chars, character_size, triplet_size, reduit_n_size, c, k;
	char *buffer, triplet[7], *p;
	uichar_t character;
	int result, n;
	state_t j;
	uichar_t i;
	automaton_t *paut;

	if ( (automaton_file = fopen( filename, "r") ) == NULL ) return(NULL);

	result=fscanf(automaton_file,AUT_HEADER,triplet, &(nb_states));
	if (result == EOF) return(NULL) ;
	if (result!=2) return(NULL) ;

	character_size=strlen(triplet);
// hypothèse reduit_n_size = character_size 
	reduit_n_size = character_size ;
	triplet_size=strlen(triplet)+2;
	if ((buffer=calloc(sizeof(action_t),MAX_ALPHABET_SIZE*nb_states))==NULL) return(NULL);
	
	p=buffer;
	while ((c=fgetc(automaton_file)) != AUT_EOL) {
		if (c>0xff) {
			printf("Error in fgetc\n");
			return(NULL);
		}
		*p++=(char)c;
	}
	nb_chars=(p-buffer)/nb_states;
	if ((paut=allocAutomaton(nb_states, nb_chars, filename)) == NULL) return(NULL);
/* populate actions */
	for (j=0; j<automaton_nb_states(paut) ; j++) {
		for (i=0; i<automaton_nb_characters(paut); i++)
			automaton_action(paut,j,i)=buffer[j*automaton_nb_characters(paut)+i];
	}

/* populate reduit_n */
	result=fread(buffer,reduit_n_size,automaton_nb_states(paut), automaton_file);
	for (j=0; j<automaton_nb_states(paut) ; j++) {
		for (n=0,k=0; k<reduit_n_size; k++) n+=buffer[j*reduit_n_size+k]<<(8*k);
		automaton_reduit_n(paut,j)=n;
	}
	if (result != paut->nb_states) goto FAIL;
	if ((result=fgetc(automaton_file)) != AUT_EOL) goto FAIL;

/* populate reduit_c */
	result=fread(buffer,character_size,automaton_nb_states(paut), automaton_file);
	for (j=0; j<automaton_nb_states(paut) ; j++) {
		for (character=0,k=0; k<character_size; k++) character+=buffer[j*character_size+k]<<(8*k);
		automaton_reduit_c(paut,j)=character;
	}
	if (result != paut->nb_states) goto FAIL;
	if ((result=fgetc(automaton_file)) != AUT_EOL) goto FAIL;

/* populate decale */
	while ( fread(&triplet, triplet_size, 1, automaton_file) != 0 ) {
		if (memcmp((char *)&triplet, AUT_END_OF_SEQUENCE, triplet_size) == 0 )  break;
		for (character=0,k=0; k<triplet_size-2; k++) character+=triplet[1+k]<<(8*k);
		automaton_decale(paut,triplet[0],character)=triplet[triplet_size-1];
	}

/* populate branch */
	while ( fread(&triplet, triplet_size, 1, automaton_file) != 0 ) {
		if (memcmp((char *)&triplet, AUT_END_OF_SEQUENCE, triplet_size) == 0 )  break;
		for (character=0,k=0; k<triplet_size-2; k++) character+=triplet[1+k]<<(8*k);
		automaton_branch(paut,triplet[0],character)=triplet[triplet_size-1];
	}

	for( int j=0; j < automaton_nb_states(paut); j++) {
		for (i=0; i<automaton_nb_characters(paut); i++) {
                       if (automaton_action(paut,j,i) != 0) {
				automaton_allowed_character(paut,i)=1;
			}
		}
	}

	if (buffer != NULL) free (buffer);
	fclose(automaton_file);
	return (paut);
FAIL:
	if (buffer != NULL) free (buffer);
	freeAut(&paut);
	fclose(automaton_file);
	return (NULL);
}
