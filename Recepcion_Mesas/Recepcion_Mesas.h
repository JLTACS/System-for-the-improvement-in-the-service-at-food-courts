/*
 * Recepcion_Mesas.h
 *
 * Created: 14/05/2016 03:26:21 p. m.
 *  Author: joseluis
 */ 


#ifndef RECEPCION_MESAS_H_
#define RECEPCION_MESAS_H_

#include <avr/io.h>
#include <stdio.h>
#include "SPI.h"
#include "MFRC522_NFC.h"
#include "CardDetect.h"
#include "LCDFunctions.h"
#include "I2C_ATmega8.h"
#include "YEP.h"


unsigned char u8aBuffer_Keyboard[10];
unsigned int u16Pago;
unsigned int long u16SUP;
unsigned char u8Opcion;
unsigned char u8Restaurant;






#endif /* RECEPCION_MESAS_H_ */