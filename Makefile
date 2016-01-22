all: wthread
	g++ main.cpp -o main wthread.o

wthread:
	g++ wthread.cpp -c

clean:
	rm -f *.o main
