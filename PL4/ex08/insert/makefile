insert: main.o
	gcc -g -Wall main.o -o insert -lrt -lpthread

main.o: main.c
	gcc -g -Wall -c main.c

run: insert
	./insert

clean:
	rm *.o