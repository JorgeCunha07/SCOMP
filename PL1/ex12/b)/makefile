ex12b: main.o spawn_child.o
	gcc main.o spawn_child.o -o ex12b

main.o: main.c
	gcc -g -Wall -c main.c -o main.o

spawn_child.o: spawn_child.c
	gcc -g -Wall -c spawn_child.c -o spawn_child.o

clean:
	rm -f *.o ex12b

run: ex12b
	./ex12b