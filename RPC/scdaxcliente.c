/*scdaxcliente.c*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <rpc/rpc.h>
#include "scdax.h"

int main (int argc, char *argv[]) {
	CLIENT *cl;
	int option = 0;
	char *server, *ddespl, *filename;
	struct to_encrypt input;
	struct to_encrypt *output;

	char *msg = "HOLAMUNDOCRUEL";

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

	input.text = msg;

	cl = clnt_create(server, SCDAXPROG, SCDAXVERS, "tcp");

	if (cl == (CLIENT *)NULL) {
		clnt_pcreateerror(server);
		exit(1);
	}

	output = dothething_1 (&input,cl);

	printf("Texto: %s, Valor: %d\n", output->text, output->key);


	return 0;
}