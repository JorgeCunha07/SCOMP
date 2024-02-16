ex02: main.o
	gcc -g -Wall main.o -o ex02 -lrt -lpthread

main.o: main.c
	gcc -g -Wall -c main.c

run: ex02
	./ex02

clean:
	rm *.o