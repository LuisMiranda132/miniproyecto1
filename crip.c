#include <crip.h>

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
			
		} else if (79 <= text[i] && text[i] &&  text[i] <= 90) { // O-Z
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

void stganograf(char *text)
{
   int i, size = strlen(text);
   for(i = 0; i < size; i++)
   {
      switch(text[i]){
      case 'A': text[i] = '4';
	 break;
      case 'B': text[i] = '8';
	 break;
      case 'C': text[i] = '(';
	 break;
      case 'E': text[i] = '3';
	 break;
      case 'H': text[i] = '#';
	 break;
      case 'I': text[i] = '1';
	 break;
      case 'O': text[i] = '0';
	 break;
      case 'S': text[i] = '5';
	 break;
      case 'T': text[i] = '7';
	 break;
      case 'Y': text[i] = '&';
	 break;
      case '4': text[i] = 'A';
	 break;
      case '8': text[i] = 'B';
	 break;
      case '(': text[i] = 'C';
	 break;
      case '3': text[i] = 'E';
	 break;
      case '#': text[i] = 'H';
	 break;
      case '1': text[i] = 'I';
	 break;
      case '0': text[i] = 'O';
	 break;
      case '5': text[i] = 'S';
	 break;
      case '7': text[i] = 'T';
	 break;
      case '&': text[i] = 'Y';
	 break;

      }
   }
   
}

int check(char *text)
{
   int sineste = 0, coneste = 0;
   int i, size = strlen(text);
   for(i = 0; i < size; i++)
   {
      if(!sineste&&(text[i] == 'A' || text[i] == 'B' || text[i] == 'C' ||
		    text[i] == 'E' || text[i] == 'H' || text[i] == 'I' || 
		    text[i] == 'O' || text[i] == 'S' || text[i] == 'T' ||
		    text[i] == 'Y')){
	 sineste = 1;
      }else if(
	 !coneste&&(text[i] == '4' || text[i] == '8' || text[i] == '(' ||
		    text[i] == '3' || text[i] == '#' || text[i] == '1' || 
		    text[i] == '0' || text[i] == '5' || text[i] == '7' ||
		    text[i] == '&')){
	 coneste = 1;
      }
      if(coneste && sineste){
	 return -1;
      }
   }
   if(coneste){
      return 1;
   }else if(sineste){
      return 0;
   }
}
