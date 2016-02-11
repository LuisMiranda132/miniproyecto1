#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define BUFFSIZE 1024

int main(int argc, char *argv[])
{
   int sockfd = 0, n = 0, clave;
   char recvBuff[BUFFSIZE];
   char sendBuff[1024];
   FILE *file;
   char *buffer;
   long size;
    
   struct sockaddr_in serv_addr;

   if (argc != 5) {
      fprintf(stderr, "USAGE: cliente <server_ip> <file> <port> <clave> \n");
      exit(1);
   }

   file = fopen(argv[2],"r");
   fseek(file, 0, SEEK_END);
   size = ftell(file);
   rewind(file);

   clave = atoi(argv[4]);
   
   buffer = (char*) malloc (sizeof(char)*size);

   if( fread (buffer, 1, size, file) != size)
   {
      fprintf(stderr, "\nError: leyendo ar chivo\n");
      exit(1);
   }

   printf("El buffer: ");
   printf(buffer);
   printf("\n");
   
   if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
   {
      fprintf(stderr, "\nError: No lo pude crear\n");
      exit(1);
   }

   printf("cree el socket\n");
   
   memset(&serv_addr, '0', sizeof(serv_addr));

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(atoi(argv[3]));

   // convert text to IP address 
   if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
   {
      fprintf(stderr,"\nError: inet");
      exit(1);
   }

   printf("logre inet\n");
   
   if( connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
   {
      fprintf(stderr,"\nError : Connect Failed\n");
      exit(1);
   }
   printf("logre connect\n");

   printf("Enviando\n");
   printf("%s y mide %d\n", argv[2], size);

   if(send(sockfd, &clave, sizeof(int), 0) != sizeof(int)){
      fprintf(stderr,"\nError : Error en numero de envio\n");
      exit(1);
   }
   printf("envie la clave\n");
   
   if(send(sockfd, &size, sizeof(int), 0) != sizeof(int)){
      fprintf(stderr,"\nError : Error en numero de envio\n");
      exit(1);
   }

   printf("Voy con el texto\n");      
   if(send(sockfd, buffer, size, 0) != size){
      fprintf(stderr,"\nError : Error en numero de envio txt\n");
      exit(1);
   }

   printf("enviado\n");
   
   printf("Recibiendo: ");
   while( n < size){
      int bytes = 0;
      if ((bytes = recv(sockfd, recvBuff, BUFFSIZE-1, 0)) < 1) {
   	 fprintf(stderr,"\nError : Failed to receive bytes from server\n");
   	 exit(1);
      }
      n += bytes;
      recvBuff[bytes] = '\0';        /* Assure null terminated string */
      printf(recvBuff);
   }

   printf("\n");
   close(sockfd);
   exit(0);
   
}
