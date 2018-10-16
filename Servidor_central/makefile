# make all: genera los ejecutables 
all: central 


# make central: genera solo el ejecutable central
central: central.o funciones.o 
	gcc -Wall central.o funciones.o -o central -L/usr/lib -lssl -lcrypto -lpthread

central.o: central.c
	gcc -Wall -c central.c -L/usr/lib -lssl -lcrypto -lpthread

funciones.o: funciones.c
	gcc -Wall -c funciones.c -L/usr/lib -lssl -lcrypto 


# make clean
clean:
	rm -rf *o central 

