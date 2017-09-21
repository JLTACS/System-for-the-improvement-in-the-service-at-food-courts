/*
 * YEP.c
 *
 * Created: 14/05/2016 12:27:55 p. m.
 *  Author: joseluis
 */ 
#include "YEP.h"

/* Conversion de ASCII a Decimal */
unsigned int ASCIITo_Decimal(unsigned char *ascii,unsigned char size){
	unsigned int decimal = 1, convertion = 0;
	unsigned char u8index;
	
	ascii += size;
	
	for(u8index = size; u8index <= size; u8index--){
		if (*ascii < 0x30 || *ascii > 0x39){
			convertion += 0;
			ascii--;
		}else {
			convertion += ((*ascii-0x30)*decimal);
			decimal *= 10;
			ascii--;
		}
	}
	return convertion;
}

/*Conversion de ASCII a Octal*/
unsigned int long ASCIITo_Octal(unsigned char *ascii,unsigned char size){
	unsigned int long octal = 1; 
	unsigned int long convertion = 0;
	unsigned char u8index;
	
	ascii += size;
	
	for(u8index = size; u8index <= size; u8index--){
		if (*ascii < 0x30 || *ascii > 0x37){
			convertion += 0;
			ascii--;
			}else {
			convertion += ((*ascii-0x30)*octal);
			octal *= 8;
			ascii--;
		}
	}
	return convertion;
}

/* Descontar pago de la tarjeta*/
unsigned char Descontar_Card (unsigned int u16Pago){
	unsigned int u16Saldo;
	unsigned char write[16];
Detect_Card();
Read_Card(4);
	u16Saldo = ASCIITo_Decimal(&read[0],15);
	if(u16Saldo < u16Pago){
		
		return 0;	
	}else{
	u16Saldo = u16Saldo - u16Pago;
	
	snprintf((char*)&write[0],16,"    %d              ",u16Saldo);
	Reset_Card();
	Detect_Card();
	writeTagBlockMemory(&write[0]);
	return 1;
}
	}
	
/* Generar codigo YEP*/
void Generate_Yep(unsigned int long u16Pago,unsigned char u8Restaurant){
	unsigned int long YEP = 0;
	unsigned char RTC[7], Hour;
	Get_RTC(RTC);
	switch(u8Restaurant){
		case 1: YEP = u16Pago*12905; break;
		case 2: YEP = u16Pago*13981; break;	
		case 3: YEP = u16Pago*11650; break;
	}
	Hour = ((RTC[2]>>4)*10);
	Hour = (Hour + (RTC[2]&0x0F));
	
	snprintf((char*)&u8aLCD_Mensaje[0],17,"YEP: 1%lX%d%.2X       ",YEP,RTC[3],Hour*10);	
	LCD_MESSG(u8aLCD_Mensaje,32);
}

void AddTo_Card (unsigned int u16Reembolso){
	unsigned int u16Saldo;
	unsigned char write[16];
	Detect_Card();
	Read_Card(4);
	u16Saldo = ASCIITo_Decimal(&read[0],15);
	
		u16Saldo = u16Saldo + u16Reembolso;
		
		snprintf((char*)&write[0],16,"    %d              ",u16Saldo);
		Reset_Card();
		Detect_Card();
		writeTagBlockMemory(&write[0]);
		
		Detect_Card();
		Read_Card(4);
		u16Saldo = ASCIITo_Decimal(&read[0],15);

		snprintf((char*)&u8aLCD_Mensaje[0],17,"Saldo: %d            ",u16Saldo);
		LCD_MESSG(u8aLCD_Mensaje,32);
}

/* Decodificar SUP y Añadir saldo a la tarjeta*/
void  Decode_SUP(unsigned int long u16SUP,unsigned char u8Restaurant){
	unsigned int long SUP = 0;
	unsigned int  u16Saldo;
	unsigned char write[16];
	
		switch(u8Restaurant){
			case 1: SUP = u16SUP/12905; break;
			case 2: SUP = u16SUP/13981; break;
			case 3: SUP = u16SUP/11650; break;
		}		
		
	Detect_Card();
	Read_Card(4);
	u16Saldo = ASCIITo_Decimal(&read[0],15);
	
	u16Saldo = u16Saldo + SUP;
	
	snprintf((char*)&write[0],16,"    %d              ",u16Saldo);
	Reset_Card();
	Detect_Card();
	writeTagBlockMemory(&write[0]);
	
	Detect_Card();
	Read_Card(4);
	u16Saldo = ASCIITo_Decimal(&read[0],15);

	snprintf((char*)&u8aLCD_Mensaje[0],17,"Saldo: %d            ",u16Saldo);
	LCD_MESSG(u8aLCD_Mensaje,32);
}