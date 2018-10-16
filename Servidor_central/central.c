#include "funciones.h"

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;



int PUERTO=30000;
int contador=0;
char *certificado_cai="mycert.pem";
char *archivoKEY="mykey.pem";


void *AtencionDescarga(void* cliente){

    char buf1[100];
    char buf[1024];
    char buf2[1024];
    char archivoSalida[10];
    sockete aux=*(sockete*)cliente;
    
    //semaforo
    contador++;
    
    sprintf(buf1,"CSR%d",contador);
    sprintf(archivoSalida,"CSRFir%d",contador);
    //semafin

    int sockaux=aux.sd;
    if(recv( sockaux, buf, sizeof(buf), 0 )==-1)
    {
        perror("error");            
    }

    FILE* fd = fopen(buf1,"w");
    FILE* fa;
    fprintf(fd,"%s",buf);
    fclose(fd);

    firma(buf1,archivoKEY,archivoSalida,certificado_cai);

    strcpy(buf,"");
    fd = fopen(archivoSalida,"r");
    fread(buf,sizeof(char),1024,fd);
    fclose(fd);

    if(send( sockaux, buf, sizeof(buf), 0 )==-1)
    {
        perror("error");            
    }

    strcpy(buf,"");

    if(recv( sockaux, buf, sizeof(buf), 0 )==-1)
    {
        perror("error");            
    }
    //semaforo
    fd = fopen("lista_de_videos.txt","w+");
    fprintf(fd,"%s",buf);
    rewind(fd);
    sprintf(buf,"%d",PUERTO);

    fa=fopen("videos.txt","a");

    while(!feof(fd)){
        fscanf(fd,"%s", buf2);
        fprintf(fa, "%s %s %s ",buf2, aux.ip, buf);
        printf("%s %s %s \n",buf2, aux.ip, buf );
    }
    fclose(fa);
    //semafin
    fclose(fd);    

    
    PUERTO++;

    if(send( sockaux, buf, sizeof(buf), 0 )==-1)
    {
        perror("error");            
    }


    pthread_exit(NULL);
}

void *ServDescargas(void* port){

    int a=*(int*)port;
    int server;
    pthread_t hilo;
    server = OpenListener(a);
    sockete aux;
    while (1)
    {   
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        int client = accept(server, (struct sockaddr*)&addr, &len);  /* accept connection as usual */
       
        aux.sd=client;
        strcpy(aux.ip,inet_ntoa(addr.sin_addr));
        if (client==-1)
        {
            perror("Error");
        }

        printf("Connection: %s:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        pthread_create(&hilo, NULL, AtencionDescarga , (void *)&aux); 
    }
    
    close(server);          /* close server socket */
    pthread_exit(NULL);
}


void *atender(void* cliente){
    
    char* buf="Si es un usuario registrado escriba si, por el contrario escriba REGISTRO.\nPara salir escriba fin";
    char* buf1="Introduzca su login:";
    char* buf2="Introduzca su contrasena:";
    char* buf3="Login o contrasena no autorizado.\nEscriba si para volver a intentar o REGISTRO para registrarse, fin para salir";
    char buf4[1024];
    char buf5[1024];
    char bufaux[1024];
    char* buf6="Introduzca un nombre de usuario:";
    char* buf7="introduzca la contrasena que desea usar.\nDebe tener al menos un numero, un caracter especial una mayuscula y tener 8 o mas caracteres\n";
    char* buf8="Operacion finalizada";
    char* buf9="Por favor introduzca REGISTRO o si, si desea finalizar la operacion escriba fin\n";
    char* buf10="El usuario solicitado ya existe, vuelva a escribir REGISTRO e intente de nuevo\n";
    char* buf11="La contrasena no es valida, debe tener al menos un numero, un caracter especial\nuna mayuscula y tener 8 o mas caracteres\n";
    char* buf12="Desea descargar un video ahora? Escriba el que desea! Puede finalizar si lo desea (fin)\n";
    char* buf13="Escriba el comando VIDEO.\nPuede finalizar si lo desea (fin o cualquier otra cosa)\n";
    char* buf14="Video disponible! Enviando informacion para descarga";
    char* buf15="Lo sentimos, su video no pudo ser localizado o no esta disponible en estos momentos";
    char* buf16="Que video desea descargar?";

    int bytes;
    

    SSL_CTX *ctx;
    SSL *ssl;
    int sd;
    int sockaux=*(int*)cliente;
    ctx = InitServerCTX(); 
    LoadCertificates(ctx, "mycert.pem", "mykey.pem"); /* load certs */
    ssl = SSL_new(ctx);              /* get new SSL state with context */
    SSL_set_fd(ssl, sockaux);      /* set connection socket to SSL state */

    if ( SSL_accept(ssl) == FAIL )     /* do SSL-protocol accept */
        ERR_print_errors_fp(stderr);

    SSL_write(ssl, buf, strlen(buf));
    

    while(1)
    {
        
        bytes = SSL_read(ssl, buf4, sizeof(buf4)); /* get reply & decrypt */
        buf4[bytes] = 0;
        switch (SSL_get_error(ssl, bytes))
        { 
            case SSL_ERROR_NONE: 
                break;
            case SSL_ERROR_ZERO_RETURN: 
               
                sd = SSL_get_fd(ssl);       /* get socket connection */
                SSL_free(ssl);         /* release SSL state */
                close(sd);          /* close connection */
                pthread_exit(NULL);
            default: 
 
                sd = SSL_get_fd(ssl);       /* get socket connection */
                SSL_free(ssl);         /* release SSL state */
                close(sd);          /* close connection */
                pthread_exit(NULL);
        }
        if (!strcmp(buf4,"si"))
        {
            SSL_write(ssl, buf1, strlen(buf1));
            bytes = SSL_read(ssl, buf4, sizeof(buf4)); /* get reply & decrypt */
            buf4[bytes] = 0;
            printf("%s\n", buf4);
            SSL_write(ssl, buf2, strlen(buf2));
            bytes = SSL_read(ssl, buf5, sizeof(buf5)); /* get reply & decrypt */
            buf5[bytes] = 0;
            printf("%s\n",buf5 );
            if(buscUsuario(buf4,buf5)==1)
            {
                printf("usuario confirmado\n");
                SSL_write(ssl, buf13, strlen(buf13));
                bytes = SSL_read(ssl, bufaux, sizeof(bufaux)); /* get reply & decrypt */
                bufaux[bytes] = 0;
                printf("%s\n", bufaux); 
                if (!strcmp(bufaux,"VIDEO"))
                {
                        SSL_write(ssl, buf16, strlen(buf16));
                        bytes = SSL_read(ssl, bufaux, sizeof(bufaux)); /* get reply & decrypt */
                        bufaux[bytes] = 0;
                        if(buscarVideo(bufaux, buf4, buf5))
                        {
                            SSL_write(ssl, buf14, strlen(buf14));
                            printf("%s\n",buf4 );
                            printf("%s\n",buf5 );
                            SSL_write(ssl, buf4, strlen(buf4));
                            SSL_write(ssl, buf5, strlen(buf5));
                            break;
                        }
                        else
                        {
                            SSL_write(ssl, buf15, strlen(buf15));
                            break;
                        }
                }
                else
                {
                    SSL_write(ssl, buf8, strlen(buf8));
                    break;
                }
            }
            else 
                SSL_write(ssl, buf3, strlen(buf3));
        }  
        else if (!strcmp(buf4,"REGISTRO"))
        {
            SSL_write(ssl, buf6, strlen(buf6));
            bytes = SSL_read(ssl, buf4, sizeof(buf4)); /* get reply & decrypt */
            buf4[bytes] = 0;
            printf("%s\n", buf4);
            SSL_write(ssl, buf7, strlen(buf7));
            bytes = SSL_read(ssl, buf5, sizeof(buf5)); /* get reply & decrypt */
            buf5[bytes] = 0;
            printf("%s\n",buf5 );
            if(busLogin(buf4)==0 && verificarCont(buf5))
            {
                agreUsuario(buf4,buf5);
                SSL_write(ssl, buf12, strlen(buf12));
                bytes = SSL_read(ssl, bufaux, sizeof(bufaux)); /* get reply & decrypt */
                bufaux[bytes] = 0;
                printf("%s\n", bufaux); 
                if (!strcmp(bufaux,"fin"))
                {
                    SSL_write(ssl, buf8, strlen(buf8));
                    break;
                }
                else
                {
                    if(buscarVideo(bufaux, buf4, buf5))
                    {
                        SSL_write(ssl, buf14, strlen(buf14));
                        SSL_write(ssl, buf4, strlen(buf4));
                        SSL_write(ssl, buf5, strlen(buf5));
                        break;
                    }

                    else
                    {
                        SSL_write(ssl, buf15, strlen(buf15));
                        break;
                    }
                }
            }
                
            else
            {

                if (busLogin(buf4)==1)
                    SSL_write(ssl, buf10, strlen(buf10));
                else
                    SSL_write(ssl, buf11, strlen(buf11));
            }
                
        }  
        else if(!strcmp(buf4,"fin"))
        {
            SSL_write(ssl, buf8, strlen(buf8));
            break;
        }  
        else
            SSL_write(ssl, buf9, strlen(buf9));  

    } 
    
    sd = SSL_get_fd(ssl);       /* get socket connection */
    SSL_free(ssl);         /* release SSL state */
    close(sd);          /* close connection */
    pthread_exit(NULL);
}


void *AtencionCliente(void* port){
    
    int a=*(int*)port;
    SSL_library_init();
    int server;
    pthread_t hilo;
    server = OpenListener(a);
    
    while (1)
    {   
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        int client = accept(server, (struct sockaddr*)&addr, &len);  /* accept connection as usual */
       
        if (client==-1)
        {
            perror("Error");
        }
        printf("Connection: %s:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        pthread_create(&hilo, NULL, atender , (void *)&client); 
    }
    
    close(server);          /* close server socket */
}


int main(int argc, char **port){
    int portnumc, portnums;
    pthread_t hilo1, hilo2;
    
    if(!isRoot())
    {
        printf("Este programa debe ser usado con privilegios root!!");
        exit(0);
    }

    if ( argc != 3 )
    {
        printf("Uso: %s <Numero de Puerto (clientes)> <Numero de Puerto (servidores)>\n", port[0]);
        exit(0);
    }

    argumento *arg=(argumento*)malloc(sizeof(argumento));
    strcpy(arg->pais  ,"VE");
    strcpy(arg->city  ,"CCS");
    strcpy(arg->region,"CCS");
    strcpy(arg->org   ,"symantec");
    strcpy(arg->dep   ,"symantec");
    strcpy(arg->name  ,"symantec");
    strcpy(arg->mail  ,"symantec");

    certificado_autofirmado(certificado_cai,archivoKEY,1024,0,365,arg);
    
    portnumc= atoi(port[1]);
    portnums= atoi(port[2]);
    pthread_create( &hilo1, NULL, AtencionCliente , (void *)&portnumc);
    pthread_create( &hilo2, NULL, ServDescargas , (void *)&portnums);
    
    pthread_join ( hilo1 , NULL ) ;
    pthread_join ( hilo2 , NULL ) ;
    
    free(arg);
    return 0;
}
