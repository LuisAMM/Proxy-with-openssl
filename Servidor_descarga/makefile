# make all: genera los ejecutables 
all: descarga 


# make descarga: genera solo el ejecutable descarga
descarga: SDescarga.o funciones.o 
	gcc -Wall SDescarga.o funciones.o -o descarga -L/usr/lib -lssl -lcrypto -lpthread

SDescarga.o: SDescarga.c
	gcc -Wall -c SDescarga.c -L/usr/lib -lssl -lcrypto -lpthread

funciones.o: funciones.c
	gcc -Wall -c funciones.c -L/usr/lib -lssl -lcrypto 


# make clean
clean:
	rm -rf *o descarga 

