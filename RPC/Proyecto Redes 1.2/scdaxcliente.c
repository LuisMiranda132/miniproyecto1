/*scdaxcliente.c*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <rpc/rpc.h>
#include "scdax.h"

#define MAXSIZE 5000

int main (int argc, char *argv[]) {
	CLIENT *cl;
	int size, option = 0;
	char *server, *ddespl, *filename;
	struct to_encrypt input;
	struct to_encrypt *output;
	FILE *file;
	//char *msg = "HOLAMUNDOCRUEL";

	if (argc != 9) {
		fprintf(stderr,"Uso: %s -i <dir_ip> -c <clave> -a <dir_despl> -f <arch_input>\n",argv[0]);
		exit(0);
	}

	while ((option = getopt(argc, argv, "i:c:a:f:")) != -1) {
		switch (option) {
			case 'i' : server = optarg;
				break;

			case 'c' : input.key = atoi(optarg);
				break;

			case 'a' : ddespl = optarg;
				break;

			case 'f' : filename = optarg;
				break;

			default: fprintf(stderr,"Uso: %s -i <dir_ip> -c <clave> -a <dir_despl> -f <arch_input>\n",argv[0]);
				exit(0);
		}
	}

	//server = argv[2];

	//input.key = strtol(argv[4],NULL,10);

	if (input.key<1 || input.key>27) {
		fprintf(stderr,"La clave debe ser un entero entre 1 y 27\n");
		exit(0);
	}

	//ddespl = argv[6];

	if (strcmp(ddespl,"derecha")==0) {
		input.key = -input.key;
	} else if (strcmp(ddespl,"izquierda")!=0) {
		fprintf(stderr,"La direccion de desplazamiento debe ser derecha o izquierda\n");
		exit(0);
	}

	//filename = argv[8];

	file = fopen(filename,"r");


	fseek(file, 0, SEEK_END);
	size = ftell(file);
	rewind(file);

	if (size > MAXSIZE) {
		fprintf(stderr,"El archivo de entrada es de tamaño mayor al soportado\n");
		exit(0);
	}
	//input.text = msg;
	input.text = (char*) malloc (sizeof(char)*size);
	if (fread(input.text, 1, size, file) != size) {
		fprintf(stderr, "Error leyendo el archivo\n");
		exit(1);
	}

	cl = clnt_create(server, SCDAXPROG, SCDAXVERS, "tcp");

	if (cl == (CLIENT *)NULL) {
		clnt_pcreateerror(server);
		exit(1);
	}

	output = dothething_1 (&input,cl);

	switch (output->key) {
		case 28 : 
			fprintf(stderr, "Error de reserva de memoria en el servicio\n");
			exit(1);
		case -1 :
			printf("Error, el archivo contiene caracteres no soportados\n");
	}

	printf(output->text);
	file = fopen("salida.txt","w");
	fprintf(file, output->text);


	return 0;
}