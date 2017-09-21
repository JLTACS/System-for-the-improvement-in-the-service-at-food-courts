/*
 * LCDFunctions.h
 *
 * Created: 15/03/2016 11:01:03 p. m.
 *  Author: joseluis
 */ 


#ifndef LCDFUNCTIONS_H_
#define LCDFUNCTIONS_H_

#include <avr/io.h>
#include "LCDSM.h"


char Keyboard (void);
void LCD_HANDLER(void);
unsigned char Get_LCDStatus(void);
void LCD_Write (unsigned char *u8Char, unsigned char u8Size);
void LCD_INIT (void);
void LCD_DATA (unsigned char u8Data);
void LCD_INST (unsigned char u8Instruction[]);
void LCD_MESSG (unsigned char u8aMessg[],unsigned char Size);
void LCD_Clean(void);
void BCD_2_ASCII (char Data);
unsigned char ASCII (char Data);
char Keyboard (void);


const unsigned char u8aInitLCD[6]={0x02, 0x28, 0x0C, 0x06, 0x01, 0x80};
unsigned char u8aInstruccion[1];
unsigned char u8aMensaje_OK[2]={"OK"};


void LCD_HANDLER(void){
	ptrfuncLCD();
}

unsigned char Get_LCDStatus(void){
	return gFlag;
}

void LCD_Write (unsigned char *u8Char, unsigned char u8Size){
	gFlag = 0;
	gptrMessage = u8Char;
	u8Size_Message = u8Size;
	ptrfuncLCD = LCD_WriteNibble;
	for (;Get_LCDStatus()==0;)
	{
		LCD_HANDLER();
	}
}

void LCD_INIT (void){
	
	RS_LOW;
	DDRB |= ((1<<PINB0) | (1<<PINB1));
	DDRC = (1<<PINC0) | (1<<PINC1) | (1<<PINC2) | (1<<PINC3);
	
	LCD_Write((unsigned char*)&u8aInitLCD[0],(sizeof(u8aInitLCD) << 1));
}

void LCD_DATA (unsigned char u8Data){
	
	RS_HIGH;
	
	LCD_Write(&u8Data,(sizeof(u8Data) << 1));
}

void LCD_INST (unsigned char u8Instruction[]){
	
	RS_LOW;
	
	LCD_Write(&u8Instruction[0],0x02);
}

void LCD_MESSG (unsigned char u8aMessg[],unsigned char Size){
	
	RS_HIGH;
	
	LCD_Write(&u8aMessg[0],Size);
}

void LCD_Clean(void){
	u8aInstruccion[0] = 0x01;
	LCD_INST(u8aInstruccion);
}


void LCD_JUMP(void){
	u8aInstruccion[0] = 0xC0;
	LCD_INST(u8aInstruccion);
}

void LCD_OK(void){
 u8aInstruccion[0] = 0xCE;
 LCD_INST(u8aInstruccion);
 LCD_MESSG(u8aMensaje_OK,4);
}

void BCD_2_ASCII (char Data){
	
	char x,y;
	x=(((Data&0xF0)>>4)|0x30);
	LCD_DATA(x);
	y=((Data&0x0F)|0x30);
	LCD_DATA(y);
	
}

unsigned char ASCII (char Data){
	if (Data<0x0A) {
		Data += 0x30;
	}
	else {
		Data += 0x37;
	}
	LCD_DATA(Data);
	return Data;
}

char Keyboard (void){

		char x,y;

		DDRD = 0x0F;
		PORTD = 0xF0;
		while (PIND == 0xF0);
		x=PIND;
		DDRD = 0xF0;
		PORTD = 0x0F;
		while (PIND == 0x0F);
		y=PIND;
		x |= y;

		switch (x){

			case 0xD7: y=0x00; break;
			case 0xEE: y=0x01; break;
			case 0xDE: y=0x02; break;
			case 0xBE: y=0x03; break;
			case 0xED: y=0x04; break;
			case 0xDD: y=0x05; break;
			case 0xBD: y=0x06; break;
			case 0xEB: y=0x07; break;
			case 0xDB: y=0x08; break;
			case 0xBB: y=0x09; break;
			case 0x7E: y=0x0A; break;
			case 0x7D: y=0x0B; break;
			case 0x7B: y=0x0C; break;
			case 0x77: y=0x0D; break;
			case 0xB7: y=0x0E; break;
			case 0xE7: y=0x0F; break;


		}

		DDRD = 0x0F;
		PORTD = 0xF0;
		while (PIND != 0xF0){
			PORTD = 0xF0;
		}

		return y;
	}





#endif /* LCDFUNCTIONS_H_ */