#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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

char* encrypt(char *text, int key) 
{
	int i, l = strlen(text);
	int ncount = 0; // Contar las Ñ originales
	int ncount2 = 0; // Contar las Ñ finales
	char *code = (char*)malloc(l*sizeof(char)); 

	if (code == NULL) {
		printf("Error al reservar memoria");
		error(0);
	}

	// Manipulacion de la clave
	key = key%27;
	if (key == 0)
		return;
	if (key < 0)
		key = key+27;


	for (i = 0; i < l; i++) {
		if (65 <= text[i] && text[i] <= 78) { // A-N
			code[i-ncount] = text[i] - 65;
			
		} else if (79 <= text[i] && text[i] <= 90) { // O-Z
			code[i-ncount] = text[i] - 64;

		} else if (text[i] == -61) { // Ñ (consume dos valores del arreglo)
			code[i-ncount] = 14;
			ncount++;
			i++;
		} else {
			code[i-ncount] = text[i];
		}
	}

	for (i = 0; i < l - ncount; i++) {
		if (code[i] < 27) {
			code[i] = (code[i] + key) % 27;
			if (code[i] == 14) ncount2++;
	    }
	}

	char *data = (char*)malloc((l-ncount+ncount2)*sizeof(char));
	if (data == NULL) {
		printf("Error al reservar memoria");
		error(0);
	}
	int ncount3 = 0; // The recount

	for (i = 0; i < l - ncount + ncount2; i++) {
		if (0 <= code[i-ncount3] && code[i-ncount3] <= 13) { // A-N
			data[i] = code[i-ncount3] + 65;
		} else if (15 <= code[i-ncount3] && code[i-ncount3] <= 26) { // O-Z
			data[i] = code[i-ncount3] + 64;
		} else if (code[i-ncount3] == 14){ // Ñ
			data[i] = -61; //-61    0xFFFFFFC3
			i++;
			data[i] = -111; //-111   0xFFFFFF91
			ncount3++;
		} else {
			data[i] = code[i-ncount3];
		}
	}
	return data;
}

char* decrypt(char *text, int key) 
{
	encrypt(text,-key);
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
