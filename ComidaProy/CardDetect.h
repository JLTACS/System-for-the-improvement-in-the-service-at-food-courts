/*
 * CardDetect.h
 *
 * Created: 26/04/2016 10:36:27 a. m.
 *  Author: joseluis
 */ 


#ifndef CARDDETECT_H_
#define CARDDETECT_H_

#include <avr/io.h>


unsigned char byte;
unsigned char str[MAX_LEN];
unsigned char write[MAX_LEN];
unsigned char u8aLCD_Mensaje[16];
unsigned char u8aRedo[1] = {0x80};

/* Resetar tarjeta */
void Reset_Card(){
	MFRC522_Init();
}

/* Detectar Tarjeta*/
void Detect_Card(void){
	byte = MFRC522_Request(PICC_REQALL,str);
	
	while(byte != MI_OK)
	{
		byte = MFRC522_Request(PICC_REQALL,str);
		LCD_INST(u8aRedo);
		snprintf((char*)&u8aLCD_Mensaje[0],17,"Insertar Tarjeta");
		LCD_MESSG(u8aLCD_Mensaje,32);
	}
	LCD_Clean();
}

/* Leer bloque de saldo de la tarjeta*/
void Read_Card(uchar bloque){
	unsigned char i;
	
	Reset_Card();
	readDataASCII(bloque);

	for(i=0; i<4; i++){
		read[i] = 0x20;
	}
}

/*Escribi al bloque de saldo de la tarjeta */
void Write_Card(void){
	
	Reset_Card();
	writeTagBlockMemory(&write[0]);
}

#endif /* CARDDETECT_H_ */