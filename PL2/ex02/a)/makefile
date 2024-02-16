ex02a: main.o
	gcc main.o -o ex02a

main.o: main.c
	gcc -g -Wall -c main.c -o main.o

clean:
	rm -f *.o ex02a

run: ex02a
	./ex02a