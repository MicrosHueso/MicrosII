#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#define RS PTAD_PTAD1
#define E  PTAD_PTAD0
#define BUS PTBD
#define CUATRO
#define  Mtim4ms  128

unsigned char LCD8[7]=
{0x38,0x38,0x38,0x0F,0x06,0x01,0x00};

unsigned char LCD4[7]=
{0x33,0x32,0x28,0x0F,0x06,0x01,0x00};

void MCU_init(void);
void dato(unsigned char x);
void instruccion(unsigned char x);
void instruccion48(unsigned char dato,unsigned char tipo);
void instruccion4(unsigned char dato);

void inicializa(unsigned char *tabla);
void mensaje(unsigned char *cadena);
void enable(void);
void retardoMTIM(unsigned char modulo);

void main(void) {
unsigned char nombre[8]={"GRUPO7K"};
	SOPT1=0x12;

	//inicializa(&LCD8[0]);
	inicializa(&LCD4[0]);
	mensaje(&nombre[0]);  

  for(;;) {
    __RESET_WATCHDOG();	/* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}


void MCUinit(void)
{//Configurar los PINES del Micro como salida
PTBDD=0xFF;
PTADD_PTADD0=1;
PTADD_PTADD1=1;
//Inicializar los PINES en un Estado
BUS=0xFF;
RS=0;
E=0;
}

void dato(unsigned char x)
{
RS=1;
BUS=x;
enable();
}

void instruccion(unsigned char x)
{
RS=0;
BUS=x;
enable();
}

void instruccion48(unsigned char dato,unsigned char tipo)
{
unsigned char x=0x00;
x=dato;
RS=0;
if(tipo==0x04)
	{
	dato&=0xF0;
	BUS=dato;
	enable();
	x<<=4;
	BUS=x;
	enable();
	}		
else
	{
	BUS=dato;
	enable();
	}
}

void instruccion4(unsigned char dato)
{
unsigned char x=0x00;
	x=dato;
	RS=0;
	dato&=0xF0;
	BUS=dato;
	enable();
	x<<=4;
	BUS=x;
	enable();
}



void inicializa(unsigned char *tabla)
{
	while(*tabla!=0x00)
	{
	instruccion(*tabla);
#ifdef CUATRO	
	instruccion4(*tabla)
#endif	
	instruccion48(*tabla,0x04);
	tabla++;	
	}
}
void mensaje(unsigned char *cadena)
{
	while(*cadena!=0x00)
	{
	dato(*cadena);	
	cadena++;	
	}
	
}

void enable(void)
{
E=0;
retardoMTIM(Mtim4ms);
E=1;
}


void retardoMTIM(unsigned char modulo)
{
MTIMMOD=modulo;
MTIMCLK=0x0F;  // Preescaler =256 /Busclk
MTIMSC_TRST=1;
MTIMSC_TSTP=0;  // Arranca MTIM
while(MTIMSC_TOF==0);  // Espera el retardo
MTIMSC_TSTP=1;
}
