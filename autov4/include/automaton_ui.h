#ifndef _AUTO_UI_H
#define _AUTO_UI_H 1

#include "automaton.h"

extern char* getwd();
extern char* xmalloc();

/* @requires: s is a valid char array;
   @assigns: allocates a char array r;
   @ensures: returns a copy of the char array s;
*/
char* dupstr(char* s);

/* @requires: text is a valid char array;
   @assigns: nothing;
   @ensures: completes commands; */
char* command_generator(char* text, int state);

/* @requires: text is a valid char array;
   @assigns: allocates a char array r;
   @ensures: attempts to complete on the contents of text.*/
char** fileman_completion(char* text, int start, int end);

/* @requires: s is a valid char array;
   @assigns: nothing;
   @ensures: lists the file(s) named in arg;; */
int com_list(char* arg);

/* @requires: arg is a valid char array;
   @assigns: nothing;
   @ensures: prints out the content of arg using a more system command; */
int com_view(char* arg);

/* @requires: ignore is a valid char array;
   @assigns: nothing;
   @ensures: prints out the current working directory. */
int com_pwd(char* ignore);

/* @requires: arg is a valid path to the automaton file;
	@assigns: allocates the automaton structure;
	@ensures: initializes the structure with the content of the file */
int com_load_automaton_from_file(char* arg);

/* @requires: arg is a valid char array;
   @assigns: allocates a char array r;
   @ensures: prints out help for ARG, or for all of the commands if ARG is
   not present; */
int com_help(char* arg);

/* @requires: arg is a valid char array;
   @assigns: nothing;
   @ensures: changes to the directory ARG; */
int com_cd(char* arg);

/* @requires: nothing;
   @assigns: nothing;
   @ensures: quits the program when requested by the user (by setting DONE to non-zero); */
int com_quit();

/* @requires: nothing;
	@assigns: nothing;
	@ensures: prints the DOT graph of the automaton; */
int com_automaton_DOT();

/* @requires: nothing;
	@assigns: creates a temp graph DOT file and executes a system command;
	@ensures: opens an X11 window containing the DOT graph of the automaton; */
int com_automaton_DOT_x();

/*  @requires: arg is a valid array of char;
	@assigns: nothing;
	@ensures: prints the result of the analysis of arg by the automaton; */
int com_automaton_isword(char* arg);

/* @requires: s is a valid char array;
   @assigns: allocates a char array r;
   @ensures: returns a copy of the char array s; */
int com_automaton_version();

/* A structure which contains information on the commands this program
   can understand. */

typedef struct {
	char* name;			/* User printable name of the function. */
	int (*func)();		/* Function to call to do the job. */
	char* doc;			/* Documentation for this function.  */
} COMMAND;

/* @requires: string is a valid char array;
   @assigns: allocates a char array s;
   @ensures: strips whitespace from the start and end of STRING. returns a pointer
   into string. */
char* stripwhite(char* string);

/* @requires: name is a valid char array;
   @assigns: nothing;
   @ensures: looks up name as the name of a command, and returns a pointer to that
             command. Returns a NULL pointer if name isn't a command name.  */
COMMAND* find_command(char* name);

/* @requires: nothing;
   @assigns: nothing;
   @ensures: tells the GNU Readline library how to complete; */
void initialize_readline();

/* @requires: line is a valid char array;
   @assigns: nothing;
   @ensures: executes the command line; */
int execute_line(char* line);

/* @requires: s is a valid char array;
   @assigns: allocates a char array r;
   @ensures: returns a copy of the char array s; */
char* command_generator(char* text, int state);

/* @requires: s is a valid char array;
   @assigns: allocates a char array r;
   @ensures: shows the ma; */
char** completion_matches(char* text, char* (*entry_func)(char*, int));

/* @requires: caller and arg are valid char arrays;
   @assigns: nothing;
   @ensures: returns non-zero if arg is a valid argument for caller, else prints
   an error message and returns zero; */
int valid_argument(char* caller, char* arg);

#endif