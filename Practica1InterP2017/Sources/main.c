#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

/* Prototipos de las funciones para la pantalla LCD*/
void configura(void);
void retardoRTI(unsigned char tiempo);
void enable(void);
void instruccion(unsigned char cmd);
void datos(unsigned char x);
void inicializa(void);
void cadena(unsigned char *arreglo);
unsigned char ascii (unsigned char x);


unsigned char mensaje[13]={"La tecla es:"};


void main(void) {
  configura();
  inicializa();	
  cadena(&mensaje[0]);
 
  
  for(;;) {
	  datos(ascii(PTBD&0x0F));
	  retardoRTI(0x07);
	  instruccion(0x8C);
	  __RESET_WATCHDOG();	/* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}


void configura(void)
{
SOPT1=0x12;	
PTADD=0x03;    // RS y E
PTBDD=0xF0;     // Puertos de la Pantalla a 4 bits
SRTISC=0x00;   // IRQ NO,  BANDERA=0; RTI=NO
PTAD_PTAD1=0;
PTAD_PTAD0=0;
}

void retardoRTI(unsigned char tiempo)
{
SRTISC=tiempo;   //	Aqui se configura el TIEMPO del RTI
while(SRTISC_RTIF==0); // Espera a que pase el TIEMPO seleccionado
SRTISC_RTIACK=1; // Apagar Bandera
SRTISC=0x00; // Apagar al RTI
}


void enable(void)
{
PTAD_PTAD1=1;  // Enable en 1
retardoRTI(0x01);  // Retardo de 8ms
PTAD_PTAD1=0;    // Enable en 0
}

void instruccion(unsigned char cmd)
{
unsigned char MSB;
unsigned char LSB;

PTAD_PTAD0=0;   //RS=0 para instrucciones

MSB=cmd & 0xF0;   // Mascara para hacer AND
LSB=cmd & 0x0F;
LSB=LSB<<4;

PTBD=MSB;
enable();
PTBD=LSB;
enable();
}

void datos(unsigned char x)
{
unsigned char MSB;
unsigned char LSB;

PTAD_PTAD0=1;  // RS=1 para datos
	
MSB=x & 0xF0;   // Mascara para hacer AND
LSB=x & 0x0F;
LSB=LSB<<4;

PTBD=MSB;
enable();
PTBD=LSB;
enable();
}


void inicializa(void)
{
unsigned char comandos[7]={0x33,0x32,0x28,0x0F,0x06,0x01,0x00};	
unsigned char *puntero;

puntero=&comandos[0];  //Direccion de Comandos
	while(*puntero!=0x00)
	{
	instruccion(*puntero);	
	puntero++;	
	}
}


void cadena(unsigned char *arreglo)
{
	//arreglo=&mensaje[0];
	while(*arreglo!=0x00)
	{
	datos(*arreglo);	
	arreglo++;	
	}
}


unsigned char ascii (unsigned char x)
{
if (x<=0x09) x+=0x30;  // x=x+0x30;

else  x+=0x37;

return x;
}
