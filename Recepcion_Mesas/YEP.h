/*
 * YEP.h
 *
 * Created: 14/05/2016 01:28:49 p. m.
 *  Author: joseluis
 */ 


#ifndef YEP_H_
#define YEP_H_

#include "MFRC522_NFC.h"
#include <stdio.h>

extern unsigned int ASCIITo_Decimal(unsigned char *ascii,unsigned char size);
extern unsigned int long ASCIITo_Octal(unsigned char *ascii,unsigned char size);
unsigned char Descontar_Card (unsigned int u16Pago);
void Generate_Yep(unsigned int long u16Pago,unsigned char u8Restaurant);
void Decode_SUP(unsigned int long  u16SUP, unsigned char u8Restaurant);


unsigned char u8aLCD_Mensaje[16];// i;
unsigned int decimal, convertion;


#endif /* YEP_H_ */