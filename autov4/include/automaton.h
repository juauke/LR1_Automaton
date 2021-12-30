#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#ifndef _AUTO_H
#define _AUTO_H 1

#define AUTOMATON_UNKNOWN_STATE 	0xff
#define AUTOMATON_UNKNOWN_REDUIT_N 	0xff

typedef enum {REJECT=0,ACCEPT=1,DECALE=2,REDUCE=3} action_t ;
typedef unsigned char state_t;
typedef unsigned int reduit_n_t;
typedef enum {TRUE=1, FALSE=0} boolean_t;
typedef unsigned int uichar_t;

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

#define automaton_nb_states(paut)        	paut->nb_states
#define automaton_nb_characters(paut)        	paut->nb_characters
#define automaton_filename(paut)        	paut->filename
#define automaton_allowed_character(paut,c)    	paut->allowed_characters[c]
#define automaton_isAllowedCharacter(paut,c)   	(paut->allowed_characters[c])
#define automaton_action(paut,s,c)        	paut->actions[s*paut->nb_characters+c]
#define automaton_reduit_n(paut,s)        	paut->reduit_n[s]
#define automaton_reduit_c(paut,s)        	paut->reduit_c[s]
#define automaton_decale(paut,s,c)        	paut->decale[s*paut->nb_characters+c]
#define automaton_branch(paut,s,c)        	paut->branch[s*paut->nb_characters+c]

#endif 

