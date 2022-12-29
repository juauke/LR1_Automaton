#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#include "automaton.h"

#ifndef _AUTO_CHECK_H

#define _AUTO_CHECK_H 1

/*  @requires: paut is a valid adress of an automaton structure and str is a valid array;
	@assigns: nothing;
	@ensures: prints the analysis of the string str with the automaton *paut; */
int isword(automaton_t *paut, uichar_t *str) ;

#endif

