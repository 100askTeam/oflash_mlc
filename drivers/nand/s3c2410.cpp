#include "stdafx.h"
#ifdef WIN32
#define  _X86_     //   _X86_是Intel的CPU，还有_ALPHA_、_PPC_等，都在windows.h中有定义   
#include <windef.h>   
#include <winnt.h>   
#include <winbase.h>
#endif
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include "nand.h"
#include "cpu/arm/s3c24x0/pin2410.h"
#include "drivers/jtag/jtag.h"
#include "drivers/ft2232/ft2232.h"
#include "drivers/nand/s3c2410.h"

#define BAD_CHECK	(0)
#define ECC_CHECK	(0)

//*************** JTAG dependent functions ***************
static void JtagInit(void);
static void NF_CMD(U8 cmd);
static void NF_ADDR(U8 addr);
static void NF_nFCE_L(void);
static void NF_nFCE_H(void);
static U8 NF_RDDATA(void);
static void NF_WRDATA(U8 data);
static void NF_WAITRB(void);
//*************** H/W dependent functions ***************
static void NF_Reset(void);
static void NF_Init(void);
//*******************************************************

extern int delayLoopCount;
static void Delay(int count) // unit = 100ns
{
    
    int i,j;
    for(i=0 ; i<count ; i++)
        for(j=0;j<delayLoopCount;j++);
}



//*************************************************
//*************************************************
//**           H/W dependent functions           **
//************************************************* 
//*************************************************

// NAND Flash Memory Commands
#define	SEQ_DATA_INPUT			(0x80)
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

#define WRITEVERIFY  (0)  //verifing is enable at writing.

/*
#define NF_CMD(cmd)	{rNFCMD=cmd;}
#define NF_ADDR(addr)	{rNFADDR=addr;}	
#define NF_nFCE_L()	{rNFCONF&=~(1<<11);}
#define NF_nFCE_H()	{rNFCONF|=(1<<11);}
#define NF_RSTECC()	{rNFCONF|=(1<<12);}
#define NF_RDDATA() 	(rNFDATA)
#define NF_WRDATA(data) {rNFDATA=data;}

#define NF_WAITRB()    {while(!(rNFSTAT&(1<<0)));} 
	    //wait tWB and check F_RNB pin.   
*/

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



//*************************************************
//*************************************************
//**     JTAG dependent primitive functions      **
//************************************************* 
//*************************************************
static void JtagInit(void)
{
    JTAG_RunTestldleState();
    JTAG_ShiftIRState(EXTEST);

    S2410_SetPin(CLE,LOW); 
    S2410_SetPin(ALE,LOW); 

	//NF_Init();
}


static void NF_CMD(U8 cmd)
{       
    //Command Latch Cycle
    S2410_SetPin(DATA0_7_CON ,LOW); //D[7:0]=output
    S2410_SetPin(nFCE,LOW); 
    S2410_SetPin(nFRE,HIGH); 
    S2410_SetPin(nFWE,LOW); //Because tCLS=0, CLE & nFWE can be changed simultaneously.
    S2410_SetPin(ALE,LOW); 
    S2410_SetPin(CLE,HIGH); 
    S2410_SetDataByte(cmd);
    JTAG_ShiftDRStateNoTdo(outCellValue); 

    S2410_SetPin(nFWE,HIGH);
    JTAG_ShiftDRStateNoTdo(outCellValue); 

#if 1
    S2410_SetPin(CLE,LOW);	
    S2410_SetPin(DATA0_7_CON,HIGH); //D[7:0]=input
    JTAG_ShiftDRStateNoTdo(outCellValue); 
#endif
}


static void NF_ADDR(U8 addr)
{
    //rNFADDR=addr;
    S2410_SetPin(DATA0_7_CON ,LOW); //D[7:0]=output
    S2410_SetPin(nFCE,LOW); 
    S2410_SetPin(nFRE,HIGH); 
    S2410_SetPin(nFWE,LOW);
    S2410_SetPin(ALE,HIGH);
    S2410_SetPin(CLE,LOW);
    S2410_SetDataByte(addr);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
    
    S2410_SetPin(nFWE,HIGH);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
    
#if 1
    S2410_SetPin(ALE,LOW);	
    S2410_SetPin(DATA0_7_CON,HIGH); //D[7:0]=input
    JTAG_ShiftDRStateNoTdo(outCellValue); 
#endif
}


static void NF_nFCE_L(void)
{
    S2410_SetPin(nFCE,LOW); 
    JTAG_ShiftDRStateNoTdo(outCellValue); 
}


static void NF_nFCE_H(void)
{
    S2410_SetPin(nFCE,HIGH); 
    JTAG_ShiftDRStateNoTdo(outCellValue); 
}


static U8 NF_RDDATA(void)
{
    S2410_SetPin(DATA0_7_CON ,HIGH); //D[7:0]=input
    S2410_SetPin(nFRE,LOW);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
    
    S2410_SetPin(nFRE,HIGH);
    JTAG_ShiftDRState(outCellValue,inCellValue); 
    return S2410_GetDataByte();
}

static U16 NF_RDDATA16(void)
{
    S2410_SetPin(DATA0_7_CON ,HIGH); //D[7:0]=input
    S2410_SetPin(DATA8_15_CON ,HIGH); //D[15:8]=input
    S2410_SetPin(nFRE,LOW);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
    
    S2410_SetPin(nFRE,HIGH);
    JTAG_ShiftDRState(outCellValue,inCellValue); 
    return S2410_GetDataHW();
}

static void NF_WRDATA(U8 data)
{   
    S2410_SetPin(DATA0_7_CON ,LOW); //D[7:0]=output
    S2410_SetPin(nFWE,LOW);
    S2410_SetDataByte(data);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
    
    S2410_SetPin(nFWE,HIGH);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
}

static void NF_WRDATA16(U16 data)
{   
    S2410_SetPin(DATA0_7_CON ,LOW); //D[7:0]=output
    S2410_SetPin(DATA8_15_CON ,LOW); //D[15:8]=output
    S2410_SetPin(nFWE,LOW);
    S2410_SetDataHW(data);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
    
    S2410_SetPin(nFWE,HIGH);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
}


static void NF_WAITRB(void)
{
    while(1)
    {
	JTAG_ShiftDRState(outCellValue,inCellValue); 
	if( S2410_GetPin(nWAIT)==HIGH && S2410_GetPin(NCON0)==HIGH)break;
    }
}

static int NF_READY(void)
{
	JTAG_ShiftDRState(outCellValue,inCellValue); 
	return (S2410_GetPin(nWAIT)==HIGH && S2410_GetPin(NCON0)==HIGH);
}


struct nandflash_controller s3c2410_nandflash_controller = 
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



