ex12: main.o
	gcc -g -Wall main.o -o ex12 -lrt -lpthread

main.o: main.c
	gcc -g -Wall -c main.c

run: ex12
	./ex12

clean:
	rm *.o