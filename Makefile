#
#	Makefile
#

hl		:
	cd src; make
	mv src/hl .
	- ./hl -V

clean	:
	rm -f hl src/hl src/*.o
