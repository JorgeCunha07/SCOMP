consult: main.o
	gcc -g -Wall main.o -o consult -lrt -lpthread

main.o: main.c
	gcc -g -Wall -c main.c

run: consult
	./consult

clean:
	rm *.o