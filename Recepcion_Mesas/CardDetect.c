/*
 * CFile1.c
 *
 * Created: 26/04/2016 10:38:31 a. m.
 *  Author: joseluis
 */ 



#include <avr/io.h>



unsigned char byte;
unsigned char str[MAX_LEN];

void Detect_Card(void){
	  byte = MFRC522_Request(PICC_REQALL,str);
	  
	  while(byte != MI_OK)
	  {
		  byte = MFRC522_Request(PICC_REQALL,str);
	  }
	
}