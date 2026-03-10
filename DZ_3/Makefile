OPTS= -O3 -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format -std=c++14
all: a.out

a.out: a.o
	g++ $(OPTS) a.o -o a.out 

a.o: task.cpp list2.h 
	g++ $(OPTS) -c task.cpp -o a.o

clean:
	rm *.o*