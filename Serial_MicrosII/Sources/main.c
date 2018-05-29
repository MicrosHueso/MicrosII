#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include <serie.h>



void main(void) {
	SOPT1=0x12;
	ICSTRM=0x9B;
	SerialConfig();
  
	//transmitebyte(0x41);
	//transmitebyte(0x31);
	//transmitebyte(0x41);	
	EnableInterrupts;
  for(;;) {
	  transmitebyte(0x41);
    __RESET_WATCHDOG();	/* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}


