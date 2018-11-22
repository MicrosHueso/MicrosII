

// Ejemplo de Maquina De Estado Finitos.

#define  Encender 0
#define  Apagar   1

typedef
	struct
	{
	unsigned char salidas;
	unsigned char tiempo;
	unsigned char estado_siguiente[2];
	}estado;

estado Maquina[2]={
{0,0xFF, {Enceder,Apagar}},
{1,0xFF,{Apagar,Encender}}
};

void (*puntero)(void);

void main(void){
unsigned char estado_actual=Encender;
unsigned char entrada=1;

while(1)
	{
	PTBD_PTBD7=Maquina[Estado_actual].salidas;				// Prende LEDS
	retardo(Maquina[Estado_actual].tiempo);  				// Pone el tiempo de cada Estado
	Entrada=PTAD_PTAD2;                     				// Leer Entrada para cambiar de Estado
	Estado_actual=Maquina[Estado_actual].Estado_siguiente[Entrada]; // Cambio de Estado;
	}
}

