#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#ifndef _AUTO_H
#define _AUTO_H 1

#define LR1_AUTOMATON_VERSION "V1.0"

/* Contains the current build of the program */
#define LR1_AUTOMATON_BUILD LR1_AUTOMATON_VERSION " build on "__DATE__" at "__TIME__""

/* Initialize the states of the tables of shift and goto */
#define AUTOMATON_UNKNOWN_STATE 	0xFFFFFFFF

/* Initialize the states of the tables of reduce_n */
#define AUTOMATON_UNKNOWN_REDUCE_N 	0xFFFFFFFF

/* Definition of the different actions of the automaton */
typedef enum {REJECT=0,ACCEPT=1,SHIFT=2,REDUCE=3} action_t;

/* Definition of the type of the states (considering there are no more than 256 distinct characters) of the automaton */
typedef unsigned int state_t;

/* Definition of the type of the states of the automaton */
typedef unsigned int reduce_n_t;

/* Definition of a type for booleans */
typedef enum {TRUE=1, FALSE=0} boolean_t;

/* Definition of a type for characters */
/* Taking into account any Unicode character */
typedef unsigned int uichar_t;

/* Structure describing the automaton */
typedef	struct {
		state_t			nb_states;
		uichar_t		nb_characters;
		char			*filename;
		boolean_t		*allowed_characters;
		action_t		*actions;
		reduce_n_t		*reduce_n;
		uichar_t		*reduce_c;
		state_t			*shift;
		state_t			*_goto;
} automaton_t;

/* Macros to get each component of the automaton structure */

/*  @requires: paut is a valid adress of an automaton structure;
	@assigns: nothing;
	@ensures: returns the number of states; */
#define automaton_nb_states(paut)        		paut->nb_states

/*  @requires: paut is a valid adress of an automaton structure;
	@assigns: nothing;
	@ensures: returns the number of characters accepted by the automaton; */
#define automaton_nb_characters(paut)       	paut->nb_characters

/*  @requires: paut is a valid adress of an automaton structure;
	@assigns: nothing;
	@ensures: returns the name of the file containing the automaton; */
#define automaton_filename(paut)        		paut->filename

/*  @requires: paut is a valid adress of an automaton structure and c is uichar_t;
	@assigns: nothing;
	@ensures: returns TRUE iff the character c is used otherwise FALSE; */
#define automaton_allowed_character(paut,c)    	paut->allowed_characters[c]
#define automaton_isAllowedCharacter(paut,c)   	(paut->allowed_characters[c]==1)


/*  @requires: paut is a valid adress of an automaton structure, s is state_t and c is a uichar_t;
	@assigns: nothing;
	@ensures: returns the action corresponding to (s, c); */
#define automaton_action(paut,s,c)        		paut->actions[s*paut->nb_characters+c]

/*  @requires: paut is a valid adress of an automaton structure and s is state_t;
	@assigns: nothing;
	@ensures: returns the integer corresponding to the first component of reduce(s) */
#define automaton_reduce_n(paut,s)        		paut->reduce_n[s]

/*  @requires: paut is a valid adress of an automaton structure and s is state_t;
	@assigns: nothing;
	@ensures: returns the character corresponding to the second component of reduce(s); */
#define automaton_reduce_c(paut,s)        		paut->reduce_c[s]

/*  @requires: paut is a valid adress of an automaton structure, s is state_t and c is uichar_t;
	@assigns: nothing;
	@ensures: returns the state shift(s,c); */
#define automaton_shift(paut,s,c)        		paut->shift[s*paut->nb_characters+c]

/*  @requires: paut is a valid adress of an automaton structure, s is state_t and c is uichar_t;
	@assigns: nothing;
	@ensures: returns the state goto(s, c); */
#define automaton_goto(paut,s,c)        		paut->_goto[s*paut->nb_characters+c]

#endif 

