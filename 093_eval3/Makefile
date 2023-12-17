CFLAGS=-ggdb3 -Wall -Werror -pedantic -std=gnu++98
PROGS=cyoa-step1 cyoa-step2 cyoa-step3 cyoa-step4
OBJS=$(patsinst %, %.o, $(PROGS)) 
all: $(PROGS)
cyoa-step%: cyoa-step%.o choice.o page.o story.o utility.o
	g++ -o $@ $^
%.o: %.cpp cyoa.hpp
	g++ $(CFLAGS) -c $<
.PHONY: clean
clean:
	rm -f *~ $(PROGS) $(OBJS)

