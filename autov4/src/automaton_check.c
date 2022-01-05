#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#include "automaton.h"
#include "automaton_uichar.h"
#include "automaton_color.h"
#include "automaton_file.h"

/* Maximum size of the stack */
#define MAX_STACK 0xFFFF

int isword(automaton_t* paut, uichar_t* str) {
	int i, ix; // ix = index to browse the string

	int stack_top;
	state_t stack[MAX_STACK];

	state_t ns; // next state for 
	action_t act; // defining an action even for characters that are not recognised
	char reduce_n; // keeping the first component of the previous reduce
	char reduce_alpha; // keeping the second component of the previous reduce

	ix=0;
	stack_top=0;
	stack[stack_top]=0; // initial state = 0
	while(1) {
		
		if (str[ix] > automaton_nb_characters(paut))		// force REJECT action on unrecognised characters
			act=REJECT;
		else
			act=automaton_action(paut,stack[stack_top],str[ix]);
		
// printf("action=%d,ix=%d", act, ix);
//		for (i=0; i<stack_top;i++) {
//			printf("stack[%d]=%d,", i, stack[i]);
//		}
//		printf("\n");

		switch(act) {
			case REJECT:			
			/* if act == REJECT, we do nothing except printing the current state of the string and exiting */
				printf("   Word "BOLDRED"rejected"ANSI_COLOR_RESET" => \""ANSI_COLOR_GREEN);
				for (i=0; i<ix; i++) 
					printCharacter(str[i]);
				printf(ANSI_COLOR_RED);
				printCharacter(str[ix]);
				printf(ANSI_COLOR_YELLOW);
				for (i=ix+1; i<(int)uic_strlen(str); i++) 
					printCharacter(str[i]);
				printf(ANSI_COLOR_RESET"\"\n");
				printf(ANSI_COLOR_GREEN"                     ");
				for (i=0; i<ix; i++) printf("_");
				printf(ANSI_COLOR_RED"^\n"ANSI_COLOR_RESET);
				return 1;
				break;
			case ACCEPT:	
			/* if act == ACCEPT, we do nothing except printing the accepted string and exiting */
				printf("   Word "BOLDGREEN"accepted"ANSI_COLOR_RESET" => \""ANSI_COLOR_GREEN);
				for (i=0; i < (int) uic_strlen(str) ; i++) 
					if (isprint(str[i])) printCharacter(str[i]);
				printf(ANSI_COLOR_RESET"\"\n");
				return 0;
				break;
			case SHIFT: 
			/* if act == SHIFT, we push the next state and we go to the next character */
				ns=automaton_shift(paut, stack[stack_top],str[ix]);
				stack[++stack_top]=ns;
				ix++;
				break;
			case REDUCE: 
			/* if act == REDUCE, we recover (n, alpha) of the current state, we pop n times (s' is the state of the new top of the stack)
			   and we push the state obtained by the goto(s', alpha) */
				reduce_n=automaton_reduce_n(paut, (int) stack[stack_top]);
				reduce_alpha=automaton_reduce_c(paut, (int) stack[stack_top]);
				stack_top-=reduce_n;
				ns=automaton_goto(paut, stack[stack_top], reduce_alpha);
				stack[++stack_top]=ns;
				break;
		}
	}	
}
