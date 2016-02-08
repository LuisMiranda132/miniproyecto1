#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct box
{
    char msj[10];
    struct box *next;
};

typedef struct box mensaje;

extern mensaje* colaInicio;
extern mensaje* colaFinal;

extern void colaAgregar(char*);
extern char* colaTope();
extern void colaEliminar();

#endif 
