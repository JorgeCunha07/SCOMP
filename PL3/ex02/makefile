ex02: main.o
	gcc -g -Wall main.o -o ex02.bin -lrt

main.o: main.c
	gcc -g -Wall -c main.c

run: ex02
	./ex02.bin

clean:
	rm *.o *.bin