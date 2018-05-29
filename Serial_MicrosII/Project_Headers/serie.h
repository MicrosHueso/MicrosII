/*
 * serie.h
 *
 *  Created on: May 25, 2016
 *      Author: B05500
 */
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#ifndef SERIE_H_
#define SERIE_H_

#define sizebuffer  10

/* Prototipos */
void SerialConfig(void);
void recibe(void);
void transmitebyte(unsigned char x);

/*Variables */
//unsigned char buffer[sizebuffer];  // Arreglo para recibir datos del SCI.



#endif /* SERIE_H_ */
