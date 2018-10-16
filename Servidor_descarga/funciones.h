#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <netinet/in.h>
#include <resolv.h>
#include <ctype.h>
#include "openssl/ssl.h"
#include "openssl/err.h"
#include <arpa/inet.h>


#define FAIL -1

#define  BUFSIZE     1024 

typedef struct argumeto_t
{
    char pais[2];
    char city[4];
    char region[4];
    char org[20];
    char dep[20];
    char name[20];
    char mail[20];

}argumento;

typedef struct sock
{
    char ip[15];
    int sd;

}sockete;

int OpenListener(int port);
int isRoot();
SSL_CTX* InitServerCTX(void);
void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile);
void ShowCerts(SSL* ssl);
int agreUsuario(char* login, char* cont);
int busLogin(char* login);
int buscUsuario(char* login, char* cont);
int buscarVideo(char* video, char* ip, char* port);
int verificarNum(char* cont);
int verificarMay(char* cont);
int verificarCar(char* cont);
int verificarCont(char* cont);
int certificado_autofirmado (char *archivoCRT, char *archivoKEY, int Nrsa, int Nserie, int Ndias, argumento* arg);
int firma(char *archivo_cert, char* archivo_clave, char* archivo_firmado, char* archivo_CAI);
int csr (char *archivoCRT, char *archivoKEY, int Nrsa, int Nserie, int Ndias, argumento* arg);

#endif //FUNCIONES_H_INCLUDED