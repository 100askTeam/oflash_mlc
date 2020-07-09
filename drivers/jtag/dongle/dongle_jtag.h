#ifndef __DONGLE_JTAG_H__
#define __DONGLE_JTAG_H__

#include "../jtag.h"
#include "ppt.h"

#define LOW 			 '0'               
#define HIGH			 '1'
   

// Pin Connections
//   TCK   :DATA[0]   (2) 
//   TDI   :DATA[1]   (3) 
//   TMS   :DATA[2]   (4) 
//   TDO   :STATUS[7] (11)

#define TCK_H		0x01
#define TDI_H		0x02              
#define TMS_H		0x04
#define TCK_L		0x00
#define TDI_L		0x00              
#define TMS_L		0x00
#define Dongle_JTAG_SET(value)	OutputPpt(value)

#define Dongle_JTAG_GET_TDO()	( (InputPpt()&(1<<7)) ? LOW:HIGH )  //STATUS7 is read inverted. 		


// JTAG Instruction Definition for S3C2410
#define EXTEST		"0000"  //LSB...MSB
#define BYPASS		"1111"
#define IDCODE		"0111"
#define SAMPLE_PRELOAD	"1100"

extern int delayLoopCount;
extern struct jtag_operations Dongle_opr;

int Dongle_JTAG_ReadId(void);
void Dongle_JTAG_RunTestldleState(void);
void Dongle_JTAG_ShiftIRState(char *wrIR);
void Dongle_JTAG_ShiftDRState(char *wrDR, char *rdDR);
void  Dongle_JTAG_ShiftDRStateNoTdo(char *wrDR);
void Dongle_Delay(int count);
#endif //__DONGLE_JTAG_H__



