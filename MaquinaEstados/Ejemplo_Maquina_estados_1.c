

enum state { NUEVO, ABIERTO, CERRADO, FIN };

enum state current_state = NUEVO;



void main(void) 
{
 void (*fp)();  // Aqui se define un puntero a funcion
fp = PRENDE1;  // Inicializar un puntera a una funcion Estado1  
while (current_state != FIN)
{
   switch (current_state)
    {
        case NUEVO:
        /*  Aqui va el codigo del primer estado		*/
		(*fp)();    // Llamada Referenciada a la funcion 
		fp=APAGA2; // Inicializar el puntero a la siguiente funcion
		current_state=ABIERTO;
        break;

        case ABIERTO:
        /* Aqui va el codigo para el estado 2 */
		if(boton==1)
		(*fp)();    // Llamada Referenciada a la funcion 
		fp=PRENDE1;
		current_state=CERRADO;
        break;
		
		case CERRADO:
		(*fp)();
		current_state=NUEVO;
		break;
        /* ... etc ... */
    }
}

}


void PRENDE1() {
  LED1=1;
  LED2=0;
  LED3=0;
}

void APAGA2() {
  LED1=0;
  LED2=1;
  LED3=0;
}
