
#include <avr/io.h>

#define Direction_RTC_Writte_Mode (0xD0)
#define Direction_RTC_Read_Mode   (0xD1)
#define CLK_TWBR_Config           (0x66)
#define CLK_TWSR_Config           (0x00)
#define Reset_Pointer             (0x00)

typedef unsigned char u8char_t;

void Configuration_I2C (void);
void Start_I2C (void);
void Stop_I2C (void);
void Send_I2C (u8char_t Data_I2C);
u8char_t Get_I2C (void);
void Send_RTC (u8char_t Data_String[]);
void Reset_Register_Pointer (u8char_t x);
void Get_RTC (u8char_t Data_RTC[]);
void Show_RTC(void);
u8char_t *Get_Data_RTC;
u8char_t Set_Configuration[8];
u8char_t RTC_Information[7];

u8char_t Time  [5]={"Time:"};
u8char_t Continue [11]={"Continue: A"};
u8char_t Su[3]={"Sun"}, Mo[3]={"Mon"}, Tu[3]={"Tue"}, We[3]={"Wed"}, Th[3]={"Thu"}, Fr[3]={"Fri"}, Sa[3]={"Sat"};
u8char_t Day_Week [1]={0x89};

void Configuration_I2C (void) {
	//Set the SCL to 100Khz
	TWBR = CLK_TWBR_Config;
	TWSR = CLK_TWSR_Config;
	//Enable TWI module
	TWCR = (1<<TWEN);
}

void Start_I2C (void) {
	//Start condition
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	//Waiting the start condition
	while (!(TWCR & (1<<TWINT)));
}

void Stop_I2C (void) {
	//Stop condition
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void Send_I2C (u8char_t Data_I2C) {
	//Get value for I2C bus
	TWDR = Data_I2C;
	//Clear the interrupt
	TWCR = (1<<TWINT)|(1<<TWEN);
	//Waiting to send Data_I2C
	while (!(TWCR & (1<<TWINT)));
}

u8char_t Get_I2C (void) {
	//Clear the interrupt
	TWCR = (1<<TWINT)|(1<<TWEN);
	//Waiting to receive Data_I2C
	while (!(TWCR & (1<<TWINT)));
	//Get out the value
	return TWDR;
}

void Send_RTC (u8char_t Data_String[]) {
	//Send initial conditions
	u8char_t x;
	Start_I2C();
	Send_I2C(Direction_RTC_Writte_Mode);
	Send_I2C(Reset_Pointer);
	//Send string
	for(x=0; x<8; x++) {
		Send_I2C(Data_String[x]);
	}
	Stop_I2C();
}

void Reset_Register_Pointer (u8char_t x){
	//Reset vector of register pointer to x address
	Start_I2C();
	Send_I2C(Direction_RTC_Writte_Mode);
	Send_I2C(x);
	Stop_I2C();
}

void Get_RTC (u8char_t Data_RTC[]) {
	//Set initial conditions
	char x;
	Get_Data_RTC=Data_RTC;
	for (x=0; x<7; x++, Get_Data_RTC++) {
	Reset_Register_Pointer(x);
	Start_I2C();
	Send_I2C(Direction_RTC_Read_Mode);
	*Get_Data_RTC=Get_I2C();
	}
	Stop_I2C();
}

void RTC_Configuration (void){
u8char_t x,y,z;
LCD_Clean();
LCD_MESSG(Time,10);
LCD_JUMP();

for(x=0; x<7; x++){
	regreso0: y=Keyboard();
	if (y>=0x0A){goto regreso0;}
	ASCII(y);
	regreso1: z=Keyboard();
	if (z>=0x0A){goto regreso1;}
	ASCII(z);
	y=(y<<4);
	z|=y;
	Set_Configuration[x]=z;
	LCD_JUMP();
}
LCD_Clean();
LCD_MESSG(Continue,22);
x=Keyboard();
while(x != 0x0A){x=Keyboard();}
LCD_Clean();

Send_RTC(Set_Configuration);
	
	Get_RTC(RTC_Information);
	Show_RTC();

}

void Show_RTC (void){
	u8char_t  x; 
	for (x=0; x<7; x++) {
		
		if (x==0) {LCD_Clean();}
		else if (x==4) {LCD_JUMP();}
		else if (x==3) {LCD_INST(Day_Week);}
		
		if(x!=3){
			BCD_2_ASCII(RTC_Information[6-x]);
		}
		else{
			switch(RTC_Information[3]){
				case 0x01: LCD_MESSG(Su,6); break;
				case 0x02: LCD_MESSG(Mo,6); break;
				case 0x03: LCD_MESSG(Tu,6); break;
				case 0x04: LCD_MESSG(We,6); break;
				case 0x05: LCD_MESSG(Th,6); break;
				case 0x06: LCD_MESSG(Fr,6); break;
				case 0x07: LCD_MESSG(Sa,6); break;
			}
		}
		
		if (x==2 || x==6 || x==3);
		else if(x==0 || x==1){
			LCD_DATA('/');
		}
		else{LCD_DATA(':');}
		
	}
	LCD_OK();
	x=Keyboard();
	while(x != 0x0E){x=Keyboard();}
}

