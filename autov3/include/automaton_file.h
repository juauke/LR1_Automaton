#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#include "automaton.h"

#ifndef _AUTO_FILE_H

#define _AUTO_FILE_H       1

#define AUT_EOL			'\n'
#define AUT_HEADER 		"%s %u\n"
#define AUT_END_OF_SEQUENCE	"\255\255\255\255\255\255"

void freeAut(automaton_t **ppaut);
automaton_t *loadAutomatonFromFile(char* filename) ;

#endif
