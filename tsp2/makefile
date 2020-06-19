#
# makefile to compile all programs for the AED.P04 computer lab class
#

clean:
	rm -f examples tsp a.out

all:		examples

run_all:	all
	./examples 1


examples:	examples.c
	cc -Wall -O2 examples.c -o $@ -lm

tsp:		tsp.c cities.h
	cc -Wall -O3 tsp.c -o tsp -lm

tsp2:		tsp2.c cities.h
	cc -Wall -O3 tsp2.c -o tsp2 -lm



teste:		teste.c cities.h
	cc -Wall -O3 teste.c -o teste -lm

teste2:		teste2.c cities.h
	cc -Wall -O3 teste2.c -o teste2 -lm