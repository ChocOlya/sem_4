OPTS= -O3 -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format -std=c++14
all: a01.out a02.out a03.out a04.out a05.out a06.out

a01.out: a01.o
	g++ $(OPTS) a01.o solve.o -o a01.out 

a01.o: task01.cpp solve.o
	g++ $(OPTS) -c task01.cpp -o a01.o



a02.out: a02.o
	g++ $(OPTS) a02.o solve.o -o a02.out 

a02.o: task02.cpp solve.o
	g++ $(OPTS) -c task02.cpp -o a02.o



a03.out: a03.o
	g++ $(OPTS) a03.o solve.o -o a03.out 

a03.o: task03.cpp solve.o
	g++ $(OPTS) -c task03.cpp -o a03.o



a04.out: a04.o
	g++ $(OPTS) a04.o solve.o -o a04.out 

a04.o: task04.cpp solve.o
	g++ $(OPTS) -c task04.cpp -o a04.o



a05.out: a05.o
	g++ $(OPTS) a05.o solve.o -o a05.out 

a05.o: task05.cpp solve.o
	g++ $(OPTS) -c task05.cpp -o a05.o



a06.out: a06.o
	g++ $(OPTS) a06.o solve.o -o a06.out 

a06.o: task06.cpp solve.o
	g++ $(OPTS) -c task06.cpp -o a06.o



solve.o: solve.cpp
	g++ $(OPTS) -c solve.cpp -o solve.o

clean:
	rm *.o*