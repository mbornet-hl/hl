#
#	Makefile
#	~~~~~~~~
#
#	@(#)	[MB] cr_Makefile	Version 1.8 du 26/05/07 - 
#

AIX_CC		= gxlc
# CFLAGS		= -pg -g
# CFLAGS		= -O2
CC			= gcc

all			: hl hl_which

cr_main.o		: cr_main.c cr_epri.h cr_cpri.h

cr_lex.o		: cr_lex.c cr_epri.h cr_cpri.h

hl			: cr_main.o cr_gpri.o cr_lex.o
			@ $(CC) $(CFLAGS) -o hl cr_main.o cr_gpri.o cr_lex.o
			@ strip hl
			-@ ./hl -V || :

cr_which.o	: cr_which.c

hl_which		: cr_which.o
			@ $(CC) -o hl_which cr_which.o
			@ strip hl_which


aix_hl		: cr_main.o cr_gpri.o cr_lex.o
			@ $(AIX_CC) -o hl cr_main.o cr_gpri.o cr_lex.o
			@ strip hl
			-@ ./hl -V || :

clean		:
			@ rm -f cr_main.o cr_gpri.o cr_lex.o cr_lex.c
			@ rm -f cr_which.o

clean_all		: clean
			@ rm -f hl hl_which
