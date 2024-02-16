ex04b: main.o
	gcc -g -Wall main.o -o ex04b -lrt -lpthread

main.o: main.c
	gcc -g -Wall -c main.c

run: ex04b
	./ex04b

clean:
	rm *.o