#
# 'make depend' uses makedepend to automatically generate dependencies 
#	       (dependencies are added to end of Makefile)
# 'make'	build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = gcc

# define any compile-time flags
CFLAGS = -Wall -Wextra

# define any directories containing header files other than /usr/include
#
INCLUDE_DIR = include
INCLUDES = -I $(INCLUDE_DIR)

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS = 

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
LIBS = -lreadline 

# define the C source files
SRCS_DIR = src
SRCS = $(SRCS_DIR)/auto_check.c  $(SRCS_DIR)/auto_DOT.c  $(SRCS_DIR)/auto_file.c  $(SRCS_DIR)/auto_ui.c 
SRCS_GEN_DOT = $(SRCS_DIR)/auto_file.c $(SRCS_DIR)/auto_DOT.c $(SRCS_DIR)/get_DOT.c
SRCS_AUT_DIR = aut
SRCS_AUT = $(SRCS_AUT_DIR)/arith.aut $(SRCS_AUT_DIR)/dyck.aut $(SRCS_AUT_DIR)/word.aut $(SRCS_AUT_DIR)/word_bis.aut

# define the C include files
INCLUDE_FILES = $(INCLUDE_DIR)/auto_check.h $(INCLUDE_DIR)/auto_color.h $(INCLUDE_DIR)/auto_DOT.h $(INCLUDE_DIR)/auto_file.h $(INCLUDE_DIR)/auto.h

# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#	 For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
OBJS_DIR = objs
OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)
OBJS_GEN_DOT = $(SRCS_GEN_DOT:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)
OBJS_DOT_DIR = dot
OBJS_DOT = $(SRCS_AUT:$(SRCS_AUT_DIR)/%.aut=$(OBJS_DOT_DIR)/%.dot)

# define the executable file 
MAIN = bin/auto_ui 
MAIN_GEN_DOT = bin/get_DOT
#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.SUFFIXES: .aut .dot

.PHONY: depend clean

all:    $(MAIN) $(MAIN_GEN_DOT) $(OBJS_DOT)

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

$(MAIN_GEN_DOT): $(OBJS_GEN_DOT) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN_GEN_DOT) $(OBJS_GEN_DOT) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c $(INCLUDE_FILES)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJS_DOT_DIR)/%.dot: $(SRCS_AUT_DIR)/%.aut 
	./$(MAIN_GEN_DOT) $< > $@

clean:
	$(RM) $(OBJS_DOT_DIR)/*.dot $(OBJS_DIR)/*.o *~ $(MAIN) $(MAIN_GEN_DOT)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
