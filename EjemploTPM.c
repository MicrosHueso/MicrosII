#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#define  MODULO  0xFFFF  // PERIODO
#define  PRESCALER 32
#define  BCLK  4000000
#define  T_OV   MODULO*PRESCALER*(1/BCLK)

void init_ouputcompare(void);
void periodo (unsigned int tiempo);



void main(void) {
  SOPT1_COPE=0;
  PTBDD_PTBDD7=1;  
  PTADD_PTADD0=1;  // Definir usar CH0 como TPM Output Compare
  init_ouputcompare();
  
  for(;;) {
  periodo(1000);
  PTBD_PTBD7=~PTBD_PTBD7;
  } /* loop forever */
  /* please make sure that you never leave main */
}



void init_ouputcompare(void)
{
	TPMMOD = MODULO; // Maxima cuenta del Contador General del TPM
	TPMC0V = 0x0000;  // Valor de Comparacion para hacer Toggle  	
	TPMC0SC = 0x14; //Modo Output Compare, Toggle
	TPMSC = 0x0D; // Sin IRQ, BusClk=4Mhz, Preescaler=32
}

void periodo (unsigned int tiempo)
{
while(!TPMC0SC_CH0F); // checar si el tiempo ya paso
TPMC0SC_CH0F=0; // Borra la bandera
TPMC0V = TPMC0V+tiempo; // Recarga el timer para el siguiente periodo
}

