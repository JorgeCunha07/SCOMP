ex06: main.o
	gcc -g -Wall main.o -o ex06 -lrt -lpthread

main.o: main.c
	gcc -g -Wall -c main.c

run: ex06
	./ex06

clean:
	rm *.o