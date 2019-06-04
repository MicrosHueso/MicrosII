#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */


#define MASTER

#define IIC_ERROR_STATUS 0
#define IIC_READY_STATUS 1
#define IIC_HEADER_SENT_STATUS 2
#define IIC_DATA_TRANSMISION_STATUS 3
#define IIC_DATA_SENT_STATUS 4

void configureI2C(unsigned char);
void WriteBytesI2C (unsigned char,unsigned char);
void ReadBytesI2C (unsigned char,unsigned char);
void MCU_init(void);

//----I2C Variables -------------------------------------------------------
unsigned char I2C_STEP= IIC_READY_STATUS;
unsigned char I2C_DATA_DIRECTION = 0;      /* 1 Transmit, 0 Read */
unsigned char I2C_LENGTH = 1;
unsigned char I2C_COUNTER = 0;

unsigned char I2C_DATA[128];			/* IIC Buffer */

//-------------------------------------------------------------------------


/* Main procedure. */
void main(void) {	
MCU_init();
 
  
  /* Configure interfaces. Set our IIC adress. */
#ifdef MASTER  
  configureI2C(0x50);
  I2C_DATA[0]='A';	/* test data */
#else
  configureI2C(0x52);
#endif 
     
  EnableInterrupts; /* enable interrupts */  
   					 
#ifdef MASTER  
  ReadBytesI2C(0x52,100);

  
  WriteBytesI2C(0x52,100);

   while(I2C_STEP>IIC_READY_STATUS)__RESET_WATCHDOG(); /* wait for memory to be read */

#endif 
    
  /* Application is based on interrupts so just stay here forever. */
  for(;;) {    
       
      __RESET_WATCHDOG(); /* feeds the dog */
  }                       /* loop forever */
  /* please make sure that you never leave this function */
}

/* Function to configure the IIC module. */
void configureI2C(unsigned char selfAddress){
   IICC_IICEN = 1;           /* Enable IIC            */
    IICA = selfAddress;      /* IIC Address           */
      IICF = 0x4B;           /* Set IIC frequency     */    
    I2C_STEP = IIC_READY_STATUS;
    IICC_IICIE = 1;      /* Enable IIC interrupts */
    
}


void WriteBytesI2C (unsigned char slaveAddress,unsigned char numberOfBytes){
	unsigned char Temp;
  
  I2C_LENGTH = numberOfBytes;
  I2C_COUNTER =0;
  I2C_STEP = IIC_HEADER_SENT_STATUS;
  I2C_DATA_DIRECTION = 1;
  
  /* Format the Address to fit in the IICA register and place a 0 on the R/W bit.*/
  slaveAddress &= 0xFE;

  IICC_IICEN = 0;
  IICC_IICEN = 1;
  Temp = IICS;                  /* Clear any pending interrupt */
  IICS_IICIF = 1;

  IICC_MST = 0;
  
  IICS_SRW = 0;
  IICC_TX = 1;				          /* Select Transmit Mode */
	IICC_MST = 1;                 /* Select Master Mode (Send Start Bit) */
    
  for(Temp=0;Temp<3;Temp++);    /* Small delay */
  
  IICD = slaveAddress;						/* Send selected slave address */
     
  return;
}


void ReadBytesI2C (unsigned char slaveAddress,unsigned char numberOfBytes){

	unsigned char Temp;
  
  I2C_LENGTH = numberOfBytes;

  I2C_COUNTER =0;
  I2C_STEP = IIC_HEADER_SENT_STATUS;
  I2C_DATA_DIRECTION = 0;
  
  /* Format the Address to fit in the IICA register and place a 1 on the R/W bit. */
 
  slaveAddress &= 0xFE;
  slaveAddress |= 0x01;         /* Set the Read from slave bit. */

  Temp = IICS;                  /* Clear any pending interrupt  */
  IICS_IICIF = 1;
  
  IICC_TX = 1;				          /* Select Transmit Mode          */
	IICC_MST = 1;				        /* Select Master Mode (Send Start Bit)*/
  
  IICD = slaveAddress;						/* Send selected slave address   */
  
  return;
}


//-------------------------------------------------------------





/* Interrupt handler routine to manage all the events related
 * to the IIC module.    */

interrupt 17 void Viic_isr(void){

	unsigned char Temp;

	Temp = IICS;              /* ACK the interrupt */
  IICS_IICIF = 1;
 
  if(IICS_ARBL==1){         /* Verify the Arbitration lost status */	     
	     IICS_ARBL= 1;
	     IICC_MST = 0;	
	     I2C_STEP = IIC_ERROR_STATUS;
	        

	          
	     return;     
	}										       /* If Arbitration is OK continue */  
		

	
	if(IICC_MST==1){          /* If we are the IIC Master */
	 
	 				 	 
	 	 if(IICS_RXAK==1){      /* Verify if byte sent was ACK */
	 	    IICC_MST = 0;
	      I2C_STEP = IIC_ERROR_STATUS;
	   
	      return;
	 	 }
	 			             
     if(I2C_STEP == IIC_HEADER_SENT_STATUS){ /* Header Sent */
          IICC_TX = I2C_DATA_DIRECTION;
          
          I2C_STEP = IIC_DATA_TRANSMISION_STATUS; 
          
          if(IICC_TX==0){  /* If we are reading data clock in first slave byte */
              Temp = IICD;
              return;
          }
     }
              
     if(I2C_STEP == IIC_DATA_TRANSMISION_STATUS){	 /* If byte transmision is in progress.*/
           
        	  if(IICC_TX==1){				               /* If Master is sending data to slave */     	 
        	            	    
        	    IICD = I2C_DATA[I2C_COUNTER];	     /* Send the next byte */
        	         	 
        	    I2C_COUNTER++;
        	    if(I2C_LENGTH <= I2C_COUNTER){        	     
        	      I2C_STEP=IIC_DATA_SENT_STATUS;     /* Mark we are done sending Bytes */   	  
        	    }
        	    return;        	 								     /* wait until last byte sent      */
        	  } 
        	  else{										               /* If master is reading data from slave */
        	      
        	      if((I2C_COUNTER+1) == I2C_LENGTH)  /* Master should not ACK the last byte */
        	          IICC_TXAK = 1;							   /* to indicate end of transfer         */
        	      
        	      I2C_DATA[I2C_COUNTER] = IICD;	     /* Read the next byte */       	         	 
        	      I2C_COUNTER++;
        	      																 
        	      if(I2C_LENGTH <= I2C_COUNTER){        	     
        	        I2C_STEP=IIC_DATA_SENT_STATUS;   /* Mark we are done sending Bytes */   	  
        	      }   
        	      
        	      return;           	    					 /* Return until next byte is read */
        	 }	  
       }
                   
       if(I2C_STEP==IIC_DATA_SENT_STATUS){	       /* We are done with the transmition.*/ 
        	 
        	 I2C_STEP=IIC_READY_STATUS;	             /* Reset our status flag            */
        	 Temp = IICS;                            /* ACK the interrupt                */
           IICS_IICIF=1;
        	 
        	 IICC_TX=0;
        	 IICS_SRW=0;
        	 IICC_MST=0;
        	 			                                   /* Generate a stop condition        */        	  
        	 return;
       }        
	}
  else{			 /*  SLAVE OPERATION  */  
                   																						 

       if(I2C_STEP <= IIC_READY_STATUS){	 				 /* If it is the first byte tranmited */
           I2C_STEP = IIC_DATA_TRANSMISION_STATUS;
           IICC_TX = IICS_SRW;				           /* Set the transmision reception status */
           I2C_COUNTER = 1;
           
           /* If we are receiving data read IIC1D to get free bus and get the next byte */
           if(IICC_TX==0){
              Temp = IICD;
              return;
           }
       }
       	
       
			 if(IICS_TCF==1){
          if(IICC_TX == 0){	        /* If data is received store it on the buffer */              
              I2C_DATA[I2C_COUNTER]=IICD;
              I2C_COUNTER++;
              return;          
          } 
          else{	                      /* Data sent by the slave */              
              
              if(IICS_RXAK==1){      /* If byte is not ACK end transmision. */
	                IICC_TX = 0;
	                Temp = IICD;
	                I2C_STEP = IIC_READY_STATUS;
	                return;
	 	          }
              
              IICD = I2C_DATA[I2C_COUNTER];
              I2C_COUNTER++;
              return;          
          }
		  }
   }
}

void MCU_init(void)
{
  /* ### MC9S08QG8_16 "Cpu" init code ... */
  /*  PE initialization code after reset */
  /*  System clock initialization */
  /* SOPT1: COPE=1,COPT=1,STOPE=0,BKGDPE=1,RSTPE=1 */
  SOPT1 = 0xD3;                                      
  /* SPMSC1: LVDF=0,LVDACK=0,LVDIE=0,LVDRE=1,LVDSE=1,LVDE=1,BGBE=0 */
  SPMSC1 = 0x1C;                                      
  /* SPMSC2: PDF=0,PPDF=0,PPDACK=0,PDC=0,PPDC=0 */
  SPMSC2 = 0x00;                                      
  /* SPMSC3: LVDV=0,LVWV=0 */
  SPMSC3 &= (unsigned char)~0x30;                              
  /* ICSC1: CLKS=0,RDIV=0,IREFS=1,IRCLKEN=0,IREFSTEN=0 */
  ICSC1 = 0x04;                        /* Initialization of the ICS control register 1 */
  /* ICSC2: BDIV=0,RANGE=0,HGO=0,LP=0,EREFS=0,ERCLKEN=0,EREFSTEN=0 */
  ICSC2 = 0x00;                        /* Initialization of the ICS control register 2 */
  /* Common initialization of the write once registers */
  /* SOPT2: COPCLKS=0,IICPS=0,ACIC=0 */
  SOPT2 = 0x00;                                      

  /* Common initialization of the CPU registers */
  /* PTASE: PTASE5=1,PTASE4=1,PTASE3=1,PTASE2=1,PTASE1=1,PTASE0=1 */
  PTASE |= (unsigned char)0x3F;                               
  /* PTBSE: PTBSE7=1,PTBSE6=1,PTBSE5=1,PTBSE4=1,PTBSE3=1,PTBSE2=1,PTBSE1=1,PTBSE0=1 */
  PTBSE = 0xFF;                                      
  /* PTADS: PTADS5=0,PTADS4=0,PTADS3=0,PTADS2=0,PTADS1=0,PTADS0=0 */
  PTADS = 0x00;                                      
  /* PTBDS: PTBDS7=0,PTBDS6=0,PTBDS5=0,PTBDS4=0,PTBDS3=0,PTBDS2=0,PTBDS1=0,PTBDS0=0 */
  PTBDS = 0x00;                                      
  /* ### Init_COP init code */
  SRS = 0xFF;                          /* Clear WatchDog counter */

  /* ### */
  asm CLI;                             /* Enable interrupts */
} /*MCU_init*/
