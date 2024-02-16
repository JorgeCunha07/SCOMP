ex08: main.o
	gcc -g -Wall main.o -o ex08.bin -lrt

main.o: main.c
	gcc -g -Wall -c main.c

run: ex08
	./ex08.bin

clean:
	rm *.o *.bin