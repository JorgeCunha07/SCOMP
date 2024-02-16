ex15a: ex15a.o
	gcc ex15a.o -o ex15a -lpthread -lrt
	
ex15a.o: ex15a.c
	gcc -Wall -g -c ex15a.c -o ex15a.o

run: ex15a
	./ex15a
	
clean:
	rm *.o ex15a
