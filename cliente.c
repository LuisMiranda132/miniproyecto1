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
   int sockfd = 0, n = 0;
   char recvBuff[BUFFSIZE];
   char sendBuff[1024];
   struct sockaddr_in serv_addr;

   if (argc != 4) {
      fprintf(stderr, "USAGE: cliente <server_ip> <word> <port>\n");
      exit(1);
   }
   
   if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
   {
      fprintf(stderr, "\nError: No lo pude crear\n");
      exit(1);
   }

   memset(&serv_addr, '0', sizeof(serv_addr));

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(atoi(argv[3]));

   // convert text to IP address 
   if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
   {
      fprintf(stderr,"\nError: inet");
      exit(1);
   }

   if( connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
   {
      fprintf(stderr,"\nError : Connect Failed\n");
      exit(1);
   }

   printf("Enviando\n");
   int msjsize = strlen(argv[2]);
   printf("%s y mide %d\n", argv[2], msjsize);
   
   if(send(sockfd, argv[2], msjsize, 0) != msjsize){
      fprintf(stderr,"\nError : Error en numero de envio\n");
      exit(1);
   }

   printf("Recibiendo: ");
   while( n < msjsize){
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
