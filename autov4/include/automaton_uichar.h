
#ifndef _AUTO_UICHAR_H
#define _AUTO_UICHAR_H 1

#include "automaton.h"

/*  @requires: nothing;
	@assigns: nothing;
	@ensures: prints the character c (even if c is a Unicode character); */
void _printCharacter(uichar_t c, boolean_t dotFormat) ;

/*  @requires: nothing;
	@assigns: nothing;
	@ensures: prints the character c (even if c is a Unicode character); */
#define printCharacter(c) 	_printCharacter(c,FALSE)

/*  @requires: nothing;
	@assigns: nothing;
	@ensures: prints the character c for the DOT graph (even if c is a Unicode character); */
#define printCharacterDOT(c) 	_printCharacter(c,TRUE)

/*  @requires: s is a valid array of uichar_t;
	@assigns: nothing;
	@ensures: returns the length of the given array; */
long int uic_strlen(uichar_t* s) ;

/*  @requires: dest and src are valid arrays of uichar_t;
	@assigns: nothing;
	@ensures: copies the content of src to dest and returns dest; */
uichar_t* uic_strcpy(uichar_t* dest, uichar_t* src);

/*  @requires: s is a valid array of char;
	@assigns: allocates an array of uichar_t;
	@ensures: returns the array converted to uichar_t; */
uichar_t* ctouic(char* s);

/*  @requires: s is a valid array of uichar_t;
	@assigns: allocates an array of char;
	@ensures: returns the array converted to char; */
char* uictoc(uichar_t* s) ;

#endif
