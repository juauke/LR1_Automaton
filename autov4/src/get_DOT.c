/* fileman.c -- A tiny application which demonstrates how to use the
   GNU Readline library.  This application interactively allows users
   to manipulate files and their modes. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "automaton_file.h"
#include "automaton_DOT.h"

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("%s: usage: %s <automaton_file>\n", argv[0], argv[0]);
		exit(1);
	}
	automaton_t *paut=loadAutomatonFromFile(argv[1]);
	DOTaut(paut);	// displays the automaton graph in DOT format
	freeAut(&paut);
	return 0;
}
