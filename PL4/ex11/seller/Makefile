prog: seller.o
	gcc seller.o -o prog -lpthread -lrt
	
seller.o: seller.c
	gcc -Wall -g -c seller.c -o seller.o

run: prog
	./prog
	
clean:
	rm *.o prog
