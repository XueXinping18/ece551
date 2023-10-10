GCCFLAGS=-std=gnu99 -Wall
test: test-power.o power.o
	gcc -o test test-power.o power.o
test-power.o: test-power.c
	gcc $(GCCFLAGS) -c test-power.c
power.o: power.c
	gcc $(GCCFLAGS) -c power.c
.PHONY: clean
clean:
	rm -f test *.o *.c~ *.h~
