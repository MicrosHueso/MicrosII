#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

void configura(void);
void Init_KBI(void);
void retardoRTI(unsigned char tiempo);
void checa_tecla(void);
unsigned char codigo_scan(void);

unsigned char detecta=0x00;
unsigned char tecla=0x00;
unsigned char filas=0x00;

void main(void) {
  
configura();
Init_KBI();
  
  for(;;) {
	  PTBD=0x0F;   // Poner columnas a cero para detectar tecla
	  EnableInterrupts;  // para leer filas
	  while(detecta==0x00);  // Espera tecla presionada
	    tecla=codigo_scan();
	    PTAD=tecla;  // Tecla esta en Hexa!
	    retardoRTI(0x03);
	    detecta=0x00;
  } /* loop forever */
  /* please make sure that you never leave main */
}

void configura(void)
{
SOPT1=0x12;  // quitar Watchdog
PTADD=0x0f;  // Puertos de Salida del teclado
PTBDD=0xf0;  // Habilitar filas y columnas del teclado
SRTISC=0x00;  // Habilitar Timer RTI
PTBD=0x0F;   // Poner columnas a cero para detectar tecla
PTAD=0x00;  // Inicializa el PTA
}

void Init_KBI(void)
{
KBISC=0x00;  // Quitar Interrupciones
KBIES=0x00;   // Habilitar el modo de deteccion
PTBPE=0x0F;  // Habilitar Pullups
KBIPE=0xF0;  // Habilitar KBI
KBISC=0x06;  //  Borrar Bandera
KBISC=0x02;  // Habilitar KBIE irq
}

void retardoRTI(unsigned char tiempo)
{
SRTISC=tiempo;   //	Aqui se configura el TIEMPO del RTI
while(SRTISC_RTIF==0); // Espera a que pase el TIEMPO seleccionado
SRTISC_RTIACK=1; // Apagar Bandera
SRTISC=0x00; // Apagar al RTI
}

interrupt 18 void checa_tecla(void)
{
DisableInterrupts;
KBISC_KBACK=1; // Borrar  la bandera KBIF
detecta=0x01;
filas=PTBD&0x0F;  // Filas
EnableInterrupts;
}


unsigned char codigo_scan(void)
{
PTBD=0xE0;
filas=PTBD&0x0f;
if(filas==0x0E) return 0x00;  
if(filas==0x0D) return 0x04;
if(filas==0x0B) return 0x08;
if(filas==0x07) return 0x0C;


PTBD=0xD0;
filas=PTBD&0x0F;
if(filas==0x0E) return 0x01;  
if(filas==0x0D) return 0x05;
if(filas==0x0B) return 0x09;
if(filas==0x07) return 0x0D;

PTBD=0xB0;
filas=PTBD&0x0F;
if(filas==0x0E) return 0x02;  
if(filas==0x0D) return 0x06;
if(filas==0x0B) return 0x0A;
if(filas==0x07) return 0x0E;


PTBD=0x70;
filas=PTBD&0x0F;
if(filas==0x0E) return 0x03;  
if(filas==0x0D) return 0x07;
if(filas==0x0B) return 0x0B;
if(filas==0x07) return 0x0F;

}

