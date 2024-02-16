ex13: ex13.o
	gcc ex13.o -o ex13 -lpthread -lrt
	
ex13.o: ex13.c
	gcc -Wall -g -c ex13.c -o ex13.o

run: ex13
	./ex13
	
clean:
	rm *.o ex13
