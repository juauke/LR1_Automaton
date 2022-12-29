#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#ifndef _AUTO_H

#define _AUTO_H 1

#define AUT_ALPHABET_SIZE	128
#define AUT_ALPHABET_CHAR_SIZE	sizeof(char)

#define AUT_FILENAME_MAX 	2048

enum {REJECT=0,ACCEPT=1,DECALE=2,REDUCE=3} ;

typedef char aut_char_t;
typedef char aut_reduit_nb_t;
typedef char action_t;
typedef char state_t;
typedef	struct {
		int				nb_states;
		char			allow_character[AUT_ALPHABET_SIZE];
		char			filename[AUT_FILENAME_MAX];
		action_t		*actions;
		aut_reduit_nb_t	*reduit_n;
		aut_char_t		*reduit_c;
		state_t			*decale;
		state_t			*branch;
} aut_t;

#define action(paut,s,c)        paut->actions[s*AUT_ALPHABET_SIZE+c]
#define reduit_n(paut,s)        paut->reduit_n[s]
#define reduit_c(paut,s)        paut->reduit_c[s]
#define decale(paut,s,c)        paut->decale[s*AUT_ALPHABET_SIZE+c]
#define branch(paut,s,c)        paut->branch[s*AUT_ALPHABET_SIZE+c]

#endif 
