#
#	Makefile
#	~~~~~~~~
#
#	@(#)	[MB] cr_Makefile	Version 1.1 du 20/02/22 - 
#

AIX_CC		= gxlc

all			: hl

cr_main.o		: cr_main.c cr_epri.h cr_cpri.h

cr_lex.o		: cr_lex.c cr_epri.h cr_cpri.h

hl			: cr_main.o cr_gpri.o cr_lex.o
			@ $(CC) -o hl cr_main.o cr_gpri.o cr_lex.o
			@ strip hl
			-@ ./hl -V || :

aix_hl		: cr_main.o cr_gpri.o cr_lex.o
			@ $(AIX_CC) -o hl cr_main.o cr_gpri.o cr_lex.o
			@ strip hl
			-@ ./hl -V || :
