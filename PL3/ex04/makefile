writer: writer.o
	gcc -g -Wall writer.o -o writer.bin -lrt

writer.o: writer.c
	gcc -g -Wall -c writer.c

reader: reader.o
	gcc -g -Wall reader.o -o reader.bin -lrt

reader.o: reader.c
	gcc -g -Wall -c reader.c

run: writer reader
	./writer.bin
	./reader.bin

clean:
	rm *.o *.bin
