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
#include <time.h>

#include "crip.h"

/**
 * @author Luis Miranda 10-10463
 * @author Oskar Gonzales 09-10351
 *
 * Codigo del cliente de SCDAX
 *
 */

#define MAXCONECTION 5
#define BUFFSIZE 1024

pthread_t tid[MAXCONECTION];
FILE *file;

time_t t;
struct tm tm;

// estructura para pasar datos al hilo
struct arg_struct
{
   char* ip;
   int clientfd;
};

// se encarga de guardar todo cuando se cierra el servidor
void signal_exit(int signum)
{
   fclose(file);
   exit(signum);
}

/**
 * Funcion se encarga de la comunicacion con un cliente cuando este se conecte
 */
void* HandleClient(void *args)
{
   struct arg_struct *dummy = args;
   int sock = dummy->clientfd;
   char recvBuff[BUFFSIZE];
   int n = 0, num, clave, m = -1, chk;
   char *result;

   if((m = recv(sock, &clave, sizeof(int), 0)) < 0)
   {
      t = time(NULL);
      tm = *localtime(&t);
      fprintf(file, "[%d-%d-%d %d:%d:%d] [cliente %s] [error] Error recibiendo datos\n", tm.tm_mday,
	     tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, dummy->ip);
      close(sock);
      return;
   }
   t = time(NULL);
   tm = *localtime(&t);
   fprintf(file, "[%d-%d-%d %d:%d:%d] [cliente %s] Recibi clave\n", tm.tm_mday, tm.tm_mon + 1,
	  tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, dummy->ip);
      
   if((m = recv(sock, &num, sizeof(int), 0)) < 0)
   {
      t = time(NULL);
      tm = *localtime(&t);
      fprintf(file, "[%d-%d-%d %d:%d:%d] [cliente %s] [error] Error recibiendo datos\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, dummy->ip);
      close(sock);
      return;
   }
   
   t = time(NULL);
   tm = *localtime(&t);
   fprintf(file, "[%d-%d-%d %d:%d:%d] [cliente %s] Recibi numero\n", tm.tm_mday, tm.tm_mon + 1,
	  tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, dummy->ip);
   while( n < num){
      int bytes = 0;
      if ((bytes = recv(sock, recvBuff, BUFFSIZE-1, 0)) < 1) {
	 t = time(NULL);
	 tm = *localtime(&t);
	 fprintf(file, "[%d-%d-%d %d:%d:%d] [cliente %s] [error] Error recibiendo datos\n", tm.tm_mday,
		tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, dummy->ip);
	 close(sock);
	 return;
      }
      n += bytes;
      recvBuff[bytes] = '\0';
   }

   t = time(NULL);
   tm = *localtime(&t);
   fprintf(file, "[%d-%d-%d %d:%d:%d] [cliente %s] Recibi texto\n", tm.tm_mday, tm.tm_mon + 1,
	  tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, dummy->ip);


   chk = check(recvBuff);
   if(chk == -1){
      t = time(NULL);
      tm = *localtime(&t);
      fprintf(file, "[%d-%d-%d %d:%d:%d] [cliente %s] El texto no es valido\n", tm.tm_mday, tm.tm_mon + 1,
	     tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, dummy->ip);
      num = 0;
   }else if(chk){
      t = time(NULL);
      tm = *localtime(&t);
      fprintf(file, "[%d-%d-%d %d:%d:%d] [cliente %s] El texto va a ser desencriptado\n", tm.tm_mday,
	     tm.tm_mon + 1,tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, dummy->ip);
      stganograf(recvBuff);
      result = decrypt(recvBuff, clave);
   }else{
      t = time(NULL);
      tm = *localtime(&t);
      fprintf(file, "[%d-%d-%d %d:%d:%d] [cliente %s] El texto va a ser encriptado\n", tm.tm_mday,
	     tm.tm_mon + 1,tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, dummy->ip);
      result = encrypt(recvBuff, clave);
      stganograf(result);
   }

   if(send(sock, &num, sizeof(int), 0) != sizeof(int)){
      t = time(NULL);
      tm = *localtime(&t);
      fprintf(file, "[%d-%d-%d %d:%d:%d] [cliente %s] [error] Error enviando datos\n", tm.tm_mday,
	     tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, dummy->ip);
      close(sock);
      return;
   }

   if(!num){
      close(sock);
      return ;
   }
   
   if(send(sock, result, num, 0) != num){
      t = time(NULL);
      tm = *localtime(&t);
      fprintf(file, "[%d-%d-%d %d:%d:%d] [cliente %s] [error] Error enviando datos\n", tm.tm_mday,
	     tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, dummy->ip);
      close(sock);
      return;
   }
   t = time(NULL);
   tm = *localtime(&t);
   fprintf(file, "[%d-%d-%d %d:%d:%d] [cliente %s] El texto fue enviado\n", tm.tm_mday,
	  tm.tm_mon + 1,tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, dummy->ip);

   close(sock);
}

/**
 * Funcion main del cliente
 */
int main(int argc, char *argv[])
{
   pthread_t t1;

   int serverfd = 0, clientfd = 0, n = 0;
   struct sockaddr_in serv_addr, client_addr;
   int port, option;
   struct arg_struct argumentos;
   
   time_t ticks; 
   signal(SIGINT, signal_exit);
   
   printf("%d\n",argc);
   if (argc != 5) {
      fprintf(stderr,"Uso: scdax_svr -l <puerto_scdax_svr> -b <archivo_bitácora>\n"); 
      exit(1);
   }

   // Recibe los argumentos 
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

   // creacion del socket 
   serverfd = socket(AF_INET, SOCK_STREAM, 0);
   t = time(NULL);
   tm = *localtime(&t);
   fprintf(file, "[%d-%d-%d %d:%d:%d] Logre abrir socket\n", tm.tm_mday,
	  tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
   
   memset(&serv_addr, '0', sizeof(serv_addr));

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_addr.sin_port = htons(port);

   if(bind(serverfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
   {
      t = time(NULL);
      tm = *localtime(&t);
      fprintf(file, "[%d-%d-%d %d:%d:%d] [Error] Falle bind\n", tm.tm_mday,
	     tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
      exit(1);
   }
   
   t = time(NULL);
   tm = *localtime(&t);
   fprintf(file, "[%d-%d-%d %d:%d:%d] Logre bind\n", tm.tm_mday,
	  tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

   if( listen(serverfd, MAXCONECTION) < 0)
   {
      t = time(NULL);
      tm = *localtime(&t);
      fprintf(file, "[%d-%d-%d %d:%d:%d] [Error] Falle listen\n", tm.tm_mday,
	     tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
      exit(1);
   }
   t = time(NULL);
   tm = *localtime(&t);
   fprintf(file, "[%d-%d-%d %d:%d:%d] Logre listen\n", tm.tm_mday,
	  tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

   // espera por clientes
   while(1){
      unsigned int clientlen = sizeof(client_addr);
      if((argumentos.clientfd = accept(serverfd, (struct sockaddr *) &client_addr, &clientlen)) < 0)
      {
	 t = time(NULL);
	 tm = *localtime(&t);
	 fprintf(file, "[%d-%d-%d %d:%d:%d] [Error] Falle accept\n", tm.tm_mday,
		tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
      }
      argumentos.ip = inet_ntoa(client_addr.sin_addr);
      t = time(NULL);
      tm = *localtime(&t);
      fprintf(file, "[%d-%d-%d %d:%d:%d] [cliente %s] Cliente conectado\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, argumentos.ip);
      pthread_create(&t1,NULL, HandleClient, (void *) &argumentos);
   }
}
