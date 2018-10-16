#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "funciones.h"
 
#define FAIL    -1
 


 

 
int main(int count, char *strings[])
{   SSL_CTX *ctx;
    int server;
    SSL *ssl;
    char buf[1024];
    char buf1[1024];
    char ip[1024];
    char port[1024];
    char *bloak="recibido";
    SSL_CTX *ctx2;
    int server2;
    SSL *ssl2;

    int bytes;
    char *hostname, *portnum;
 
    if ( count != 3 )
    {
        printf("usage: %s <hostname> <portnum>\n", strings[0]);
        exit(0);
    }
    SSL_library_init();
    hostname=strings[1];
    portnum=strings[2];
 
    ctx = InitCTX();
    server = OpenConnection(hostname, atoi(portnum));
    ssl = SSL_new(ctx);      /* create new SSL connection state */
    SSL_set_fd(ssl, server);    /* attach the socket descriptor */
    


    if ( SSL_connect(ssl) == FAIL )   /* perform the connection */
        ERR_print_errors_fp(stderr);
    else
    {   

        while(1)
        {
            bytes = SSL_read(ssl, buf, sizeof(buf)); /* get reply & decrypt */
            buf[bytes] = 0;
            printf("%s\n",buf);
            if (!strcmp(buf,"Operacion finalizada")){
                break;
            }
            
            else if (!strcmp(buf,"Lo sentimos, su video no pudo ser localizado o no esta disponible en estos momentos"))
                break;
            
            else if (!strcmp(buf,"Video disponible! Enviando informacion para descarga"))
            {
                bytes = SSL_read(ssl, ip, sizeof(ip)); /* get reply & decrypt */
                ip[bytes] = 0;
                bytes = SSL_read(ssl, port, sizeof(port)); /* get reply & decrypt */
                port[bytes] = 0;
                printf("La direccion ip asignada fue: %s\nEl puerto asignado fue: %s\n", ip, port);
                ctx2 = InitCTX();
                server2 = OpenConnection(ip, atoi(port));
                printf("entro a descarga\n");
                ssl2= SSL_new(ctx2);      /* create new SSL connection state */
                SSL_set_fd(ssl2, server2);
                
                FILE* f;

                if ( SSL_connect(ssl2) == FAIL )   /* perform the connection */
                    ERR_print_errors_fp(stderr);

                    else
                    {   
                        f=fopen(buf1,"wb");
                        SSL_write(ssl2, buf1, strlen(buf1));   /* encrypt & send message */

                        while(1)
                        {
                            bytes = SSL_read(ssl2, buf, 1024); /* get reply & decrypt */
                            buf[bytes] = 0;
                            printf("%s\n", buf);

                            if(!strcmp(buf,"Operacion finalizada"))
                            {
                                fclose(f);
                                break;
                            }
                    
                            bytes=fwrite(buf, 1, strlen(buf),f);
                            //printf("%d\n", bytes);
                            SSL_write(ssl2, bloak, sizeof(bloak));

                        }
                        SSL_free(ssl2);        /* release connection state */
                    }
                close(server2);         /* close socket */
                SSL_CTX_free(ctx2);        /* release context */
                break;

            }
            else{
                strcpy(buf,"");
                strcpy(buf,"");
                scanf("%s",buf1);
                SSL_write(ssl, buf1, strlen(buf1));   /* encrypt & send message */
            }
            
        }

        SSL_free(ssl);        /* release connection state */
    }
    close(server);         /* close socket */
    SSL_CTX_free(ctx);        /* release context */
    return 0;
}