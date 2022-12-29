#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#include "auto.h"

#ifndef _AUTO_FILE_H

#define _AUTO_FILE_H       1

#define AUT_EOL			'\n'
#define AUT_HEADER 		"a %u\n"
#define AUT_END_OF_SEQUENCE	"\255\255\255"

typedef struct {
	state_t s;
	AUT_CHAR_T c;
	state_t sprime;
} triplet_t;

void freeAut(aut_t **ppaut);
aut_t *loadAutomateFromFile(char* filename) ;

#endif
