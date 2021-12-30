#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 


#ifndef _AUTO_UICHAR_H
#define _AUTO_UICHAR_H 1

#include "automaton.h"

void _printCharacter(uichar_t c, boolean_t dotFormat) ;
#define printCharacter(c) 	_printCharacter(c,FALSE)
#define printCharacterDOT(c) 	_printCharacter(c,TRUE)
uichar_t *uictolc(char *s) ;
long int uic_strlen(uichar_t *s) ;
uichar_t *uic_strcpy(uichar_t *dest, uichar_t*src);
uichar_t *ctouic(char *s) ;
char *uictoc(uichar_t *s) ;

#endif
