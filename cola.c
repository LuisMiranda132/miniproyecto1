#include <cola.h>

mensaje* colaInicio = NULL;
mensaje* colaFinal = NULL;

void colaAgregar(char* msj)
{
   mensaje* tmp = (mensaje*) malloc(sizeof(mensaje));
   strcpy(tmp->msj,msj);
   tmp->next = NULL;

   if(colaInicio == NULL && colaFinal == NULL){
      colaInicio = colaFinal = tmp;
      return;
   }
   colaFinal->next = tmp;
   colaFinal = tmp;
};

char* colaTope()
{
   if(colaInicio != NULL){
      return colaInicio->msj;
   }
   return NULL;
};

void colaEliminar(void)
{
   mensaje* tmp = colaInicio;
   if(colaInicio != NULL){
      if(colaInicio == colaFinal){
	 colaInicio = colaFinal = NULL;
      }else{
	 colaInicio = colaInicio->next;
      }
      free(colaInicio);
   }
};
