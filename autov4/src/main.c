#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/errno.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "automaton_ui.h"
#include "automaton_uichar.h"
#include "automaton_file.h"
#include "automaton_color.h"
#include "automaton_DOT.h"
#include "automaton_check.h"

