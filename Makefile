#
#	Makefile
#

hl		:
	cd src; make
	mv src/hl .
	- ./hl -V || :
	mv src/hl_which .

clean	:
	rm -f hl src/hl src/*.o hl_which
