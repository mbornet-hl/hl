#
#	Makefile
#

AIX_CC	= gxlc

hl		: cr_main.o cr_gpri.o cr_lex.o
		@ $(CC) -o hl cr_main.o cr_gpri.o cr_lex.o
		@ strip hl
		-@ ./hl -V || :

aix_hl	: cr_main.o cr_gpri.o cr_lex.o
		@ $(AIX_CC) -o hl cr_main.o cr_gpri.o cr_lex.o
		@ strip hl
		-@ ./hl -V || :
