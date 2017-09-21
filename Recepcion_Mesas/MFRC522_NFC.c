/*
 * MFRC522_NFC.c
 *
 * Created: 14/05/2016 03:08:50 p. m.
 *  Author: joseluis
 */ 

#include "MFRC522_NFC.h"

/* Description: write  TAG's memory bytes *****************************************
 * Input parameter: null
 * Return: null					 */
void writeTagBlockMemory(uchar *data4){
	//4 bytes Serial number of card, the 5 bytes is verify bytes
	uchar serNum[5];
	//buffer A password, 16 buffer, the password of every buffer is 6 bytes 
	uchar sectorX_KeyA[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; 
	uchar i,j,ij;
	uchar status;
    uchar str[MAX_LEN];
	for(ij = 0; ij < 5; ij++){
		//Search card, return card types
		status = MFRC522_Request(PICC_REQIDL, str);	
		if (status == MI_OK){  }
		//Prevent conflict, return the 4 bytes Serial number of the card
		status = MFRC522_Anticoll(str);
		for(i=0; i<5; i++){	 serNum[i]=str[i];  }	//memcpy(serNum, str, 5);
		if (status == MI_OK){  }
		status = MFRC522_SelectTag(serNum);	

		status = MFRC522_Auth(0x60,4,sectorX_KeyA,serNum);	//sector 1
		if(status==MI_OK){   
			writeTagBlockData(4,data4);
			}							
		MFRC522_Halt();
		delay1s();							}
		}

/* Description: Write data to TAG's memory ************************************
 * Input parameter: block to be written, dataArray
 * Return: null					 */
void writeTagBlockData(int block, uchar *data) {
	uchar status;
	char string[31];
	status = MFRC522_Write(block, data);
}



/* Description: Send data read to serial monitor ASCII format *******************
 * Input parameter: null
 * Return: null					 */
void readDataASCII(uchar bloque){
	//4 bytes Serial number of card, the 5 bytes is verfiy bytes
	uchar serNum[5];
	uchar ij;
	//buffer A password, 16 buffer, the passowrd of every buffer is 6 byte 
	uchar sectorX_KeyA[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; 
	uchar i,j;
	uchar status;
for(ij = 0; ij < 5; ij++){
		//Search card, return card types
		status = MFRC522_Request(PICC_REQIDL, read);	
		if (status == MI_OK){  }
		//Prevent conflict, return the 4 bytes Serial number of the card
		 status = MFRC522_Anticoll(read);
		for(i=0; i<5; i++){	 serNum[i]=read[i];  }	//memcpy(serNum, str, 5);
		if (status == MI_OK){   
                                                               }
		status = MFRC522_SelectTag(serNum);	
		status = MFRC522_Auth(0x60,bloque,sectorX_KeyA,serNum);	//sector 2
		//for(j=8;j<11;j++)					{
			status = MFRC522_Read(bloque, read);
		//}
			
		MFRC522_Halt();
		delay1s();
		
		
			 }}


/* Description: 1 s delay  *****************************************************
 * Input parameter: null
 * Return: null					 */
void delay1s(void){
	int i;
	for(i=0; i<5; i++){}}	// 1 s delay



/* Description: initilize RC522 ************************************************
 * Input parameter: null
 * Return: null					 */
void MFRC522_Init(void) {
						
	MFRC522_Reset(); 	
	//Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
    Write_MFRC522(TModeReg, 	0x8D);			//Tauto=1; f(Timer) = 6.78MHz/TPreScaler
    Write_MFRC522(TPrescalerReg,0x3E);			//TModeReg[3..0] + TPrescalerReg
    Write_MFRC522(TReloadRegL, 	30	);           
    Write_MFRC522(TReloadRegH, 	0	);	
	Write_MFRC522(TxAutoReg, 	0x40);			//100%ASK
	Write_MFRC522(ModeReg, 		0x3D);			//CRC initilizate value 0x6363	
	//ClearBitMask(Status2Reg, 	0x08);			//MFCrypto1On=0
	//Write_MFRC522(RxSelReg, 	0x86);			//RxWait = RxSelReg[5..0]
	//Write_MFRC522(RFCfgReg, 	0x7F);   		//RxGain = 48dB
	AntennaOn();				}				//turn on antenna

/* Description: write a byte data into one register of MFRC522 *****************
 * Input parameter: addr--register address; val--the value that need to write in
 * Return: Null						*/
void Write_MFRC522(uchar reg, uchar data) {
		ENABLE_CHIP();
		spi_transmit((reg<<1)&0x7E);
		spi_transmit(data);
		DISABLE_CHIP();		}	//digitalWrite(chipSelectPin, HIGH);	

/* Description: read a byte data into one register of MFRC522 ******************
 * Input parameter: addr--register address
 * Return: return the read value		*/
uchar Read_MFRC522(uchar reg) {
	uchar val;
	ENABLE_CHIP();
	spi_transmit(((reg<<1)&0x7E)|0x80);
	val = spi_transmit(0x00);
	DISABLE_CHIP();						//digitalWrite(chipSelectPin, HIGH);	
	return val;					}

/* Description: set RC522 register bit *****************************************
 * Input parameter:reg--register address; mask--value
 * Return: null						*/
void SetBitMask(uchar reg, uchar mask)  {
    uchar tmp;
    tmp = Read_MFRC522(reg);
    Write_MFRC522(reg, tmp | mask);  	}	// set bit mask

/* Description: clear RC522 register bit ***************************************
 * Input parameter: reg--register address; mask--value
 * Return: null 						*/
void ClearBitMask(uchar reg, uchar mask) {
    uchar tmp;
    tmp = Read_MFRC522(reg);
    Write_MFRC522(reg, tmp & (~mask));	 }  // clear bit mask

/* Description: Turn on antenna, every time turn on or shut down antenna need at least 1ms delay
 * Input parameter: null
 * Return: null						*/
void AntennaOn(void) {
	uchar temp;
	temp = Read_MFRC522(TxControlReg);
	if (!(temp & 0x03)){  SetBitMask(TxControlReg, 0x03);  } }

/* Description: Turn off antenna, every time turn on or shut down antenna need at least 1ms delay
 * Input parameter: null
 * Return: null						*/
void AntennaOff(void){	ClearBitMask(TxControlReg, 0x03);  }

/* Description: reset RC522 ****************************************************
 * Input parameter:null
 * Return:null					*/
void MFRC522_Reset(void){  Write_MFRC522(CommandReg, PCD_RESETPHASE);  }

/* Description: Searching card, read card type *********************************
 * Input parameter: reqMode -- search methods,
 *			 TagType--return card types
 *			 	0x4400 = Mifare_UltraLight
 *				0x0400 = Mifare_One(S50)
 *				0x0200 = Mifare_One(S70)
 *				0x0800 = Mifare_Pro(X)
 *				0x4403 = Mifare_DESFire
 * return:return MI_OK if successed		*/
uchar MFRC522_Request(uchar reqMode, uchar *TagType) {
	uchar status;  
	uint backBits;							//the data bits that received
	Write_MFRC522(BitFramingReg, 0x07);		//TxLastBists = BitFramingReg[2..0]
	TagType[0] = reqMode;
	status = MFRC522_ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);
	if ((status != MI_OK) || (backBits != 0x10)){  status = MI_ERR;  }
	return status;				}

/* Description: communicate between RC522 and ISO14443 *************************
 * Input parameter: command--MF522 command bits
 *			 sendData--send data to card via rc522
 *			 sendLen--send data length		 
 *			 backData--the return data from card
 *			 backLen--the length of return data
 * return: return MI_OK if successed				*/
uchar MFRC522_ToCard(uchar command, uchar *sendData, uchar sendLen, uchar *backData, uint *backLen){
    uchar status = MI_ERR;
    uchar irqEn = 	0x00;
    uchar waitIRq = 0x00;
    uchar lastBits;
    uchar n;
    uint i;
    switch (command) {
        case PCD_AUTHENT: 	{	//verify card password		
			irqEn = 0x12;
			waitIRq = 0x10;
			break;			}
		case PCD_TRANSCEIVE:{	//send data in the FIFO
			irqEn = 0x77;
			waitIRq = 0x30;
			break;			}
		default:	break; 	}
    Write_MFRC522(CommIEnReg, irqEn|0x80);	//Allow interruption
    ClearBitMask(CommIrqReg, 0x80);			//Clear all the interrupt bits
    SetBitMask(FIFOLevelReg, 0x80);			//FlushBuffer=1, FIFO initilizate
	Write_MFRC522(CommandReg, PCD_IDLE);	//NO action;cancel current command	
	//write data into FIFO
    for (i=0; i<sendLen; i++){ 	 Write_MFRC522(FIFODataReg, sendData[i]);   }
	//procceed it
	Write_MFRC522(CommandReg, command);
    if (command == PCD_TRANSCEIVE){   SetBitMask(BitFramingReg, 0x80);	} //StartSend=1,transmission of data starts  
	//waite receive data is finished
	i = 2000;	//i should adjust according the clock, the maxium the waiting time should be 25 ms
    do {
		//CommIrqReg[7..0]
		//Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
        n = Read_MFRC522(CommIrqReg);
        i--;
    }while ((i!=0) && !(n&0x01) && !(n&waitIRq));
    ClearBitMask(BitFramingReg, 0x80);			//StartSend=0	
    if (i != 0) {    
        if(!(Read_MFRC522(ErrorReg) & 0x1B))	//BufferOvfl Collerr CRCErr ProtecolErr
        {
            status = MI_OK;
            if (n & irqEn & 0x01){ 	status = MI_NOTAGERR;	}
            if (command == PCD_TRANSCEIVE){
               	n = Read_MFRC522(FIFOLevelReg);
              	lastBits = Read_MFRC522(ControlReg) & 0x07;
                if (lastBits){ 	*backLen = (n-1)*8 + lastBits;   }
                else{ 	*backLen = n*8;   }
                if (n == 0){  	n = 1;   }
                if (n > MAX_LEN){   n = MAX_LEN;   	}	
				//read the data from FIFO
                for (i=0; i<n; i++){   	backData[i] = Read_MFRC522(FIFODataReg); 	}
            }
        }
        else{	status = MI_ERR;  	}     
    }	
    //SetBitMask(ControlReg,0x80);           	//timer stops
    //Write_MFRC522(CommandReg, PCD_IDLE); 
    return status;					}

/* Description: Prevent conflict, read the card serial number ******************
 * Input parameter: serNum--return the 4 bytes card serial number, the 5th byte is recheck byte
 * return: return MI_OK if successed			*/
uchar MFRC522_Anticoll(uchar *serNum){
    uchar status;
    uchar i;
	uchar serNumCheck=0;
    uint unLen;
    //ClearBitMask(Status2Reg, 0x08);		//TempSensclear
    //ClearBitMask(CollReg,0x80);			//ValuesAfterColl
	Write_MFRC522(BitFramingReg, 0x00);		//TxLastBists = BitFramingReg[2..0]
    serNum[0] = PICC_ANTICOLL;
    serNum[1] = 0x20;
    status = MFRC522_ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);
    if (status == MI_OK){
		//Verify card serial number
		for (i=0; i<4; i++){	serNumCheck ^= serNum[i];	}
		if (serNumCheck != serNum[i]){ 	status = MI_ERR;    }
    }
    //SetBitMask(CollReg, 0x80);		//ValuesAfterColl=1
    return status;					} 				

/* Description: Use MF522 to caculate CRC **************************************
 * Input parameter: pIndata--the CRC data need to be read,len--data length,pOutData-- the caculated result of CRC
 * return: Null
 */
void CalulateCRC(uchar *pIndata, uchar len, uchar *pOutData){
    uchar i, n;
    ClearBitMask(DivIrqReg, 0x04);			//CRCIrq = 0
    SetBitMask(FIFOLevelReg, 0x80);			//Clear FIFO pointer
    //Write_MFRC522(CommandReg, PCD_IDLE);
	//Write data into FIFO	
    for (i=0; i<len; i++){ 	Write_MFRC522(FIFODataReg, *(pIndata+i));   }
    Write_MFRC522(CommandReg, PCD_CALCCRC);
	//waite CRC caculation to finish
    i = 0xFF;
    do {
        n = Read_MFRC522(DivIrqReg);
        i--;
    }while ((i!=0) && !(n&0x04));			//CRCIrq = 1
	//read CRC caculation result
    pOutData[0] = Read_MFRC522(CRCResultRegL);
    pOutData[1] = Read_MFRC522(CRCResultRegM);				}

/* Description: Select card, read card storage volume *************************
 * Input parameter :serNum--Send card serial number
 * return: return the card storage volume			 */
uchar MFRC522_SelectTag(uchar *serNum) {
    uchar i;
	uchar status;
	uchar size;
    uint recvBits;
    uchar buffer[9]; 
	//ClearBitMask(Status2Reg, 0x08);			//MFCrypto1On=0
    buffer[0] = PICC_SElECTTAG;
    buffer[1] = 0x70;
    for (i=0; i<5; i++){  buffer[i+2] = *(serNum+i);  }
	CalulateCRC(buffer, 7, &buffer[7]);		
    status = MFRC522_ToCard(PCD_TRANSCEIVE, buffer, 9, buffer, &recvBits);
    if ((status == MI_OK) && (recvBits == 0x18)){ 	size = buffer[0]; 	}
    else{ 	size = 0;  	}
    return size;						}

/* Description:verify card password ********************************************
 * Input parameters:authMode--password verify mode
                 0x60 = verify A passowrd key 
                 0x61 = verify B passowrd key 
             BlockAddr--Block address
             Sectorkey--Block password
             serNum--Card serial number ,4 bytes
 * return:return MI_OK if successed				*/
uchar MFRC522_Auth(uchar authMode, uchar BlockAddr, uchar *Sectorkey, uchar *serNum) {
    uchar status;
    uint recvBits;
    uchar i;
	uchar buff[12]; 
	//Verify command + block address + buffer password + card SN
    buff[0] = authMode;
    buff[1] = BlockAddr;
    for (i=0; i<6; i++){	buff[i+2] = *(Sectorkey+i);   }
    for (i=0; i<4; i++){  	buff[i+8] = *(serNum+i);   	  }
    status = MFRC522_ToCard(PCD_AUTHENT, buff, 12, buff, &recvBits);
    if ((status != MI_OK) || (!(Read_MFRC522(Status2Reg) & 0x08))){	 status = MI_ERR;  }
    return status;																	   }

/* Description: Read data ******************************************************
 * Input parameters: blockAddr--block address; recvData--the block data which are read
 * return: return MI_OK if successed						*/
uchar MFRC522_Read(uchar blockAddr, uchar *recvData) {
    uchar status;
    uint unLen;
    recvData[0] = PICC_READ;
    recvData[1] = blockAddr;
    CalulateCRC(recvData,2, &recvData[2]);
    status = MFRC522_ToCard(PCD_TRANSCEIVE, recvData, 4, recvData, &unLen);
    if ((status != MI_OK) || (unLen != 0x90)) {  status = MI_ERR;  } 
    return status;									}

/* Description: write block data ***********************************************
 * Input parameters: blockAddr--block address; writeData--Write 16 bytes data into block
 * return: return MI_OK if successed						*/
uchar MFRC522_Write(uchar blockAddr, uchar *writeData) {
    uchar status;
    uint recvBits;
    uchar i;
	uchar buff[18];     
    buff[0] = PICC_WRITE;
    buff[1] = blockAddr;
    CalulateCRC(buff, 2, &buff[2]);
    status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff, &recvBits);
    if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A)){  status = MI_ERR;    }
    if (status == MI_OK){
        for (i=0; i<16; i++){   buff[i] = *(writeData+i);   }	//Write 16 bytes data into FIFO
        CalulateCRC(buff, 16, &buff[16]);
        status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 18, buff, &recvBits);
		if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A)){  status = MI_ERR; }
    }
    return status;										}

/* Description: Command the cards into sleep mode ******************************
 * Input parameters: null
 * return: null 						*/
void MFRC522_Halt(void){
	uchar status;
    uint unLen;
    uchar buff[4]; 
    buff[0] = PICC_HALT;
    buff[1] = 0;
    CalulateCRC(buff, 2, &buff[2]);
    status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff,&unLen);			}
