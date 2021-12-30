#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#ifndef _AUTO_LCHAR_H
#include "automaton.h"

#define _AUTO_LCHAR_H 1

typedef unsigned int uichar_t;
void printCharacter(uichar_t c) ;
uichar_t *uictolc(char *s) ;
long int uic_strlen(uichar_t *s) ;
uichar_t *uic_strcpy(uichar_t *dest, uichar_t*src);
uichar_t *ctouic(char *s) ;
char *uictoc(uichar_t *s) ;

#endif
