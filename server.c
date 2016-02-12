#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <getopt.h>
#include <signal.h>

#include "crip.h"

#define MAXCONECTION 5
#define BUFFSIZE 1024

pthread_t tid[MAXCONECTION];
FILE *file;

void signal_exit(int signum)
{
   fclose(file);
   exit(signum);
}

//void HandleClient(int sock)
void* HandleClient(void *args)
{
   int sock = *((int *) args);
   char recvBuff[BUFFSIZE];
   int n = 0, num, clave, m = -1, chk;
   char *result;

   if((m = recv(sock, &clave, sizeof(int), 0)) < 0)
   {
      fprintf(stderr,"\nError : Error en numero recibido\n");
      exit(1);
   }
   printf("la clave es: %d\n",clave);
      
   if((m = recv(sock, &num, sizeof(int), 0)) < 0)
   {
      fprintf(stderr,"\nError : Error en numero recibido\n");
      exit(1);
   }
   
   printf("Voy a recibir %d\n", num);

   printf("Recibiendo: ");
   while( n < num){
      int bytes = 0;
      if ((bytes = recv(sock, recvBuff, BUFFSIZE-1, 0)) < 1) {
   	 fprintf(stderr,"\nError : Failed to receive bytes %d\n", bytes);
   	 exit(1);
      }
      n += bytes;
      recvBuff[bytes] = '\0';        /* Assure null terminated string */
   }

   chk = check(recvBuff);
   if(chk == -1){
      printf("esto no lo hice yo chamin\n");
   }else if(chk){
      printf("voy a desencriptarlo\n");
      printf(recvBuff);
      printf("\n");
      stganograf(recvBuff);
      printf(recvBuff);
      printf("\n");
      result = decrypt(recvBuff, clave);
      printf(result);
      printf("\n");      
   }else{
      printf("voy a encriptarlo\n");
      printf(recvBuff);
      printf("\n");
      result = encrypt(recvBuff, clave);
      printf(result);
      printf("\n");
      stganograf(result);
      printf(result);
      printf("\n");
   }
   
//   sleep(5);
   
   printf("Enviando respuesta\n");
   if(send(sock, result, num, 0) != num){
      fprintf(stderr,"\nError : Error enviando al cliente\n");
      exit(1);
   }
   printf("Respuesta enviada\n");
         
   /* while( n > 0 ){ */
   /*    if(send(sock, recvBuff, n, 0) != n){ */
   /* 	 fprintf(stderr,"\nError : Error enviando al cliente\n"); */
   /* 	 exit(1); */
   /*    } */
   /*    if((n = recv(sock, recvBuff, BUFFSIZE, 0)) < 0) */
   /*    { */
   /* 	 fprintf(stderr,"\nError : Error en numero recibido\n"); */
   /* 	 exit(1); */
   /*    } */
   /* } */
   close(sock);
}


int main(int argc, char *argv[])
{
   /* int i = 10; */
   pthread_t t1;
   /* pthread_create(&(tid[0]),NULL, foobarbaz, (void *) &i); */

   /* printf("esperando el hijo\n"); */
   /* pthread_join(tid[0], NULL); */
   /* printf("Chao\n"); */

   int serverfd = 0, clientfd = 0, n = 0;
   struct sockaddr_in serv_addr, client_addr;
   int port, option;

   time_t ticks;

   signal(SIGINT, signal_exit);
   
   printf("%d\n",argc);
   if (argc != 5) {
      fprintf(stderr,"Uso: scdax_svr -l <puerto_scdax_svr> -b <archivo_bitácora>\n"); 
      exit(1);
   }
   
   while((option = getopt(argc, argv, "l:b:")) != -1)
   {
      switch(option){
      case 'l' : port = atoi(optarg);
	 break;
      case 'b' : file = fopen(optarg,"w");
	 break;
      default: fprintf(stderr,"Uso: scdax_svr -l <puerto_scdax_svr> -b <archivo_bitácora>\n");
		exit(0);
      }
      
   }

   fprintf(file, "prueba\n");
   
   serverfd = socket(AF_INET, SOCK_STREAM, 0);
   printf("logre socket");
   
   memset(&serv_addr, '0', sizeof(serv_addr));

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_addr.sin_port = htons(port);

   if(bind(serverfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
   {
      fprintf(stderr, "Bind failed");
      exit(1);
   }
   
   printf("logre bind");
   
   if( listen(serverfd, MAXCONECTION) < 0)
   {
      fprintf(stderr, "Listen failed");
      exit(1);      
   }

   printf("logre listen");

   while(1){
      unsigned int clientlen = sizeof(client_addr);
      if((clientfd = accept(serverfd, (struct sockaddr *) &client_addr, &clientlen)) < 0)
      {
	 fprintf(stderr, "Accept failed\n");
	 exit(1);
      }
      printf("Cliente conectado: %s\n", inet_ntoa(client_addr.sin_addr));
      pthread_create(&t1,NULL, HandleClient, (void *) &clientfd);
//      HandleClient(clientfd);
   }
   
      
      
      /* close(connfd); */
   /*    sleep(1); */
   /* } */
   
}
