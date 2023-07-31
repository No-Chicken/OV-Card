#ifndef __RC522_H__
#define __RC522_H__

#include "stm32l0xx.h"

#define RST_PORT												GPIOA
#define RST_PIN													GPIO_PIN_8														
#define RST_H                           HAL_GPIO_WritePin(RST_PORT, RST_PIN,1)
#define RST_L                           HAL_GPIO_WritePin(RST_PORT, RST_PIN,0)

/////////////////////////////////////////////////////////////////////
//MF522	command
/////////////////////////////////////////////////////////////////////
#define PCD_IDLE              0x00               //
#define PCD_AUTHENT           0x0E               //
#define PCD_RECEIVE           0x08               //
#define PCD_TRANSMIT          0x04               //
#define PCD_TRANSCEIVE        0x0C               //
#define PCD_RESETPHASE        0x0F               //
#define PCD_CALCCRC           0x03               //CRC

/////////////////////////////////////////////////////////////////////
//Mifare_One command
/////////////////////////////////////////////////////////////////////
#define PICC_REQIDL           0x26               //
#define PICC_REQALL           0x52               //
#define PICC_ANTICOLL1        0x93               //
#define PICC_ANTICOLL2        0x95               //
#define PICC_AUTHENT1A        0x60               //
#define PICC_AUTHENT1B        0x61               //
#define PICC_READ             0x30               //
#define PICC_WRITE            0xA0               //
#define PICC_DECREMENT        0xC0               //
#define PICC_INCREMENT        0xC1               //
#define PICC_RESTORE          0xC2               //
#define PICC_TRANSFER         0xB0               //
#define PICC_HALT             0x50               //

/////////////////////////////////////////////////////////////////////
//MF522 FIFO Lenth
/////////////////////////////////////////////////////////////////////
#define DEF_FIFO_LENGTH       64                 //FIFO size=64byte

/////////////////////////////////////////////////////////////////////
//MF522 register
/////////////////////////////////////////////////////////////////////
// PAGE 0
#define     RFU00                 0x00    
#define     CommandReg            0x01    
#define     ComIEnReg             0x02    
#define     DivlEnReg             0x03    
#define     ComIrqReg             0x04    
#define     DivIrqReg             0x05
#define     ErrorReg              0x06    
#define     Status1Reg            0x07    
#define     Status2Reg            0x08    
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     RFU0F                 0x0F
// PAGE 1     
#define     RFU10                 0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     RFU1A                 0x1A
#define     RFU1B                 0x1B
#define     MifareReg             0x1C
#define     RFU1D                 0x1D
#define     RFU1E                 0x1E
#define     SerialSpeedReg        0x1F
// PAGE 2    
#define     RFU20                 0x20  
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     RFU23                 0x23
#define     ModWidthReg           0x24
#define     RFU25                 0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsCfgReg            0x28
#define     ModGsCfgReg           0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
// PAGE 3      
#define     RFU30                 0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39  
#define     TestDAC2Reg           0x3A   
#define     TestADCReg            0x3B   
#define     RFU3C                 0x3C   
#define     RFU3D                 0x3D   
#define     RFU3E                 0x3E   
#define     RFU3F		          		0x3F


#define     REQ_ALL               0x52
#define     KEYA                  0x60

/////////////////////////////////////////////////////////////////////
//erro
/////////////////////////////////////////////////////////////////////
#define MI_OK                          (char)0
#define MI_NOTAGERR                    (char)(-1)
#define MI_ERR                         (char)(-2)
	
uint8_t RC522_ReadReg(uint8_t addr);
void RC522_WriteReg(uint8_t addr,uint8_t dat);
void RC522_PowerOff(void);
uint8_t RC522_Init(void);

char PcdReset(void);
void PcdAntennaOn(void);
void PcdAntennaOff(void);
char PcdRequest(unsigned char req_code,unsigned char *pTagType);   
char PcdAnticoll(unsigned char *pSnr);
char PcdSelect(unsigned char *pSnr);         
char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr);     
char PcdRead(unsigned char addr,unsigned char *pData);     
char PcdWrite(unsigned char addr,unsigned char *pData);    
char PcdValue(unsigned char dd_mode,unsigned char addr,unsigned char *pValue);   
char PcdBakValue(unsigned char sourceaddr, unsigned char goaladdr);                                 
char PcdHalt(void);
char PcdComMF522(unsigned char Command, 
                 unsigned char *pInData, 
                 unsigned char InLenByte,
                 unsigned char *pOutData, 
                 unsigned int  *pOutLenBit);
void CalculateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData);
void SetBitMask(unsigned char reg,unsigned char mask); 
void ClearBitMask(unsigned char reg,unsigned char mask); 
char M500PcdConfigISOType(unsigned char type);
void WaitCardOff(void);


#endif