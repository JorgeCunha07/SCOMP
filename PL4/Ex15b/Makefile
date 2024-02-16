ex15b: ex15b.o
	gcc ex15b.o -o ex15b -lpthread -lrt
	
ex15b.o: ex15b.c
	gcc -Wall -g -c ex15b.c -o ex15b.o

run: ex15b
	./ex15b
	
clean:
	rm *.o ex15b
