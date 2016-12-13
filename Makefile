all: ereport.o trts_pic.o

ereport.o:
	gcc -c ereport.S -o $@

trts_pic.o:
	gcc -c trts_pic.S -o $@

clean:
	rm *.o
