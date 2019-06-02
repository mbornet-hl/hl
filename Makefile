#
#	Makefile
#

hl	:
	cd src; make
	mv src/hl .
	- ./hl -V

