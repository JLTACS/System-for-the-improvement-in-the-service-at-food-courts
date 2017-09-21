/*
 * LCDFunctions.h
 *
 * Created: 15/03/2016 11:01:03 p. m.
 *  Author: joseluis
 */ 


#ifndef LCDFUNCTIONS_H_
#define LCDFUNCTIONS_H_

#include "LCDSM.h"

const unsigned char u8aInitLCD[6]={0x02, 0x28, 0x0C, 0x06, 0x01, 0x80};
unsigned char u8aInstruccion[1];


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
	for(;Get_LCDStatus()==0;){
		LCD_HANDLER();
	}
}

void LCD_INIT (void){
	
	
	RS_LOW;
	
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


#endif /* LCDFUNCTIONS_H_ */