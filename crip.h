#ifndef CRIPT_H_
#define CRIPT_H_

/**
 * @author Luis Miranda 10-10463
 * @author Oskar Gonzales 09-10351
 *
 * Funciones de ayuda para SCDAX
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Funcion se encarga de encriptar
 */
extern char* encrypt(char *, int);

/**
 * Funcion se encarga de desencriptar
 */
extern char* decrypt(char *, int);

/**
 * Funcion se encarga de esteganografia
 */
extern void stganograf(char *);

/**
 * Funcion se encarga de revisar la validez del texto
 */
extern int check(char *);

#endif 
