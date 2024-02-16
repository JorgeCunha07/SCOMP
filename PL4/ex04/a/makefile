ex04a: main.o
	gcc -g -Wall main.o -o ex04a -lrt -lpthread

main.o: main.c
	gcc -g -Wall -c main.c

run: ex04a
	./ex04a

clean:
	rm *.o