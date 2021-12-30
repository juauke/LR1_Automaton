#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "automaton_uichar.h"

int main(int argc, char * argv[]) {
	uichar_t *ns;
	printf("IN  = %s\n", argv[1]);	
	ns=ctouic(argv[1]);	
	uictoc(ns);
	free(ns);
}
