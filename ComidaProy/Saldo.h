/*
 * Saldo.h
 *
 * Created: 24/05/2016 02:48:44 p. m.
 *  Author: joseluis
 */ 


#ifndef SALDO_H_
#define SALDO_H_

#include <avr/io.h>
#include "LCDFunctions.h"
#include <stdio.h>

#define METAL (PIND & (1<<PIND7))
#define HERR (PINC & (1<<PINC7))
#define HALL (PINC & ((1<<PINC0) | (1<<PINC1) | (1<<PINC2)))
#define OPTICAL (PINC & ((1<<PINC3) | (1<<PINC4) | (1<<PINC5) | (1<<PINC6)))

unsigned char u8index, u8jindex,u8tempflag;
unsigned char u8aCoin_Hall[8];
unsigned char u8Coin_Optical;
unsigned char  u8Temp;

/*
unsigned int Coins()
{
		           
		unsigned int u16Coin = 0;
				                         
		for(u8index = 0; METAL == 0x80; u8index++){
			u8aCoin_Hall[u8index]= HALL;
			while((u8aCoin_Hall[u8index] == HALL) && (METAL == 0x80));
		}


		for(u8index = 0; u8index < 8; u8index++){
			for(u8jindex = 0; u8jindex < 8; u8jindex++){
				if (u8aCoin_Hall[u8index] <= u8aCoin_Hall[u8jindex]){
					u8Temp= u8aCoin_Hall[u8index];
					u8aCoin_Hall[u8index] = u8aCoin_Hall[u8jindex];
					u8aCoin_Hall[u8jindex] = u8Temp;
				}
			}
		}                                                       //Fin Hall
		
		
		while(OPTICAL == 0);
		u8Coin_Optical = (OPTICAL >> 3);
		switch (u8aCoin_Hall[7]){
			case 0x00:
			if(u8Coin_Optical == 0x01){
				u16Coin += 10;
			}
			break;
			case 0x01:
			if(u8Coin_Optical == 0x08){
				u16Coin += 1;
			}
			break;
			case 0x03:
			if(u8Coin_Optical == 0x04){
				u16Coin += 2;
			}
			break;
			case 0x07:
			if(u8Coin_Optical == 0x02){
				u16Coin += 5;
			}
			break;
			default:
			break;
		}
		for(u8index = 0; u8index<8;u8index++){
			u8aCoin_Hall[u8index] = 0;
		}
		return u16Coin;
}



*/
#endif /* SALDO_H_ */