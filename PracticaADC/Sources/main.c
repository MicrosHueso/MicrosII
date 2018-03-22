#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#define PTA0CH0  0x00
#define PTA1CH1  0x01
#define SENSOR   26
#define BANDGAP  27
#define IRQ      0x60

unsigned int resultado=0x0000;


void initADC (unsigned char canal);
unsigned int medicion(unsigned char canal);
void irqADC (void);



void main(void) {
  SOPT1_COPE=0;
	//EnableInterrupts;
  
initADC(PTA0CH0);
  

  for(;;) {
	  resultado=medicion(PTA0CH0);
  } /* loop forever */
  /* please make sure that you never leave main */
}



void initADC (unsigned char canal)
{
unsigned char pin=0x00;

ADCSC2=0x00;  // Trigger Por Software 
ADCCFG=0x08;  // BusClock 
if(canal== 0x00) pin=0x01;
else pin=canal;
APCTL1=pin;
ADCSC1=0x00|canal; // Canal PTA0 , IRQ=1 
}



unsigned int medicion(unsigned char canal)
{
ADCSC1=0x00|canal; 
while(ADCSC1_COCO==0); // Espera para hacer Conversion
(void)(ADCSC1);  // LECTURA DUMMY
return ADCR;
}


interrupt 19 void irqADC (void)
{
resultado=ADCR;	
ADCSC1=0x60;  
}
