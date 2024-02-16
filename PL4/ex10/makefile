ex10: main.o
	gcc -g -Wall main.o -o ex10 -lrt -lpthread

main.o: main.c
	gcc -g -Wall -c main.c

run: ex10
	./ex10

clean:
	rm *.o