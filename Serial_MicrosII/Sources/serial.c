/*
 * serial.c
 *
 *  Created on: May 25, 2016
 *      Author: B05500
 */
#include <serie.h>

unsigned char buffer[sizebuffer];  // Arreglo para recibir datos del SCI.

void SerialConfig(void)
{
		SCIBDH = 0x00;  //  BaudRate= Busclk/(16*BR)
		SCIBDL = 0x1A;  // Configurar el Baud Rate = 9600	
		SCIC1 = 0x00;   // 8bits datos ; sin Paridad; Sin Loop
		SCIC2 = 0x2C;   // Receptor por IRQ, TX por Polling, Tx y RX Enable
		SCIC3 = 0x00;   // Sin Noveno Bit ; ni chequeo de Errores
		(void)SCIS1;  // Lectura Dummy.
		SCID=0x30;  // Para apagar banderas

}

void transmitebyte(unsigned char x){
SCIC2_TE = 1;	// Habilita el modulo de TX
SCID=x;   // Buffer de datos de TX/RX
while(!SCIS1_TC);	// Checar bandera de TX hasta que se manda SCID
SCIC2_TE = 0;	// para limpiar la bandera TC
}


interrupt 15 void recibe(void)
{
static unsigned char i=0;
if(i==sizebuffer)i=0;
buffer[i]=SCID;
(void)SCIS1;  // Lectura Dummy.
SCID=0; // Borra la bandera la de RX 
i++;
}


