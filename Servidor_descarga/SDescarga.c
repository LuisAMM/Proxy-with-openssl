#include "funciones.h"


void* estadistica(void* sock){
  int sockfd=*(int*)sock;
  char buffer[1024];
  while(1)
  {
    if(send( sockfd, buffer, 1024, 0 )==-1){
      perror("Error");
    }

  strcpy(buffer,"");

    if(recv( sockfd, buffer, 1024, 0 )==-1){
      perror("error");      
    }    


  }

}


void *atender(void* cliente) {
    
  char buf[1024];
  char buf1[1024];
  char bufaux[1024];
  char* buf2="Operacion finalizada\n";
  int bytes;
  FILE *f;
  //int tam;
  //int bloaux;
  //int blo;
  char bloack[10000];
  SSL_CTX *ctx;
  SSL *ssl;
  int sd;
  int sockaux=*(int*)cliente;
  ctx = InitServerCTX(); 
  LoadCertificates(ctx, "firmado.pem", "salida.key"); /* load certs */
  ssl = SSL_new(ctx);              /* get new SSL state with context */
  SSL_set_fd(ssl, sockaux);      /* set connection socket to SSL state */
  printf("hola\n");
  if ( SSL_accept(ssl) == FAIL )     /* do SSL-protocol accept */
      ERR_print_errors_fp(stderr);

  bytes = SSL_read(ssl, buf, sizeof(buf)); /* get reply & decrypt */
  buf[bytes] = 0;
  printf("%s\n",buf );
  sprintf( buf1,"/home/luis/Proyecto/Servidor_descarga/videos/%s",buf);
  printf("%s\n",buf1 );


  f=fopen(buf1,"rb");
  if(f==NULL){
    perror("Error:");
    sd = SSL_get_fd(ssl);       /* get socket connection */
    SSL_free(ssl);         /* release SSL state */
    close(sd);          /* close connection */
    pthread_exit(NULL);

  }
  else{
      //fseek(f, 0L, SEEK_END);
    //tam=ftell(f);
    //rewind(f);
    //if (tam%1024==0)
    //  blo=tam/1024;
    //else{
    //  blo=tam/1024+1;
    //}
    int ack;
    ack=0;
    //bloaux=1;
    while(!feof(f))
    {
        fread(bufaux,1, 1024, f);
        SSL_write(ssl, bufaux, 1024);
        //bytes = SSL_read(ssl, bloack, sizeof(bloack)); /* get reply & decrypt */
        bloack[bytes]=0;
        switch (SSL_get_error(ssl, bytes))
        { 
            case SSL_ERROR_NONE: 
                break;
            case SSL_ERROR_ZERO_RETURN: 
                fclose(f);
                sd = SSL_get_fd(ssl);       /* get socket connection */
                SSL_free(ssl);         /* release SSL state */
                close(sd);          /* close connection */
                pthread_exit(NULL);
            default: 
                fclose(f);
                sd = SSL_get_fd(ssl);       /* get socket connection */
                SSL_free(ssl);         /* release SSL state */
                close(sd);          /* close connection */
                pthread_exit(NULL);
        }
        printf("%s\n",bufaux );
        if(strcmp(bloack,"recibido")==0){
          ack=ack+1;
          printf("%d\n", ack);
    
        }
    //if(atoi(bloack)==bloaux)
    //  bloaux=bloaux+1;
    memset(bufaux,0,sizeof(bufaux));
    //if(bloaux==blo)
    //  break;
    }

    SSL_write(ssl, buf2, strlen(buf2));
    fclose(f);
    sd = SSL_get_fd(ssl);       /* get socket connection */
    SSL_free(ssl);         /* release SSL state */
    close(sd);          /* close connection */
    pthread_exit(NULL);


  }



}



void* Central(void* client){

  pthread_t hilo, hilo1;
  int server;
	sockete aux = *(sockete*)client;
  argumento *arg=(argumento*)malloc(sizeof(argumento));
  int PUERTO;
	strcpy(arg->pais  ,"ca");
	strcpy(arg->city  ,"ca");
	strcpy(arg->region,"ca");
	strcpy(arg->org   ,"luis");
	strcpy(arg->dep   ,"luis");
	strcpy(arg->name  ,"luis");
	strcpy(arg->mail  ,"luis");
	csr ("salida.crt", "salida.key", 1024, 1, 1, arg);
	
	struct  sockaddr_in CLNT_addr; 
	int PORTNUMBER=aux.sd;                  //Puerto especificado por el cliente            
	char buffer [BUFSIZE];
	int     sockfd;                   /* Client socket   descriptor */   
	if( ( sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 ) 
  {     
    perror( "Error" );   
    exit(1);
  }
  
        
  CLNT_addr.sin_family = AF_INET;                                     
	CLNT_addr.sin_port   = htons( PORTNUMBER );                    
	inet_pton( AF_INET, aux.ip, &CLNT_addr.sin_addr ); 
	if( connect( sockfd, (struct sockaddr*)&CLNT_addr, sizeof( CLNT_addr ) ) ==-1 ){          //Se conecta con el servidor
		perror("Error");                                    
		exit(5);
	}


	FILE* fd=fopen("salida.crt","r");
  fread(buffer,sizeof(char),1024,fd);
  fclose(fd);


  if(send( sockfd, buffer, 1024, 0 )==-1){
    perror("Error");
	}

	strcpy(buffer,"");

	if(recv( sockfd, buffer, 1024, 0 )==-1){
		perror("error");			
	}             

	fd = fopen("firmado.pem","w");
  fprintf(fd,"%s",buffer);
  fclose(fd);
  
  memset(buffer,0,sizeof(buffer));
   
  //Se obtiene la lista de archivos disponibles
  fd=fopen("lista","r");
  fread (buffer , 1 ,sizeof(buffer) , fd);    
  fclose (fd);

  if(send( sockfd, buffer, sizeof(buffer), 0 )==-1){
    perror("Error");
  }
   
  memset(buffer,0,sizeof(buffer));

  if(recv( sockfd, buffer, 1024, 0 )==-1){
     perror("error");        
  }
  PUERTO=atoi(buffer);

   
  printf("Mi puerto es: %d\n",PUERTO);

  SSL_CTX *ctx;

  pthread_create(&hilo1, NULL, estadistica , (void *)&sockfd);

  server = OpenListener(PUERTO);    /* create server socket */
  
  while (1)
  {   
      struct sockaddr_in addr;
      socklen_t len = sizeof(addr);
      int client = accept(server, (struct sockaddr*)&addr, &len);  /* accept connection as usual */
      printf("Connection: %s:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
      pthread_create(&hilo, NULL, atender , (void *)&client); 
  }
  close(server);          /* close server socket */
  SSL_CTX_free(ctx); 

  //close(sockfd);

  pthread_exit(NULL);
}




int main(int argc, char  *argv[]){
	   
    
    char *portnum;
    pthread_t hilo1;
    if(!isRoot())
    {
        printf("This program must be run as root/sudo user!!");
        exit(0);
    }
    if ( argc != 3 )
    {
        printf("Usage: %s <portnum>\n", argv[0]);
        exit(0);
    }
    SSL_library_init();
    
    portnum = argv[2];
    int port=atoi(portnum);
    
    sockete aux;
    strcpy(aux.ip,argv[1]);
    aux.sd=port;

    pthread_create(&hilo1, NULL, Central , (void *)&aux);

    pthread_join(hilo1, NULL);
	
	return 0;
}