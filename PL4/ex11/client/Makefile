prog: client.o
	gcc client.o -o prog -lpthread -lrt
	
client.o: client.c
	gcc -Wall -g -c client.c -o client.o

run: prog
	./prog
	
clean:
	rm *.o prog
