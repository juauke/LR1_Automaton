/* fileman.c -- A tiny application which demonstrates how to use the
   GNU Readline library.  This application interactively allows users
   to manipulate files and their modes. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "auto.h"
#include "auto_file.h"
#include "auto_DOT.h"
#include "auto_check.h"

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "%s: usage: %s [aut_file]\n", argv[0], argv[0]);
		exit(1);
	}
	aut_t *paut=loadAutomatonFromFile(argv[1]);
	DOTaut(paut);
}
