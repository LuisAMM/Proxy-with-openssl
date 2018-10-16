#include "funciones.h"

int OpenListener(int port){   
    int sd;
    struct sockaddr_in addr;
 
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if ( bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        perror("can't bind port");
        abort();
    }
    if ( listen(sd, 10) != 0 )
    {
        perror("Can't configure listening port");
        abort();
    }
    return sd;
}
 
int isRoot(){
    if (getuid() != 0)
    {
        return 0;
    }
    else
    {
        return 1;
    } 
}

SSL_CTX* InitServerCTX(void){   SSL_METHOD *method;
    SSL_CTX *ctx;
 
    OpenSSL_add_all_algorithms();  /* load & register all cryptos, etc. */
    SSL_load_error_strings();   /* load all error messages */
    method = TLSv1_2_server_method();  /* create new server-method instance */
    ctx = SSL_CTX_new(method);   /* create new context from method */
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}
 
void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile){
    /* set the local certificate from CertFile */
    if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* set the private key from KeyFile (may be the same as CertFile) */
    if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* verify private key */
    if ( !SSL_CTX_check_private_key(ctx) )
    {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }
}
 
void ShowCerts(SSL* ssl){   
    X509 *cert;
    char *line;
 
    cert = SSL_get_peer_certificate(ssl); /* Get certificates (if available) */
    if ( cert != NULL )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);
        X509_free(cert);
    }
    else
        printf("No certificates.\n");
}
 
int agreUsuario(char* login, char* cont){
    FILE* f;
    f=fopen("usuarios.txt","a+");
    char cont1[40], login1[40];
    sprintf(login1," %s ", login);
    sprintf(cont1," %s ", cont);
    fputs(login1,f);
    fputs(cont,f);
    fclose(f);
    return 1;
}

int busLogin(char* login){

    char buf[1024];
    FILE* f;
    f=fopen("usuarios.txt","r");
    while(!feof(f))
    {
        fscanf(f,"%s",buf);
        if(!strcmp(login,buf))
        {
            fclose(f);
                return 1;
        }
        else
            fscanf(f,"%s",buf);

    }
    fclose(f);
    return 0;
}

int buscUsuario(char* login, char* cont){
    char buf[1024];
    FILE* f;
    f=fopen("usuarios.txt","r");
    while(!feof(f))
    {
        fscanf(f,"%s",buf);
        if(!strcmp(login,buf))
        {
            fscanf(f,"%s",buf);
           
            if(!strcmp(cont,buf))
            {    
                fclose(f);
                return 1;
            }
            else
            {    
                fclose(f);
                return 0;
            }
        }
        else
            fscanf(f,"%s",buf);

    }
    fclose(f);
    return 0;
}

int buscarVideo(char* video, char* ip, char* port){
    char buf[1024];
    FILE* f;
    f=fopen("videos.txt","r");
    while(!feof(f))
    {
        fscanf(f,"%s",buf);
        
        if(!strcmp(video,buf))
        {
            fscanf(f,"%s",ip);
            fscanf(f,"%s",port);
            fclose(f);
            return 1;
        }
        
        else
        {

            fscanf(f,"%s",buf);
            fscanf(f,"%s",buf);
        }

    }
    fclose(f);
    return 0;

}

int verificarNum(char* cont){
    int i;
    for(i=0;i<strlen(cont);i++){
        if(isdigit(cont[i])) 
            return 1;
    }
    return 0;

}

int verificarMay(char* cont){
    int i;
    for(i=0;i<strlen(cont);i++){
        if(cont[i]=='A' ||
           cont[i]=='B' ||
           cont[i]=='C' ||
           cont[i]=='D' ||
           cont[i]=='E' ||
           cont[i]=='F' ||
           cont[i]=='G' ||
           cont[i]=='H' ||
           cont[i]=='I' ||
           cont[i]=='J' ||
           cont[i]=='K' ||
           cont[i]=='L' ||
           cont[i]=='M' ||
           cont[i]=='N' ||
           cont[i]=='O' ||
           cont[i]=='P' ||
           cont[i]=='Q' ||
           cont[i]=='R' ||
           cont[i]=='S' ||
           cont[i]=='T' ||
           cont[i]=='U' ||
           cont[i]=='V' ||
           cont[i]=='W' ||
           cont[i]=='X' ||
           cont[i]=='Y' ||
           cont[i]=='Z')
            return 1;
    }
    return 0;
}

int verificarCar(char* cont){
    int i;
    for(i=0;i<strlen(cont);i++){
        if(cont[i]!='A' && cont[i]!='a' && cont[i]!='1' && 
           cont[i]!='B' && cont[i]!='b' && cont[i]!='2' &&
           cont[i]!='C' && cont[i]!='c' && cont[i]!='3' &&
           cont[i]!='D' && cont[i]!='d' && cont[i]!='4' &&
           cont[i]!='E' && cont[i]!='e' && cont[i]!='5' &&
           cont[i]!='F' && cont[i]!='f' && cont[i]!='6' &&
           cont[i]!='G' && cont[i]!='g' && cont[i]!='7' &&
           cont[i]!='H' && cont[i]!='h' && cont[i]!='8' &&
           cont[i]!='I' && cont[i]!='i' && cont[i]!='9' &&
           cont[i]!='J' && cont[i]!='j' && cont[i]!='0' &&
           cont[i]!='K' && cont[i]!='k' &&
           cont[i]!='L' && cont[i]!='l' &&
           cont[i]!='M' && cont[i]!='m' &&
           cont[i]!='N' && cont[i]!='n' &&
           cont[i]!='O' && cont[i]!='o' &&
           cont[i]!='P' && cont[i]!='p' &&
           cont[i]!='Q' && cont[i]!='q' &&
           cont[i]!='R' && cont[i]!='r' &&
           cont[i]!='S' && cont[i]!='s' &&
           cont[i]!='T' && cont[i]!='t' &&
           cont[i]!='U' && cont[i]!='u' &&
           cont[i]!='V' && cont[i]!='v' &&
           cont[i]!='W' && cont[i]!='w' &&
           cont[i]!='X' && cont[i]!='x' &&
           cont[i]!='Y' && cont[i]!='y' &&
           cont[i]!='Z' && cont[i]!='z' )
         return 1;
    }
    return 0;
}

int verificarCont(char* cont){
    if(verificarNum(cont) && verificarCar(cont) && verificarMay(cont) && (strlen(cont)>=8) )
        return 1;
    else 
        return 0;
}

int firma(char *archivo_cert, char* archivo_clave, char* archivo_firmado, char* archivo_CAI)
{
    FILE * fp,*fp_clave;
    X509 * x=X509_new();
    X509 * cai=X509_new();
    EVP_PKEY * clavecai= EVP_PKEY_new();
    X509_NAME * nombre=NULL;
    
    // Abrimos los fichero
    BIO* bio_cert = BIO_new_file(archivo_cert, "rb");
    PEM_read_bio_X509(bio_cert, &x, NULL, NULL);
    
    BIO* bio_cai = BIO_new_file(archivo_CAI, "rb");
    PEM_read_bio_X509(bio_cai, &cai, NULL, NULL);
    
    if ((fp =fopen(archivo_firmado,"w")) == NULL) 
    {
        perror("ERROR al leer el fichero con el la nuevo");
        exit(1);
    }
    if ((fp_clave =fopen(archivo_clave,"r")) == NULL) 
    {
        perror("ERROR al leer el fichero con el la llave");
        exit(1);
    }

    nombre=X509_get_subject_name(cai);
    X509_set_issuer_name(x,nombre);

    X509_gmtime_adj(X509_get_notBefore(x),0);
    X509_gmtime_adj(X509_get_notAfter(x),(long)60*60*24*365);
   
    // Leemos la clave del cai
    PEM_read_PrivateKey(fp_clave,&clavecai,NULL,NULL);
    
    // Leemos el certificado

    X509_sign(x,clavecai,EVP_md5());
    
    PEM_write_X509(fp,x);
    
    BIO_get_close(bio_cai);
    BIO_get_close(bio_cert);
    fclose(fp);
    fclose(fp_clave);
    X509_free(x);
    X509_free(cai);
    EVP_PKEY_free(clavecai);
    return 0;
}    

int certificado_autofirmado (char *archivoCRT, char *archivoKEY, int Nrsa, int Nserie, int Ndias, argumento* arg)
{
   FILE * fp_cert,*fp_clave;
   X509 * x=NULL;
   EVP_PKEY * clave=NULL;
   RSA * rsa;
   X509_NAME * nombre=NULL;

   // Abrimos el fichero para escritura del certificado

    if ((fp_cert =fopen(archivoCRT,"w")) == NULL) {
        perror("ERROR al abrir el fichero del certificado");
        exit(1);
   }
   
   // Abrimos el fichero para escritura de la clave privada
    if ((fp_clave =fopen(archivoKEY,"w")) == NULL) {
        perror("ERROR al abrir el fichero de la clave privada");
        exit(1);
    }
   // Iniciación de variables y estructuras del programa

   x = X509_new();
   clave = EVP_PKEY_new();
   // Generación del par de llaves RSA

   rsa=RSA_generate_key(Nrsa,RSA_F4,NULL,NULL);

   // Asignación del par RSA a la estructura de clave privada EVP_PKEY

   EVP_PKEY_assign_RSA(clave,rsa);

   // Relleno de datos del certificado

   X509_set_version(x,2);
   ASN1_INTEGER_set(X509_get_serialNumber(x),Nserie);
   X509_gmtime_adj(X509_get_notBefore(x),0);
   X509_gmtime_adj(X509_get_notAfter(x),(long)60*60*24*Ndias);
   X509_set_pubkey(x,clave);

   // Relleno de nombres del certificado. Esto podria hacerse de varias formas:

   nombre=X509_get_subject_name(x);
   
   X509_NAME_add_entry_by_txt(nombre,"C",MBSTRING_ASC, (const unsigned char *)    arg->pais, -1, -1, 0);
   X509_NAME_add_entry_by_txt(nombre,"ST",MBSTRING_ASC, (const unsigned char *)   arg->city, -1, -1, 0);
   X509_NAME_add_entry_by_txt(nombre,"L",MBSTRING_ASC, (const unsigned char *)    arg->region, -1, -1, 0);
   X509_NAME_add_entry_by_txt(nombre,"O",MBSTRING_ASC, (const unsigned char *)    arg->org, -1, -1, 0);
   X509_NAME_add_entry_by_txt(nombre,"OU",MBSTRING_ASC, (const unsigned char *)   arg->dep, -1, -1, 0);
   X509_NAME_add_entry_by_txt(nombre,"CN",MBSTRING_ASC, (const unsigned char *)   arg->name, -1, -1, 0);
   X509_NAME_add_entry_by_txt(nombre,"Email",MBSTRING_ASC, (const unsigned char *)arg->mail, -1, -1, 0);

   X509_set_issuer_name(x,nombre);

   // Firmamos el certificado que hemos creado con nuestra propia clave
    
   X509_sign(x,clave,EVP_md5());
    
   // Escribimos el certificado y la clave privada en los ficheros especificados

   PEM_write_X509(fp_cert,x);
   PEM_write_PrivateKey(fp_clave,clave,NULL,NULL,0,NULL, NULL);

   // Cerramos ficheros y liberamos de la memoria las estructuras usadas

    fclose(fp_cert);
    fclose(fp_clave);
    X509_free(x);
    EVP_PKEY_free(clave);
    return 0;
}


int csr (char *archivoCRT, char *archivoKEY, int Nrsa, int Nserie, int Ndias, argumento* arg)
{
    FILE * fp_cert,*fp_clave;
    X509 * x=NULL;
    EVP_PKEY * clave=NULL;
    RSA * rsa;
    X509_NAME * nombre=NULL;

    // Abrimos el fichero para escritura del certificado

    if ((fp_cert =fopen(archivoCRT,"w")) == NULL) 
    {
        perror("ERROR al abrir el fichero del certificado");
        exit(1);
    }
   
   // Abrimos el fichero para escritura de la clave privada
    if ((fp_clave =fopen(archivoKEY,"w")) == NULL) 
    {
        perror("ERROR al abrir el fichero de la clave privada");
        exit(1);
    }
    // Iniciación de variables y estructuras del programa

    x = X509_new();
    clave = EVP_PKEY_new();
    // Generación del par de llaves RSA

    rsa=RSA_generate_key(Nrsa,RSA_F4,NULL,NULL);

    // Asignación del par RSA a la estructura de clave privada EVP_PKEY

    EVP_PKEY_assign_RSA(clave,rsa);

    // Relleno de datos del certificado

    X509_set_version(x,2);
    ASN1_INTEGER_set(X509_get_serialNumber(x),Nserie);
    X509_gmtime_adj(X509_get_notBefore(x),0);
    X509_gmtime_adj(X509_get_notAfter(x),(long)60*60*24*Ndias);
    X509_set_pubkey(x,clave);

    // Relleno de nombres del certificado. Esto podria hacerse de varias formas:

    nombre=X509_get_subject_name(x);
   
    X509_NAME_add_entry_by_txt(nombre,"C",MBSTRING_ASC, (const unsigned char *)    arg->pais, -1, -1, 0);
    X509_NAME_add_entry_by_txt(nombre,"ST",MBSTRING_ASC, (const unsigned char *)   arg->city, -1, -1, 0);
    X509_NAME_add_entry_by_txt(nombre,"L",MBSTRING_ASC, (const unsigned char *)    arg->region, -1, -1, 0);
    X509_NAME_add_entry_by_txt(nombre,"O",MBSTRING_ASC, (const unsigned char *)    arg->org, -1, -1, 0);
    X509_NAME_add_entry_by_txt(nombre,"OU",MBSTRING_ASC, (const unsigned char *)   arg->dep, -1, -1, 0);
    X509_NAME_add_entry_by_txt(nombre,"CN",MBSTRING_ASC, (const unsigned char *)   arg->name, -1, -1, 0);
    X509_NAME_add_entry_by_txt(nombre,"Email",MBSTRING_ASC, (const unsigned char *)arg->mail, -1, -1, 0);

    // Escribimos el certificado y la clave privada en los ficheros especificados

    PEM_write_X509(fp_cert,x);
    PEM_write_PrivateKey(fp_clave,clave,NULL,NULL,0,NULL, NULL);

    // Cerramos ficheros y liberamos de la memoria las estructuras usadas

    fclose(fp_cert);
    fclose(fp_clave);
    X509_free(x);
    EVP_PKEY_free(clave);
    return 0;
}

int OpenConnection(const char *hostname, int port)
{   int sd;
    struct hostent *host;
    struct sockaddr_in addr;
 
    if ( (host = gethostbyname(hostname)) == NULL )
    {
        perror(hostname);
        abort();
    }
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        close(sd);
        perror(hostname);
        abort();
    }
    return sd;
}
 
SSL_CTX* InitCTX(void)
{   SSL_METHOD *method;
    SSL_CTX *ctx;
 
    OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
    SSL_load_error_strings();   /* Bring in and register error messages */
    method = TLSv1_2_client_method();  /* Create new client-method instance */
    ctx = SSL_CTX_new(method);   /* Create new context */
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}