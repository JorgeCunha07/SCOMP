ex02b: main.o
	gcc main.o -o ex02b

main.o: main.c
	gcc -g -Wall -c main.c -o main.o

clean:
	rm -f *.o ex02b

run: ex02b
	./ex02b