/*
 * LCDSM.h
 *
 * Created: 15/03/2016 12:27:23 a. m.
 *  Author: joseluis
 */ 


#ifndef LCDSM_H_
#define LCDSM_H_

#define RS_HIGH PORTA |= (1<<PINA4)
#define RS_LOW PORTA &= ~(1<<PINA4)
#define EN_HIGH PORTA |= (1<<PINA5)
#define EN_LOW PORTA &= ~(1<<PINA5)
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
	
	PORTA = (PINA & ~(0xF)) | (u8Temp & 0xF);
	ptrfuncLCD = LCD_Wait;	
	EN_HIGH;
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