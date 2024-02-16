ex14: main.o
	gcc -g -Wall main.o -o ex14 -lrt -lpthread

main.o: main.c
	gcc -g -Wall -c main.c

run: ex14
	./ex14

clean:
	rm *.o