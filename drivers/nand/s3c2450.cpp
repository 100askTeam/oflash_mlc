#include <stdio.h>
#include "nand.h"
#include "cpu/arm/s3c24x0/pin2410.h"
#include "drivers/jtag/jtag.h"
#include "drivers/ft2232/ft2232.h"
#include "drivers/nand/s3c2450.h"

#define BAD_CHECK	(0)
#define ECC_CHECK	(0)

//*************** JTAG dependent functions ***************
static void NF_CMD(U8 cmd);
static void NF_RADDR(U8 raddr);
static void NF_nFCE_L(void);
static void NF_nFCE_H(void);
static U8 NF_RDRDATA(void);
static void NF_WRRDATA(U8 rdata);
static void NF_WAITRB(void);
//*************** H/W dependent functions ***************
static void NF_Reset(void);
static void NF_Init(void);
//*******************************************************


//*************************************************
//*************************************************
//**           H/W dependent functions           **
//************************************************* 
//*************************************************

// NAND Flash Memory Commands
#define	SEQ_RDATA_INPUT			(0x80)
#define	READ_ID				(0x90)
#define	RESET				(0xFF)
#define	READ_1_1			(0x00)
#define	READ_1_2			(0x01)
#define	READ_2				(0x50)
#define	PAGE_PROGRAM			(0x10)
#define	BLOCK_ERASE			(0x60)
#define	BLOCK_ERASE_CONFIRM		(0xD0)
#define	READ_STATUS			(0x70)


// block0: reserved for boot strap
// block1~4095: used for OS image
// badblock SE: xx xx xx xx xx 00 ....
// good block SE: ECC0 ECC1 ECC2 FF FF FF ....


static void NF_Reset(void)
{
    NF_nFCE_L();

    NF_CMD(0xFF);   //reset command

    Delay(1);	    //tWB = 100ns. 

    NF_WAITRB();    //wait 200~500us;
     
    NF_nFCE_H();    
}


static void NF_Init(void)
{
    NF_Reset();

    //NF_nFCE_L();
    NF_CMD(READ_1_1);        
    //NF_nFCE_H();
}


static void NF_CMD(U8 cmd)
{   
    
    //Command Latch Cycle
	S2450_ContRDataBus(HIGH); //low  //RD[15:0]=output
    
	S2450_SetPin(nFCE,LOW); 
    S2450_SetPin(nFRE,HIGH); 
    S2450_SetPin(nFWE,LOW); //Because tCLS=0, CLE & nFWE can be changed simultaneously.
    
	S2450_SetPin(ALE,LOW); 
    S2450_SetPin(CLE,HIGH); 
    
	S2450_SetRDataByte(cmd);
    JTAG_ShiftDRStateNoTdo(outCellValue); 

    S2450_SetPin(nFWE,HIGH);
    JTAG_ShiftDRStateNoTdo(outCellValue); 

    S2450_SetPin(CLE,LOW);	
	S2450_ContRDataBus(LOW); //high //RD[15:0]=input
    JTAG_ShiftDRStateNoTdo(outCellValue); 
}


static void NF_RADDR(U8 raddr)
{
    //rNFRADDR=raddr;
	S2450_ContRDataBus(HIGH); //low //RD[15:0]=output
    S2450_SetPin(nFCE,LOW); 
    S2450_SetPin(nFRE,HIGH); 
    S2450_SetPin(nFWE,LOW);
    S2450_SetPin(ALE,HIGH);
    S2450_SetPin(CLE,LOW);
    S2450_SetRDataByte(raddr);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
    
    S2450_SetPin(nFWE,HIGH);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
    
#if 1
    S2450_SetPin(ALE,LOW);	
	S2450_ContRDataBus(LOW); //high  //RD[15:0]=input
    JTAG_ShiftDRStateNoTdo(outCellValue); 
#endif
}


static void NF_nFCE_L(void)
{
    S2450_SetPin(nFCE_CON,HIGH); //low //Added to SJF2450 
	S2450_SetPin(nFCE,LOW); 
    JTAG_ShiftDRStateNoTdo(outCellValue); 
}


static void NF_nFCE_H(void)
{
    S2450_SetPin(nFCE,HIGH); 
    JTAG_ShiftDRStateNoTdo(outCellValue); 
}


static U8 NF_RDRDATA(void)
{
	S2450_ContRDataBus(LOW); //high //RD[15:0]=input
    S2450_SetPin(nFRE,LOW);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
    
    S2450_SetPin(nFRE,HIGH);
    JTAG_ShiftDRState(outCellValue,inCellValue); 
    return S2450_GetRDataByte();
}

static void NF_WRRDATA(U8 rdata)
{   
	S2450_ContRDataBus(HIGH); //low //RD[15:0]=output
    S2450_SetPin(nFWE,LOW);
    S2450_SetRDataByte(rdata);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
    
    S2450_SetPin(nFWE,HIGH);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
}


static void NF_WAITRB(void)
{
	while(1)
    {
		JTAG_ShiftDRState(outCellValue,inCellValue); 
	
		if( S2450_GetPin(RnB)==HIGH)
		break;
	}
}

struct nandflash_controller s3c2440_nandflash_controller = 
{
	JtagInit,
	NF_nFCE_L,
	NF_nFCE_H,
	NF_READY,
	NF_CMD,
	NF_ADDR,
	NF_RDDATA,
	NF_WRDATA,
	NF_RDDATA16,
	NF_WRDATA16,
	NF_WAITRB	
};


