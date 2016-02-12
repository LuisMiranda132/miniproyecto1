/*scdaxservidor.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include "scdax.h"

/* Recibe el texto y clave, realiza comprobacion de esteganografia
   y cifra o descifra segun corresponda */
struct to_encrypt *
dothething_1_svc(struct to_encrypt *txtnkey, struct svc_req *rqstp) {
	static to_encrypt result;
	int cifrado = 0;
	int i, l = strlen(txtnkey->text);
	int ncount = 0;
	int ncount2 = 0;
	char *code = (char*)malloc(l*sizeof(char));

	printf("TEXTO: %s, CLAVE: %d \n",txtnkey->text,txtnkey->key);

	if (code == NULL) {
		result.key = 28;
		return &result;
	}

	/* Aqui van las comprobaciones de esteganografia*/

	/* Si ya esta cifrado, descifrar */
	if (cifrado) {
		txtnkey->key = -txtnkey->key;
	}

	txtnkey->key = txtnkey->key%27;
	if (txtnkey->key == 0) {
		result.key = 0;
		return &result;
	}
	if (txtnkey->key < 0)
		txtnkey->key = txtnkey->key+27;

	for (i = 0; i < l; i++) {
		if (65 <= txtnkey->text[i] && txtnkey->text[i] <= 78) { // A-N
			code[i-ncount] = txtnkey->text[i] - 65;
			
		} else if (79 <= txtnkey->text[i] && txtnkey->text[i] <= 90) { // O-Z
			code[i-ncount] = txtnkey->text[i] - 64;

		} else if (txtnkey->text[i] == -61) { // Ñ (consume dos valores del arreglo)
			code[i-ncount] = 14;
			ncount++;
			i++;
		} else {
			code[i-ncount] = txtnkey->text[i];
		}
	}

	for (i = 0; i < l - ncount; i++) {
		if (code[i] < 27) {
			code[i] = (code[i] + txtnkey->key) % 27;
			if (code[i] == 14) ncount2++;
	    }
	}

	char *data = (char*)malloc((l-ncount+ncount2)*sizeof(char));
	if (data == NULL) {
		result.key = 28;
		return &result;
	}
	int ncount3 = 0;

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
	result.key = 0;
	result.text = data;
	return &result;
}