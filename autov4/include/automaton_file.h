#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#include "automaton.h"

#ifndef _AUTO_FILE_H

#define _AUTO_FILE_H       1

/* End of line character for the automaton */
#define AUT_EOL			'\n'

/* Header of the automaton */
#define AUT_HEADER 		"%s %u\n"

/* End of sequence characters for the automaton */
#define AUT_END_OF_SEQUENCE	"\255\255\255\255\255\255"

/*  @requires: ppaut is a valid adress of a pointer to an automaton structure;
	@assigns: frees the memory used by the automaton structure;
	@ensures: returns nothing; */
void freeAut(automaton_t **ppaut);

/*  @requires: filename is a valid path to an automaton file;
	@assigns: allocates and initializes the elements of the automaton structure;
	@ensures: returns an adress of the automaton structure; */
automaton_t* loadAutomatonFromFile(char* filename) ;

#endif
