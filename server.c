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

#include "cola.h"

#define MAXCONECTION 5
#define BUFFSIZE 32

pthread_t tid[2];

void* foobarbaz(void *args)
{
   int num = *((int *) args);
   printf("espero %d \n", num);
   sleep(num);
   char str[3];
   sprintf(str,"soy el %d\n",num);
   colaAgregar(str);
};

void HandleClient(int sock)
{
   char recvBuff[BUFFSIZE];
   int n = -1;

   if((n = recv(sock, recvBuff, BUFFSIZE, 0)) < 0)
   {
      fprintf(stderr,"\nError : Error en numero recibido\n");
      exit(1);
   }

   while( n > 0 ){
      if(send(sock, recvBuff, n, 0) != n){
	 fprintf(stderr,"\nError : Error enviando al cliente\n");
	 exit(1);
      }
      if((n = recv(sock, recvBuff, BUFFSIZE, 0)) < 0)
      {
	 fprintf(stderr,"\nError : Error en numero recibido\n");
	 exit(1);
      }
   }
   close(sock);
}

int main(int argc, char *argv[])
{
   /* int i = 10; */
   /* pthread_t t1, t2; */
   /* pthread_create(&(tid[0]),NULL, foobarbaz, (void *) &i); */

   /* printf("esperando el hijo\n"); */
   /* pthread_join(tid[0], NULL); */
   /* printf("Chao\n"); */

   int serverfd = 0, clientfd = 0, n = 0;
   struct sockaddr_in serv_addr, client_addr;

   time_t ticks;

   if (argc != 2) {
      fprintf(stderr, "USAGE: server <port>\n");
      exit(1);
   }
   
   serverfd = socket(AF_INET, SOCK_STREAM, 0);
   memset(&serv_addr, '0', sizeof(serv_addr));

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_addr.sin_port = htons(atoi(argv[1]));

   if(bind(serverfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
   {
      fprintf(stderr, "Bind failed");
      exit(1);
   }

   if( listen(serverfd, MAXCONECTION) < 0)
   {
      fprintf(stderr, "Listen failed");
      exit(1);      
   }

   while(1){
      unsigned int clientlen = sizeof(client_addr);
      if((clientfd = accept(serverfd, (struct sockaddr *) &client_addr, &clientlen)) < 0)
      {
	 fprintf(stderr, "Accept failed\n");
	 exit(1);
      }
      printf("Cliente conectado: %s\n", inet_ntoa(client_addr.sin_addr));
      HandleClient(clientfd);
   }
   
      
      
      /* close(connfd); */
   /*    sleep(1); */
   /* } */
   
}
