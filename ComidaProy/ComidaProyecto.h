/*
 * ComidaProyecto.h
 *
 * Created: 26/04/2016 11:35:09 a. m.
 *  Author: joseluis
 */ 


#ifndef COMIDAPROYECTO_H_
#define COMIDAPROYECTO_H_


#include <avr/io.h>
#include <avr/interrupt.h>
#include "LCDFunctions.h"
#include "SPI.h"
#include "MFRC522_NFC.h"
#include <stdio.h>
#include "CardDetect.h"
#include "Saldo.h"



#define BUTTONS (PINB & (0x03))
#define CIEN (PIND & (1<<PIND2))

unsigned char Coin_Cycle();
unsigned char gfbillete;



#endif /* COMIDAPROYECTO_H_ */