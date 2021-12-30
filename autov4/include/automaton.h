#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#ifndef _AUTO_H
#define _AUTO_H 1

/* Initialize the states of the tables of decale and branche */
#define AUTOMATON_UNKNOWN_STATE 	0xff

/* Initialize the states of the tables of reduit_n */
#define AUTOMATON_UNKNOWN_REDUIT_N 	0xff

/* Definition of the different actions of the automaton */
typedef enum {REJECT=0,ACCEPT=1,DECALE=2,REDUCE=3} action_t;

/* Definition of the type of the states (considering there are no more than 256 distinct characters) of the automaton */
typedef unsigned int state_t;

/* Definition of the type of the states of the automaton */
typedef unsigned int reduit_n_t;

/* Definition a a type for booleans */
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
		reduit_n_t		*reduit_n;
		uichar_t		*reduit_c;
		state_t			*decale;
		state_t			*branch;
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
#define automaton_isAllowedCharacter(paut,c)   	(paut->allowed_characters[c])


/*  @requires: paut is a valid adress of an automaton structure, s is state_t and c is a uichar_t;
	@assigns: nothing;
	@ensures: returns the action corresponding to (s, c); */
#define automaton_action(paut,s,c)        		paut->actions[s*paut->nb_characters+c]

/*  @requires: paut is a valid adress of an automaton structure and s is state_t;
	@assigns: nothing;
	@ensures: returns the integer corresponding to the first component of reduit(s) */
#define automaton_reduit_n(paut,s)        		paut->reduit_n[s]

/*  @requires: paut is a valid adress of an automaton structure and s is state_t;
	@assigns: nothing;
	@ensures: returns the character corresponding to the second component of reduit(s); */
#define automaton_reduit_c(paut,s)        		paut->reduit_c[s]

/*  @requires: paut is a valid adress of an automaton structure, s is state_t and c is uichar_t;
	@assigns: nothing;
	@ensures: returns the state decale(s,c); */
#define automaton_decale(paut,s,c)        		paut->decale[s*paut->nb_characters+c]

/*  @requires: paut is a valid adress of an automaton structure, s is state_t and c is uichar_t;
	@assigns: nothing;
	@ensures: returns the state branchement(s, c); */
#define automaton_branch(paut,s,c)        		paut->branch[s*paut->nb_characters+c]

#endif 

