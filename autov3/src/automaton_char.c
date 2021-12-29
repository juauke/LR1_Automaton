#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#include "automaton.h"
#include "automaton_file.h"

void printCharacter(char_t c) {
       	if (c == ' ') printf("' '"); 
	else if (c == ',') printf("','"); 
	else if (isprint(c)) printf("%c",c);
       	else if (c == '\n') printf("'\\n'"); 
	else printf("'0x%02x'",(unsigned char)c); 
}
