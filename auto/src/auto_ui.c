/* fileman.c -- A tiny application which demonstrates how to use the
   GNU Readline library.  This application interactively allows users
   to manipulate files and their modes. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/errno.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "auto.h"
#include "auto_color.h"
#include "auto_file.h"
#include "auto_DOT.h"
#include "auto_check.h"

aut_t *paut=NULL;

extern char *getwd ();
extern char *xmalloc ();

/* The names of functions that actually do the manipulation. */
int com_list (), com_view (), com_rename (), com_stat (), com_pwd ();
int com_load_automaton_from_file (), com_help (), com_cd (), com_quit ();
int com_automaton_DOT (); int com_automaton_isword ();

/* A structure which contains information on the commands this program
   can understand. */

typedef struct {
  char *name;			/* User printable name of the function. */
  int(*func)();		/* Function to call to do the job. */
  char *doc;			/* Documentation for this function.  */
} COMMAND;

COMMAND commands[] = {
  { "cd", com_cd, "Change to directory DIR" },
  { "load", com_load_automaton_from_file, "Load Automaton from file" },
  { "loadFile", com_load_automaton_from_file, "Synonym for `load'" },
  { "gDOT", com_automaton_DOT, "Show Automaton DOT graph" },
  { "getDOT", com_automaton_DOT, "Synonym for `gDOT'" },
  { "chk", com_automaton_isword, "Check string to automaton" },
  { "check", com_automaton_isword, "Synonym for `chk'" },
  { "help", com_help, "Display this text" },
  { "?", com_help, "Synonym for `help'" },
  { "list", com_list, "List files in DIR" },
  { "ls", com_list, "Synonym for `list'" },
  { "pwd", com_pwd, "Print the current working directory" },
  { "quit", com_quit, "Quit using LR1" },
  { "rename", com_rename, "Rename FILE to NEWNAME" },
  { "stat", com_stat, "Print out statistics on FILE" },
  { "view", com_view, "View the contents of FILE" },
  { (char*) NULL, (int(*)())NULL, (char *)NULL }
};

/* Forward declarations. */
char *stripwhite ();
COMMAND *find_command ();
void initialize_readline ();
int execute_line (char*);
char * command_generator (char *text, int state);
char **completion_matches(char *text, char *(*entry_func)(char*, int));
int valid_argument (char *,char *);
void too_dangerous (char *);

/* The name of this program, as taken from argv[0]. */
char *progname;

/* When non-zero, this global means the user is done using this program. */
int done;

char *
dupstr (s)
     char *s;
{
  char *r;

  r = xmalloc (strlen (s) + 1);
  strcpy (r, s);
  return (r);
}

int 
main (argc, argv)
     int argc;
     char **argv;
{
  char *line, *s;
  char prompt[AUT_FILENAME_MAX+20];
  char historyfile[AUT_FILENAME_MAX+20];

  sprintf(historyfile,"%s/.history_auto_ui", getenv("HOME"));

  progname = argv[0];

  if (argc>1) paut=loadAutomatonFromFile(argv[1]);

  initialize_readline ();	/* Bind our completer. */
  read_history(historyfile);

  /* Loop reading and executing lines until the user quits. */
  for ( ; done == 0; )
    {
	printf(BOLDCYAN);
	if (paut == NULL) {
      		line = readline("LR1: ");
	} else {
		printf(ANSI_COLOR_RESET" >>> Allowed characters = ["ANSI_COLOR_GREEN);
		for (int i=0 ; i<AUT_ALPHABET_SIZE; i++) {
			if ( paut->allow_character[i] == 1) {
				if (isprint(i)) printf("%c", i);
        else {
					// /*** if user needs to write '\n' explicitely */
					// if (i == '\n') printf("\\n");
					// else
				  if (i!=0 && i!='\n') printf("\\x%02x", i);
				}
			}
		}
		printf(ANSI_COLOR_RESET"]"BOLDCYAN"\n");
		sprintf(prompt, "LR1[current=%s]: ", paut->filename);
      		line = readline(prompt);
	}
	printf(ANSI_COLOR_RESET);

      if (!line) break;

      /* Remove leading and trailing whitespace from the line.
         Then, if there is anything left, add it to the history list
         and execute it. */
      s = stripwhite (line);

      if (*s) {add_history(s); write_history(historyfile); execute_line(s);}

      free (line);
    }
  
  exit(0);
}

/* Execute a command line. */
int
execute_line (line)
     char *line;
{
  register int i;
  COMMAND *command;
  char *word;

  /* Isolate the command word. */
  i = 0;
  while (line[i] && whitespace(line[i])) i++;
  
  word = line + i;

  while (line[i] && !whitespace(line[i])) i++;

  if (line[i]) line[i++] = '\0';

  command = find_command(word);

  if (!command) {fprintf (stderr, "%s: No such command for .\n", word); return -1;}

  /* Get argument to command, if any. */
  while (whitespace(line[i])) i++;

  word = line + i;

  /* Call the function. */
  return ((*(command->func)) (word));
}

/* Look up NAME as the name of a command, and return a pointer to that
   command.  Return a NULL pointer if NAME isn't a command name. */
COMMAND* find_command(char* name) {
  register int i;

  for (i = 0; commands[i].name; i++)
    if (strcmp(name, commands[i].name) == 0)
      return &commands[i];

  return ((COMMAND*) NULL);
}

/* Strip whitespace from the start and end of STRING.  Return a pointer
   into STRING. */
char* stripwhite(char *string) {
    register char *s, *t;

  for (s = string; whitespace(*s); s++);

  if (*s == 0)
    return s;

  t = s + strlen(s) - 1;
  while (t > s && whitespace(*t))
    t--;
  *++t = '\0';

  return s;
}

/* **************************************************************** */
/*                                                                  */
/*                  Interface to Readline Completion                */
/*                                                                  */
/* **************************************************************** */

char *command_generator();
char **fileman_completion();

/* Tell the GNU Readline library how to complete.  We want to try to complete
   on command names if this is the first word in the line, or on filenames
   if not. */
void initialize_readline() {
  /* Allow conditional parsing of the ~/.inputrc file. */
  rl_readline_name = "LR1";

  /* Tell the completer that we want a crack first. */
  rl_attempted_completion_function = fileman_completion;
}

/* Attempt to complete on the contents of TEXT.  START and END show the
   region of TEXT that contains the word to complete.  We can use the
   entire line in case we want to do some simple parsing.  Return the
   array of matches, or NULL if there aren't any. */
char** fileman_completion(char* text, int start, int end) {
  char** matches = (char **) NULL;

  /* If this word is at the start of the line, then it is a command
     to complete.  Otherwise it is the name of a file in the current
     directory. */
  if (end<start) return NULL;
  
  if (start == 0) matches = completion_matches(text, command_generator);

  return matches;
}

/* Generator function for command completion.  STATE lets us know whether
   to start from scratch; without any state (i.e. STATE == 0), then we
   start at the top of the list. */
char* command_generator(text, state)
    char* text;
    int state;
{
  static int list_index, len;
  char* name;

  /* If this is a new word to complete, initialize now.  This includes
     saving the length of TEXT for efficiency, and initializing the index
     variable to 0. */
  if (!state) {list_index = 0; len = strlen (text);}

  /* Return the next name which partially matches from the command list. */
  while ((name = commands[list_index].name) != NULL)
    {
      list_index++;

      if (strncmp (name, text, len) == 0) return (dupstr(name));
    }

  /* If no names matched, then return NULL. */
  return ((char*) NULL);
}

/* **************************************************************** */
/*                                                                  */
/*                       LR1 Commands                           */
/*                                                                  */
/* **************************************************************** */

/* String to pass to system ().  This is for the LIST, VIEW and RENAME
   commands. */
static char syscom[1024];

/* List the file(s) named in arg. */
int com_list(char *arg) {
  if (!arg)
    arg = "";

  sprintf(syscom, "ls -FClg %s", arg);
  return system(syscom);
}

int com_view(char* arg) {
  if (!valid_argument("view", arg)) {printf("Invalid argument given for view.\n"); exit(1);}

  sprintf(syscom, "more %s", arg);
  return system(syscom);
}

int com_rename(char* arg) {
	if (arg !=NULL) too_dangerous("rename");
  
  return 0;
}

int com_stat(char* arg) {
  struct stat finfo;

  if (!valid_argument("stat", arg)) {printf("Invalid argument given for stat.\n"); exit(1);}

  if (stat(arg, &finfo) == -1) {perror("stat"); exit(2);}

  printf("Statistics for `%s':\n", arg);

  printf("%s has %d link%s, and is %d byte%s in length.\n", arg, (int) finfo.st_nlink, (finfo.st_nlink == 1) ? "" : "s", (int) finfo.st_size, (finfo.st_size == 1) ? "" : "s");
  printf("Inode Last Change at: %s", ctime(&finfo.st_ctime));
  printf("      Last access at: %s", ctime(&finfo.st_atime));
  printf("    Last modified at: %s", ctime(&finfo.st_mtime));
  return 0;
}

int com_load_automaton_from_file(char* arg) {
  if (paut!=NULL) {printf("No automaton given.\n"); freeAut(&paut); exit(1);}

  if (!valid_argument ("stat", arg)) {printf("Invalid argument given for stat.\n"); exit(2);}

  printf("Load automaton from `%s'\n", arg);
  paut=loadAutomatonFromFile(arg);
	if (paut==NULL) {perror("loadAutomatonfromFile"); exit(3);}
  
  return 0;
}

int com_automaton_DOT() {
	if (paut == NULL) {printf("No automaton loaded.\n"); exit(1);}

	DOTaut(paut);
  return 0;
}

char *replaceBackslashN(char* s) {
	char *p, *p1, *buff;

	buff=malloc(strlen(s));
	for (p=s, p1=buff; *p; p++, p1++) {
		switch (*p) {
		case '\\':                      // detecting the \n's
			if (*(p+1)  == 'n') { 
				p++;
				*p1='\n';
			}
			break;
		
    default:
			*p1=*p;
			break;
		}
	}

	*p1='\0';
	return buff;
}

int com_automaton_isword(char* arg) {

	if (paut == NULL) {printf("No automaton loaded.\n"); exit(1);}
	
	if (!valid_argument ("automatonCheck", arg)) {printf("Invalid argument given for automatonCheck.\n"); exit(2);}
//	/* if user needs to write '\n' explicitely */
//	ns=replaceBackslashN(arg);
	
  int l=strlen(arg);
	char* ns=malloc(l+1*sizeof(char));
	strcpy(ns,arg);
	ns[l]='\n';
	ns[l+1]='\0';
	
  isword(paut,ns);

	free(ns);
  return 0;
}

/* Print out help for ARG, or for all of the commands if ARG is
   not present. */
int com_help(char* arg) {
  register int i;
  int printed = 0;

  for (i = 0; commands[i].name; i++) {
      if (!*arg || (strcmp(arg, commands[i].name) == 0)) {printf("%s\t\t%s.\n", commands[i].name, commands[i].doc); printed++;}
    }
    
  if (!printed) {
      printf("No commands match `%s'.  Possibilties are:\n", arg);

      for (i = 0; commands[i].name; i++) {
          /* Print in six columns. */
          if (printed == 6) {printed = 0; printf("\n");}

          printf("%s\t", commands[i].name);
          printed++;
      }
    }
      else printf("\n");

  return 0;
}

/* Change to the directory ARG. */
int com_cd(char* arg) {
  if (chdir(arg) == -1) {perror("chdir"); exit(1);}

  com_pwd("");
  return 0;
}

/* Print out the current working directory. */
int com_pwd(char* ignore) {
  char dir[1024], *s;

  s = getcwd(dir, 1024);
  if (s == 0) {fprintf(stderr, "Error getting pwd: %s(%s)\n", dir, ignore); exit(1);}

  printf("Current directory is %s\n", dir);
  return 0;
}

/* The user wishes to quit using this program.  Just set DONE non-zero. */
int com_quit() {
	done = 1;
  return 0;
}

/* Function which tells you that you can't do this. */
void too_dangerous(char* caller) {
  fprintf(stderr, "%s: Too dangerous for me to distribute.  Write it yourself.\n", caller);
}

/* Return non-zero if ARG is a valid argument for CALLER, else print
   an error message and return zero. */
int valid_argument (char* caller, char* arg) {
  if (!arg || !*arg) {fprintf(stderr, "%s: %s [arg]\n", caller, caller); exit(1);}

  return 0;
}
