# make all: genera los ejecutables de los tres programas
all:cliente



# make cliente: genera solo el ejecutable cliente
cliente: sslcl.o funciones.o 
	gcc -Wall sslcl.o funciones.o -o cliente -L/usr/lib -lssl -lcrypto -lpthread

sslcl.o: sslcl.c
	gcc -Wall -c sslcl.c -L/usr/lib -lssl -lcrypto -lpthread


funciones.o: funciones.c
	gcc -Wall -c funciones.c -L/usr/lib -lssl -lcrypto 


# make clean
clean:
	rm -rf *o cliente

