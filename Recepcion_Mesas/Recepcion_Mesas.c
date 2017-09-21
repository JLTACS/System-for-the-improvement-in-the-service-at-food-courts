/*
 * Recepcion_Mesas.c
 *
 * Created: 14/05/2016 11:36:32 a. m.
 *  Author: joseluis
 */ 


#include "Recepcion_Mesas.h"

int main(void)
{
	unsigned char i;
	
/* Inicializacion de modulos y perifericos*/	
 spi_init();
 MFRC522_Init();
 Configuration_I2C();
 LCD_INIT();
 
 /* Configuracion de hora y fecha por RTC*/
 Get_RTC(RTC_Information);
 Show_RTC();
 LCD_Clean();
 snprintf((char*)&u8aLCD_Mensaje[0],17,"Modificar Hora   ");
 LCD_MESSG(u8aLCD_Mensaje,32);
 LCD_JUMP();
 snprintf((char*)&u8aLCD_Mensaje[0],17,"SI            NO");
 LCD_MESSG(u8aLCD_Mensaje,32);
 
 u8Opcion = Keyboard();
 while(u8Opcion != 0x0F && u8Opcion != 0x0E){
	 u8Opcion = Keyboard();
 }
 
 if(u8Opcion == 0x0F){
 CorreccionHora: RTC_Configuration();
 }
 
 /* Confirmacion de Fecha y Hora */
  LCD_Clean();
  snprintf((char*)&u8aLCD_Mensaje[0],17,"¿Fecha correcta?");
  LCD_MESSG(u8aLCD_Mensaje,32);
  LCD_JUMP();
  snprintf((char*)&u8aLCD_Mensaje[0],17,"SI            NO");
  LCD_MESSG(u8aLCD_Mensaje,32);
 
  u8Opcion = Keyboard();
  while(u8Opcion != 0x0F && u8Opcion != 0x0E){
	  u8Opcion = Keyboard();
  }
  
  if(u8Opcion == 0x0E){
	  goto CorreccionHora;
  }
 
 /* Mensaje de Bienvenido */
 for(;;){ 
  LCD_Clean();
  snprintf((char*)&u8aLCD_Mensaje[0],17,"Bienvenido      ");
  LCD_MESSG(u8aLCD_Mensaje,32);
  LCD_OK();
  
  u8Opcion = Keyboard();
  while(u8Opcion != 0x0E){
	  u8Opcion = Keyboard();
  }
  
  /* Seleccionar Accion */
   LCD_Clean();
   snprintf((char*)&u8aLCD_Mensaje[0],17,"Pagar          A");
   LCD_MESSG(u8aLCD_Mensaje,32),
   LCD_JUMP();
   snprintf((char*)&u8aLCD_Mensaje[0],17,"Reembolso      B");
   LCD_MESSG(u8aLCD_Mensaje,32);
  
  u8Opcion = Keyboard(); 
  while(u8Opcion != 0x0A && u8Opcion != 0x0B){
	  u8Opcion = Keyboard();
  }
   LCD_Clean();  
   Detect_Card(); //Detectar Tarjeta
   /*Seleccionar Restaurante*/
   LCD_Clean();
   snprintf((char*)&u8aLCD_Mensaje[0],17,"Restaurante:    ");
   LCD_MESSG(u8aLCD_Mensaje,32),
   LCD_JUMP();
   snprintf((char*)&u8aLCD_Mensaje[0],17," 1      2      3");
   LCD_MESSG(u8aLCD_Mensaje,32);
   
   u8Restaurant = Keyboard();
   while(u8Restaurant < 0x01 || u8Restaurant > 0x03){
	   u8Restaurant = Keyboard();
   }
   
   /* Pagar */
  if(u8Opcion == 0x0A){
  Regresar:	  LCD_Clean();
	 
	 snprintf((char*)&u8aLCD_Mensaje[0],17,"Pago:              ");
	 LCD_MESSG(u8aLCD_Mensaje,32);
	 LCD_OK();
	 u8aInstruccion[0] = 0x86;
	 LCD_INST(u8aInstruccion); 
	 
	 /*Introduccion del pago*/
	 for(i=0; i<5; i++){
	 u8aBuffer_Keyboard[i] = Keyboard();
	   if (u8aBuffer_Keyboard[i] != 0x0F && u8aBuffer_Keyboard[i] != 0x0E){
		   u8aBuffer_Keyboard[i] = ASCII(u8aBuffer_Keyboard[i]);
	   }else{
		    u8aBuffer_Keyboard[i] = 0x20;
		   i += (5-i);
	   }  
	 }
	
	   u16Pago = ASCIITo_Decimal(&u8aBuffer_Keyboard[0],4);
	   for(i=0; i<10; i++){
		   u8aBuffer_Keyboard[i] = 0x00;
	   }
	  
	   LCD_Clean();
	   
	   /*Confirmar Pago*/
	   snprintf((char*)&u8aLCD_Mensaje[0],17,"Confirma: %d     ",u16Pago);
	   LCD_MESSG(u8aLCD_Mensaje,32);
	   LCD_JUMP();
	   snprintf((char*)&u8aLCD_Mensaje[0],17,"Regresar      OK");
	   LCD_MESSG(u8aLCD_Mensaje,32);
	  
	u8Opcion = Keyboard();
	  while(u8Opcion != 0x0F && u8Opcion != 0x0E){
		  u8Opcion = Keyboard();
	  }
	  
	  if(u8Opcion == 0x0F){
		 goto Regresar;              //Regresar a ingreso de pago
	  }else if(u8Opcion == 0x0E){
		  LCD_Clean();
		 if (Descontar_Card(u16Pago)){     //Descontar Saldo y Verificar suficiente
		  Generate_Yep(u16Pago,u8Restaurant);  //Generar YEP
		  LCD_JUMP();
		  LCD_OK();
		 }else{
		  snprintf((char*)&u8aLCD_Mensaje[0],17,"Saldo            ");  //Mensaje de Saldo Insuficiente
		  LCD_MESSG(u8aLCD_Mensaje,32);
		  LCD_JUMP();
		  snprintf((char*)&u8aLCD_Mensaje[0],17,"Insuficiente  OK");
		  LCD_MESSG(u8aLCD_Mensaje,32);
		  }
		u8Opcion = Keyboard();
		  while(u8Opcion != 0x0E){
		  u8Opcion = Keyboard();
		  }
		  
	  }
	   /*Reembolso*/
  }else if(u8Opcion == 0x0B ){
	   RegresarB:	  LCD_Clean();
	   snprintf((char*)&u8aLCD_Mensaje[0],17,"SUP:            ");
	   LCD_MESSG(u8aLCD_Mensaje,32);
	   LCD_OK();
	   u8aInstruccion[0] = 0x85;
	   LCD_INST(u8aInstruccion);
	   u16Pago = 0;
	   /* Ingresar codigo de reembolso */
	   for(i=0; i<10; i++){
		  u8aBuffer_Keyboard[i] = Keyboard();
		   if (u8aBuffer_Keyboard[i] != 0x0F && u8aBuffer_Keyboard[i] != 0x0E){
			   u8aBuffer_Keyboard[i] = ASCII(u8aBuffer_Keyboard[i]);
			   }else{
			   u8aBuffer_Keyboard[i] = 0x20;
			   i += (10-i);
		   }
	   }
	   
	  u16SUP = ASCIITo_Octal(&u8aBuffer_Keyboard[0],9);
	  for(i=0; i<10; i++){
		  u8aBuffer_Keyboard[i] = 0x00;
	  }
	  
	   LCD_Clean();
	   /* Confirmar codigo*/
	   snprintf((char*)&u8aLCD_Mensaje[0],17,"Confirm: %lo     ",u16SUP);
	   LCD_MESSG(u8aLCD_Mensaje,32);
	   LCD_JUMP();
	   snprintf((char*)&u8aLCD_Mensaje[0],17,"Regresar      OK");
	   LCD_MESSG(u8aLCD_Mensaje,32);
	   
	   u8Opcion = Keyboard();
	   while(u8Opcion != 0x0F && u8Opcion != 0x0E){
		   u8Opcion = Keyboard();
	   }
	   
	   if(u8Opcion == 0x0F){
		   goto RegresarB;  //Regresar ingreso de codigo
		}else if(u8Opcion == 0x0E){
		   LCD_Clean();
		   Decode_SUP(u16SUP,u8Restaurant); //Decodificar SUP
			   LCD_OK();
			   LCD_MESSG(u8aLCD_Mensaje,4);
			   
		 u8Opcion = Keyboard();
		   while(u8Opcion != 0x0E){
			   u8Opcion = Keyboard();
		   }
		   
	   }
	  
  }
  LCD_Clean();
	  }
}
