ex06: main.o
	gcc -g -Wall main.o -o ex06.bin -lrt

main.o: main.c
	gcc -g -Wall -c main.c

run: ex06
	./ex06.bin

clean:
	rm *.o *.bin