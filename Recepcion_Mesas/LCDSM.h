/*
 * LCDSM.h
 *
 * Created: 15/03/2016 12:27:23 a. m.
 *  Author: joseluis
 */ 



#ifndef LCDSM_H_
#define LCDSM_H_

#include <avr/io.h>

#define RS_HIGH PORTB |= (1<<PINB1)
#define RS_LOW PORTB &= ~(1<<PINB1)
#define EN_HIGH PORTB |= (1<<PINB0)
#define EN_LOW PORTB &= ~(1<<PINB0)
#define LOWERNIBBLE 0x0F
#define TIME (50)



void(*ptrfuncLCD)(void);
void LCD_Idle (void);
void LCD_WriteNibble (void);
void LCD_Wait (void);

unsigned int u16Time = TIME;
unsigned char *gptrMessage;
unsigned char u8Size_Message;
unsigned char gFlag;
unsigned char u8Temp;

void LCD_Idle (void)
{
	gFlag = 1;
}

void LCD_WriteNibble (void){


	if (u8Size_Message & 0x1){
		u8Temp = (*gptrMessage++) & LOWERNIBBLE;
		}else{
		u8Temp = (*gptrMessage >> 4) & LOWERNIBBLE;
	}
	EN_HIGH;
	PORTC = (PORTC & ~(0xF)) | (u8Temp & 0xF);
	ptrfuncLCD = LCD_Wait;
}

void LCD_Wait (void){


	if (!u16Time--){
		EN_LOW;
		u16Time = TIME;
		ptrfuncLCD = LCD_WriteNibble;
		if (!(--u8Size_Message)){
			ptrfuncLCD = LCD_Idle;
		}
	}
}


#endif /* LCDSM_H_ */