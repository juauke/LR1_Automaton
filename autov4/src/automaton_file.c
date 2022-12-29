#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#include "automaton_file.h"

/*  @requires: paut is a valid adress to an automaton type;
	@assigns: nothing;
	@ensures: dumps the values of the automaton structure; */
void dumpAutomaton(automaton_t *paut) {
	state_t j;
	uichar_t i;

	printf("Automaton [%s], nb_states=%d, nb_characters=%d\n",
			automaton_filename(paut),
			automaton_nb_states(paut),
			automaton_nb_characters(paut)); /* Prints the number of states and the alphabet size of the automaton */
	printf("========== actions =============\n");
	for (j=0; j<automaton_nb_states(paut);j++) {
		printf("%d|",j);
		for (i=0; i<automaton_nb_characters(paut);i++) {
			if (automaton_action(paut,j,i) != REJECT) printf("%02x=%0x,",i,automaton_action(paut,j,i)); /* Prints the actions of the automaton */
		}
		printf("\n");
	}
	printf("========== reduce =============\n");
	for (j=0; j<automaton_nb_states(paut);j++) {
		printf("%d|",j);
		if (automaton_reduce_n(paut,j) != AUTOMATON_UNKNOWN_REDUCE_N) printf("(%d,%02x)",automaton_reduce_n(paut,j),automaton_reduce_c(paut,j)); /* Prints both components of REDUCE of the automaton */
		printf("\n");
	}
	printf("========== shift =============\n");
	for (j=0; j<automaton_nb_states(paut);j++) {
		printf("%d|",j);
		for (i=0; i<automaton_nb_characters(paut);i++) {
			if (automaton_shift(paut,j,i) != AUTOMATON_UNKNOWN_STATE) printf("%02x=>%02x,",i,automaton_shift(paut,j,i)); /* Prints the SHIFT states of the automaton */
		}
		printf("\n");
	}
	printf("========== goto =============\n");
	for (j=0; j<automaton_nb_states(paut);j++) {
		printf("%d|",j);
		for (i=0; i<automaton_nb_characters(paut);i++) {
			if (automaton_goto(paut,j,i) != AUTOMATON_UNKNOWN_STATE) printf("%02x=>%02x,", i, automaton_goto(paut,j,i)); /* Prints the GOTO states of the automaton */
		}
		printf("\n");
	}
}

/*  @requires: ppaut is a valid pointer to a pointer to an automaton type;
	@assigns: nothing;
	@ensures: frees all allocated elements corresponding to the automaton; */
void freeAut(automaton_t **ppaut) {
	if  (*ppaut != NULL) {
		if ((*ppaut)->_goto					!= NULL) free((*ppaut)->_goto);
		if ((*ppaut)->shift					!= NULL) free((*ppaut)->shift);
		if ((*ppaut)->reduce_c				!= NULL) free((*ppaut)->reduce_c);
		if ((*ppaut)->reduce_n				!= NULL) free((*ppaut)->reduce_n);
		if ((*ppaut)->actions				!= NULL) free((*ppaut)->actions);
		if ((*ppaut)->filename				!= NULL) free((*ppaut)->filename);
		if ((*ppaut)->allowed_characters	!= NULL) free((*ppaut)->allowed_characters);
		free(*ppaut);
	}
}

automaton_t *allocAutomaton(int nb_states, int nb_chars, char* filename) {
	automaton_t *paut;
	if ((paut=calloc(1, sizeof(automaton_t))) == NULL) {perror("calloc-automaton"); return NULL;}
/* allocation of the automaton structure where the number of states, the number of characters and the filename are required to allocate enough memory */
	
	automaton_nb_states(paut)=nb_states;
	automaton_nb_characters(paut)=nb_chars;

/* allocation of the filename */
	if ((paut->filename=calloc(sizeof(char), strlen(filename)+1)) == NULL) {perror("calloc-filename"); freeAut(&paut); return NULL;} 
	strcpy(automaton_filename(paut), filename);

/* allocation of the allowed_character array */
	if ((paut->allowed_characters=calloc(sizeof(boolean_t), automaton_nb_characters(paut))) == NULL) {perror("calloc-allowed_characters"); freeAut(&paut); return NULL;}

/* allocation of the action array */
	if ((paut->actions=calloc(sizeof(action_t), automaton_nb_states(paut)*automaton_nb_characters(paut))) == NULL) {perror("calloc-actions"); freeAut(&paut); return NULL;}
	
/* allocation of the reduce_n array */
	if ((paut->reduce_n=calloc(sizeof(reduce_n_t), automaton_nb_states(paut))) == NULL) {perror("calloc-reduce_n"); freeAut(&paut); return NULL;}
/* memset to 0xFFFF to identify the undefined reduce_n */
	memset(paut->reduce_n, AUTOMATON_UNKNOWN_REDUCE_N, automaton_nb_states(paut));
	
/* allocation of the reduce_c array */
	if ((paut->reduce_c=calloc(sizeof(uichar_t), automaton_nb_states(paut))) == NULL) {perror("calloc-reduce_c"); freeAut(&paut); return NULL;}
	
/* allocation of the shift array */
	if ((paut->shift=calloc(sizeof(state_t), automaton_nb_states(paut)*automaton_nb_characters(paut))) == NULL) {perror("calloc-shift"); freeAut(&paut); return NULL;}
/* memset to 0xFFFF to identify the undefined shift state */
	memset(paut->shift, AUTOMATON_UNKNOWN_STATE, sizeof(state_t)*automaton_nb_states(paut)*automaton_nb_characters(paut));

/* allocation of the _goto array */
	if ((paut->_goto=calloc(sizeof(state_t), automaton_nb_states(paut)*automaton_nb_characters(paut))) == NULL) {perror("calloc-goto"); freeAut(&paut); return NULL;}
/* memset to 0xFFFF to identify the undefined _goto state */
	memset(paut->_goto, AUTOMATON_UNKNOWN_STATE, sizeof(state_t)*automaton_nb_states(paut)*automaton_nb_characters(paut));
	
	return paut;
}

#define MAX_ALPHABET_SIZE 0x110000

automaton_t *loadAutomatonFromFile(char* filename) {
	FILE* automaton_file;
	int nb_states, nb_chars;
/* we suppose that the sizes of characters, triplet and reduce_n will be deduced from the file */
	int character_size, triplet_size, reduce_n_size; 
	int c, k;
	char *buffer, triplet[7], *p;
	uichar_t character;
	int result, n;
	state_t j;
	uichar_t i;
	automaton_t *paut;

	if ((automaton_file = fopen(filename, "r")) == NULL) {perror("fopen"); return NULL;}

/* from the first line, we find: a string whose length defines the byte number per character and the number of states separated by a space
   examples: "a 4" -> 1 byte per character and 4 states || "aaa 18" -> 3 bytes per character and 18 states */
	result=fscanf(automaton_file, AUT_HEADER, triplet, &(nb_states));
	if (result == EOF) {perror("fscanf1-nb_states"); return NULL;}
	if (result != 2) {perror("fscanf2-nb_states"); return NULL;}

	character_size=strlen(triplet);
/* hypothesis: reduce_n_size = character_size */ 
	reduce_n_size = character_size ;

/* for the end of sequence of triplet
   example: 3 -> '\255\255\255' */
	triplet_size=strlen(triplet)+2;
	
/* temp storage of the actions array until we deduce the real alphabet size */
	if ((buffer=calloc(sizeof(action_t),MAX_ALPHABET_SIZE*nb_states))==NULL) {perror("calloc-buffer"); return NULL;}
	p=buffer;

	while ((c=fgetc(automaton_file)) != AUT_EOL) {
		if (c>0xff) {printf("Error in fgetc\n"); return NULL;} // too big of a character found

		*p++=(char) c;
	}

/* calculation of the alphabet size (number of bytes of the action array divided by the number of states) */
	nb_chars=(p-buffer)/nb_states;

/* with the number of states, the alphabet size and the filename, we can allocate all resources needed for the automaton */
	if ((paut=allocAutomaton(nb_states, nb_chars, filename)) == NULL) {perror("allocAutomaton"); return NULL;}

/* populate actions from the previously read buffer */
	for (j=0; j<automaton_nb_states(paut); j++) {
		for (i=0; i<automaton_nb_characters(paut); i++)
			automaton_action(paut,j,i)=buffer[j*automaton_nb_characters(paut)+i];
	}

/* populate reduce_n from file */
	result=fread(buffer, reduce_n_size, automaton_nb_states(paut), automaton_file);
	for (j=0; j<automaton_nb_states(paut) ; j++) {
		for (n=0,k=0; k<reduce_n_size; k++) n+=buffer[j*reduce_n_size+k]<<(8*k);
		automaton_reduce_n(paut,j)=n;
	}
	if (result != (int) paut->nb_states) {perror("fread-reduce_n"); goto FAIL;}
	if ((result=fgetc(automaton_file)) != AUT_EOL) {perror("fgetc-reduce_n"); goto FAIL;}

/* populate reduce_c from file (hypothesis: byte order Big-endian) */
	result=fread(buffer, character_size, automaton_nb_states(paut), automaton_file);
	for (j=0; j < automaton_nb_states(paut) ; j++) {
		for (character=0, k=0; k < character_size; k++) character+=buffer[j*character_size+k]<<(8*((character_size-1) - k));
		automaton_reduce_c(paut,j)=character;
	}
	if (result != (int) paut->nb_states) {perror("fread-reduce_c"); goto FAIL;}
	if ((result=fgetc(automaton_file)) != AUT_EOL) {perror("fgetc-reduce_c"); goto FAIL;}

/* populate shift from file (hypothesis: byte order Big-endian) */
	while (fread(&triplet, triplet_size, 1, automaton_file) != 0) {
/* end of loop with the string \255 (current state) n= 1, 2, 3 or 4 '\255' (character) \255 (next state) */
		if (memcmp((char *) &triplet, AUT_END_OF_SEQUENCE, triplet_size) == 0 ) break;
		for (character=0,k=0; k<triplet_size-2; k++) character+=triplet[1+k]<<(8*((character_size-1) - k));
		automaton_shift(paut, triplet[0], character)=triplet[triplet_size-1];
	}

/* populate goto (hypothesis: byte order Big-endian) */
	while (fread(&triplet, triplet_size, 1, automaton_file) != 0) {
/* end of loop with the string \255 (current state) n= 1, 2, 3 or 4 '\255' (alpha) \255 (next state) */
		if (memcmp((char*) &triplet, AUT_END_OF_SEQUENCE, triplet_size) == 0 ) break;
		for (character=0,k=0; k<triplet_size-2; k++) character+=triplet[1+k]<<(8*((character_size-1) - k));
		automaton_goto(paut,triplet[0],character)=triplet[triplet_size-1];
	}

/* populate the array of the allowed characters by the automaton */
	for (j=0; j < automaton_nb_states(paut); j++)
		for (i=0; i < automaton_nb_characters(paut); i++)
			automaton_allowed_character(paut,i)=(automaton_isAllowedCharacter(paut, i) || automaton_action(paut,j,i) != 0);

	goto CLOSE;

FAIL:
	freeAut(&paut);
	paut=NULL;

CLOSE:
	if (buffer != NULL) free(buffer);
	fclose(automaton_file);
	return paut;
}
