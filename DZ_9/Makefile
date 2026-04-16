OPTS= -O3 -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format -std=c++14
all: server client

server: server.o
	g++ $(OPTS) server.o -o server 

server.o: ser_tcp.cpp #list2.h 
	g++ $(OPTS) -c ser_tcp.cpp -o server.o



client: client.o
	g++ $(OPTS) client.o -o client 

client.o: cli_tcp.cpp #list2.h 
	g++ $(OPTS) -c cli_tcp.cpp -o client.o

clean:
	rm *.o server client