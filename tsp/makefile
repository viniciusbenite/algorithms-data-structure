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


tsp_d:		tsp_d.c cities.h
	cc -Wall -O3 tsp_d.c -o tsp_d -lm
