CCFLAGS=--std=gnu++98 -pedantic -Wall -Werror -ggdb3
%.o: %.cpp
	g++ -c $(CCFLAGS) $<
code: code.o
	g++ -o code $(CCFLAGS) code.o
clean:
	rm -f *.o *~ code
