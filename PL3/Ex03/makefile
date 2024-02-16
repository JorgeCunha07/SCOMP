escrita: escrita.o
	gcc -g -Wall escrita.o -o escrita 

escrita.o: escrita.c
	gcc -g -Wall -c escrita.c -o escrita.o

leitura: leitura.o
	gcc -g -Wall leitura.o -o leitura 

leitura.o: leitura.c
	gcc -g -Wall -c leitura.c -o leitura.o

clean:
	rm *.o

run: escrita leitura
	./escrita
	./leitura