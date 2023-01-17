#
#	Makefile
#	~~~~~~~~
#
#	@(#)	[MB] cr_Makefile	Version 1.2 du 21/10/12 - MODIFIE
#

AIX_CC		= gxlc
#CFLAGS		= -pg -g
#CFLAGS		= -O2
CC			= gcc

all			: hl

cr_main.o		: cr_main.c cr_epri.h cr_cpri.h

cr_lex.o		: cr_lex.c cr_epri.h cr_cpri.h

hl			: cr_main.o cr_gpri.o cr_lex.o
			@ $(CC) $(CFLAGS) -o hl cr_main.o cr_gpri.o cr_lex.o
#			@ strip hl
			-@ ./hl -V 2>&1 | less -RX  || :

aix_hl		: cr_main.o cr_gpri.o cr_lex.o
			@ $(AIX_CC) -o hl cr_main.o cr_gpri.o cr_lex.o
			@ strip hl
			-@ ./hl -V < /dev/null || :

clean		:
			@ rm -f hl cr_main.o cr_gpri.o cr_lex.o cr_lex.c
