consult_all: main.o
	gcc -g -Wall main.o -o consult_all -lrt -lpthread

main.o: main.c
	gcc -g -Wall -c main.c

run: consult_all
	./consult_all

clean:
	rm *.o