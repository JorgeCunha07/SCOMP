ex16: main.o
	gcc -g -Wall main.o -o ex16 -lrt -lpthread

main.o: main.c
	gcc -g -Wall -c main.c

run: ex16
	./ex16

clean:
	rm *.o