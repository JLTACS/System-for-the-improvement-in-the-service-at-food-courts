/*
 * ComidaProy.c
 *
 * Created: 14/03/2016 11:58:07 p. m.
 *  Author: joseluis
 */ 

#include "ComidaProyecto.h"

unsigned char Coin_Cycle(void);

unsigned char u8aInstruccion[1];
unsigned char u8aLCD_Mensaje[16];
unsigned char  u8Opcion;
unsigned char write[MAX_LEN];


unsigned char u8Flag_Operacion = 0;
unsigned char i;
unsigned int u16Saldo, u16Coin = 0;

/* Activar motor */
/*ISR(INT0_vect){
	PORTD |= (1<<PIND4);
}

ISR(INT1_vect){                   
    PORTD &= ~(1<<PIND4);
	u16Coin += 100;
	LCD_JUMP();
	snprintf((char*)&u8aLCD_Mensaje[0],MAX_LEN,"    %d        ",u16Coin);
	LCD_MESSG(u8aLCD_Mensaje,18);
	
}*/

int main(void){
	
/*Configuracion de puertos*/
    MCUCSR|= (1<<JTD);
    MCUCSR|= (1<<JTD);
    DDRA = 0xFF;
	DDRB &= ~(1<<PINB0); 
	DDRB &= ~(1<<PINB1); 
	DDRC = 0x00;
	DDRD &= ~(1<<PIND7);
	DDRD |= (1<<PIND4);
	PORTD |= (1<<PIND7);
	
	MCUCR = (1<<ISC00)|(1<<ISC01)|(1<<ISC11);
	
	EN_LOW;
	
	ptrfuncLCD=LCD_Idle;
	
	spi_init();
	MFRC522_Init();
	LCD_INIT();

	
	for (;;)
	{
		
		/*Esperar tarjeta*/
	   Detect_Card();
	    
		LCD_Clean();
		
		/*Seleccionar accion a realizar*/
		snprintf((char*)&u8aLCD_Mensaje[0],17,"Revisar Saldo  A");
		LCD_MESSG(u8aLCD_Mensaje,32);  
	
		LCD_JUMP();
		
		snprintf((char*)&u8aLCD_Mensaje[0],17,"Depositar      B");
		LCD_MESSG(u8aLCD_Mensaje,32);
		
		u8Flag_Operacion = 0;
		
		
		for(;u8Flag_Operacion == 0;){	
			
		u8Opcion = BUTTONS;		
		for (; u8Opcion > 2 && u8Opcion <= 0;){
		u8Opcion = BUTTONS;	
		}
		
	
	/* Leer Saldo de la tarjeta*/	
		if(u8Opcion == 1){
			u8Flag_Operacion = 1;
			LCD_Clean();
			
			Detect_Card();  //Detectar y leer tarjeta
			Read_Card(4);
		
			LCD_MESSG(read,30); //Mostrar Saldo en pantalla
			
			
			u8aInstruccion[0] = 0xCC;
			LCD_INST(u8aInstruccion);
			
			snprintf((char*)&u8aLCD_Mensaje[0],16," Fin");
			LCD_MESSG(u8aLCD_Mensaje,32);
			
			while(BUTTONS == 3);
			
			//Esperar Retirado de Tarjeta
			LCD_Clean();
			
			snprintf((char*)&u8aLCD_Mensaje[0],17,"Retire   Tarjeta");
			LCD_MESSG(u8aLCD_Mensaje,32);
			
		/*Depositar Saldo a Tarjeta*/	
		}else if(u8Opcion == 2){
			u8Flag_Operacion = 1;
			u16Coin = 0;
			u16Saldo = 0;
			LCD_Clean();
			
			Detect_Card();   //Detectar y leer tarjeta
			Read_Card(4);
			LCD_MESSG(read,30);
			sscanf((char*)read," %d",&u16Saldo);  //Obtener Saldo de tarjeta
			u8aInstruccion[0] = 0xC4;
			LCD_INST(u8aInstruccion);
			snprintf((char*)&u8aLCD_Mensaje[0],16,"0        Fin");  //Valor inicial de deposito
			LCD_MESSG(u8aLCD_Mensaje,32);
			
			//sei();  //Activar interrupciones
			
			/* Deteccion de Monedas*/
			while(Coin_Cycle()){  
			
			if(gfbillete){
				u16Coin += 100;
				gfbillete = 0;
			}else{
			/*Leer Hall hasta detectar conduccion de la moneda*/
			for(u8index = 0; METAL == 0x80; u8index++){
				u8aCoin_Hall[u8index]= HALL;
				while((u8aCoin_Hall[u8index] == HALL) && (METAL == 0x80));
			}

			/*ordenar valores del Hall*/
			for(u8index = 0; u8index < 8; u8index++){
				for(u8jindex = 0; u8jindex < 8; u8jindex++){
					if (u8aCoin_Hall[u8index] <= u8aCoin_Hall[u8jindex]){
						u8Temp= u8aCoin_Hall[u8index];
						u8aCoin_Hall[u8index] = u8aCoin_Hall[u8jindex];
						u8aCoin_Hall[u8jindex] = u8Temp;
					}
				}
			}                                                       //Fin Hall
			
			
			while(OPTICAL == 0);  //Esperar deteccion del sensor optico
			u8Coin_Optical = (OPTICAL >> 3);
			//switch (u8aCoin_Hall[7]){              //Comparar valor del Hall
				//case 0x00:
				if(u8Coin_Optical == 0x01){        //Comparar para moneda de 10
					u16Coin += 10;
				}
				//break;
				//case 0x01:                         //Comparar para moneda de 1
				if(u8Coin_Optical == 0x08){
					u16Coin += 1;
				}
				//break;
				//case 0x03:                        //Comparar para moneda de 2
				if(u8Coin_Optical == 0x04){
					u16Coin += 2;
				}
				//break;
				//case 0x07:                        //Comparar para moneda de 5
				if(u8Coin_Optical == 0x02){
					u16Coin += 5;
				}
				//break;
				//default:
				//break;
			//}
			for(u8index = 0; u8index<8;u8index++){   //Limpiar valores del arreglo Hall
				u8aCoin_Hall[u8index] = 0;
			}
			}
			
			LCD_JUMP();
			snprintf((char*)&u8aLCD_Mensaje[0],MAX_LEN,"    %d              ",u16Coin);     //Mostrar monto depositado
			LCD_MESSG(u8aLCD_Mensaje,18);
			
			}
			
			//cli();   //Desactivar interrupciones
			
			LCD_Clean();
			
			/*Mostrar Saldo*/ 
			snprintf((char*)&u8aLCD_Mensaje[0],16,"Saldo:");
			LCD_MESSG(u8aLCD_Mensaje,12);
			
			u8aInstruccion[0] = 0x86;
			LCD_INST(u8aInstruccion);
			
			Read_Card(4);
			sscanf((char*)read," %d",&u16Saldo);
			snprintf((char*)&write[0],16,"    %d             ",u16Coin + u16Saldo);  //Mostrar saldo mas saldo depositado
			LCD_MESSG(write,32);
			
			while(BUTTONS == 3);
			
			Detect_Card();
			Write_Card();  //Escribir saldo a la tarjeta
		
		     u8aInstruccion[0] = 0x01;
		     LCD_INST(u8aInstruccion);
		     
		     snprintf((char*)&u8aLCD_Mensaje[0],17,"Retire   Tarjeta");
		     LCD_MESSG(u8aLCD_Mensaje,32);
			 
		}
		}
	
	}
		}
		
/* Condicion para el ciclo de deposito de saldo */		
unsigned char Coin_Cycle(void){
Fallo:	while(BUTTONS == 3 && !HERR && !CIEN);  //Esperar sensor de Herradura o Boton de Salida
		 if(HERR){    //Sensor de herradura: seguir en ciclo
		gfbillete = 0;
		return 1;
	}else if(CIEN){
		gfbillete = 1;
		while(CIEN);
		return 1;
	}else if(BUTTONS == 2){
		gfbillete = 0;
	return 0;
	}
	  goto Fallo;                         //Boton: Salir del ciclo
}

