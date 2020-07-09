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
#include "cpu/arm/s3c6410/pin6410.h"
#include "drivers/jtag/jtag.h"
#include "drivers/ft2232/ft2232.h"
#include "drivers/nand/s3c6410.h"


#define BAD_CHECK	(0)
#define ECC_CHECK	(0)

//*************** JTAG dependent functions ***************
static void JtagInit(void);
static void NF_CMD(U8 cmd);
static void NF_RADDR(U8 raddr);
static void NF_nFCE_L(void);
static void NF_nFCE_H(void);
static U8 NF_RDRDATA(void);
static void NF_WRRDATA(U8 rdata);
static void NF_WAITRB(void);
//*************** H/W dependent functions ***************
static U16 NF_CheckId(void);
static int NF_ReadPage(U32 block,U32 page,U8 *buffer,U8 *spareBuf);
static int NF_WritePage(U32 block,U32 page,U8 *buffer,U8 *spareBuf);
//buffer size is 512 bytes
static int NF_IsBadBlock(U32 block);
static int NF_MarkBadBlock(U32 block);
static void NF_Reset(void);
static void NF_Init(void);
//*******************************************************


static U32 targetBlock;	    // Block number (0 ~ 4095)
static U32 targetSize;	    // Total byte size 
static U8 blockBuf[0x20000];   

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

#define WRITEVERIFY  (0)  //verifing is enable at writing.

/*
#define NF_CMD(cmd)	{rNFCMD=cmd;}
#define NF_RADDR(raddr)	{rNFRADDR=raddr;}	
#define NF_nFCE_L()	{rNFCONF&=~(1<<11);}
#define NF_nFCE_H()	{rNFCONF|=(1<<11);}
#define NF_RSTECC()	{rNFCONF|=(1<<12);}
#define NF_RDRDATA() 	(rNFRDATA)
#define NF_WRRDATA(rdata) {rNFRDATA=rdata;}

#define NF_WAITRB()    {while(!(rNFSTAT&(1<<0)));} 
	    //wait tWB and check F_RNB pin.   
*/

static void NF_Xm0CSn2_OUT_L(void)
{
    S6410_SetPin(Xm0CSn2_CON,HIGH);//Added to SJF2440 
	S6410_SetPin(Xm0CSn2_OUT,LOW); 
    JTAG_ShiftDRStateNoTdo(outCellValue); 
}

static void NF_Xm0CSn2_OUT_H(void)
{
    S6410_SetPin(Xm0CSn2_OUT,HIGH); 
    JTAG_ShiftDRStateNoTdo(outCellValue); 
}

static void NF_Reset(void)
{
    NF_Xm0CSn2_OUT_L();

    NF_CMD(0xFF);   //reset command

    NF_WAITRB();    //wait 200~500us;
     
    NF_Xm0CSn2_OUT_H();    
}

static void NF_Init(void)
{
    NF_Reset();

    //NF_Xm0CSn2_OUT_L();
    NF_CMD(READ_1_1);        
    //NF_Xm0CSn2_OUT_H();
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

	//Added to SJF2440 
	S6410_SetPin(Xm0RDY1_CLE_CON,HIGH);
	S6410_SetPin(Xm0RDY0_ALE_CON,HIGH);
	S6410_SetPin(Xm0CSn2_CON,HIGH); 
	S6410_SetPin(Xm0INTsm1_FREn_CON,HIGH); 
	S6410_SetPin(Xm0INTsm0_FWEn_CON,HIGH); 
	
	S6410_SetPin(Xm0RDY1_CLE_OUT,HIGH); 
	S6410_SetPin(Xm0RDY0_ALE_OUT,HIGH); 	
	NF_Init();
}


static void NF_CMD(U8 cmd)
{   
    
    //Command Latch Cycle
	S6410_ContRDataBus(HIGH); //RD[15:0]=output
    
	S6410_SetPin(Xm0CSn2_OUT,LOW); 
    S6410_SetPin(Xm0INTsm1_FREn_OUT,HIGH); 
    S6410_SetPin(Xm0INTsm0_FWEn_OUT,LOW); //Because tCLS=0, CLE & nFWE can be changed simultaneously.
    
	S6410_SetPin(Xm0RDY0_ALE_OUT,LOW); 
    S6410_SetPin(Xm0RDY1_CLE_OUT,HIGH); 
    
	S6410_SetRDataByte(cmd);
    JTAG_ShiftDRStateNoTdo(outCellValue); 

    S6410_SetPin(Xm0INTsm0_FWEn_OUT,HIGH);
    JTAG_ShiftDRStateNoTdo(outCellValue); 

#if 1
    S6410_SetPin(Xm0RDY1_CLE_OUT,LOW);	
    //Command Latch Cycle
	S6410_ContRDataBus(HIGH); //RD[15:0]=output
    JTAG_ShiftDRStateNoTdo(outCellValue); 
#endif
}


static void NF_RADDR(U8 raddr)
{
    //rNFADDR=addr;
    //Command Latch Cycle
	S6410_ContRDataBus(HIGH); //RD[15:0]=output

    S6410_SetPin(Xm0CSn2_OUT,LOW); 
    S6410_SetPin(Xm0INTsm1_FREn_OUT,HIGH); 
    S6410_SetPin(Xm0INTsm0_FWEn_OUT,LOW);
    S6410_SetPin(Xm0RDY0_ALE_OUT,HIGH);
    S6410_SetPin(Xm0RDY1_CLE_OUT,LOW);
    S6410_SetRDataByte(raddr);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
    
    S6410_SetPin(Xm0INTsm0_FWEn_OUT,HIGH);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
    
#if 1
    S6410_SetPin(Xm0RDY0_ALE_OUT,LOW);	
    S6410_ContRDataBus(LOW); //RD[15:0]=input
    JTAG_ShiftDRStateNoTdo(outCellValue); 
#endif
}


static void NF_nFCE_L(void)
{
    S6410_SetPin(Xm0CSn2_CON,HIGH);//Added to SJF2440 
	S6410_SetPin(Xm0CSn2_OUT,LOW); 
    JTAG_ShiftDRStateNoTdo(outCellValue); 
}


static void NF_nFCE_H(void)
{
    S6410_SetPin(Xm0CSn2_OUT,HIGH); 
    JTAG_ShiftDRStateNoTdo(outCellValue); 
}


static U8 NF_RDRDATA(void)
{
    S6410_ContRDataBus(LOW); //RD[15:0]=input
    S6410_SetPin(Xm0INTsm1_FREn_OUT,LOW);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
    
    S6410_SetPin(Xm0INTsm1_FREn_OUT,HIGH);
    JTAG_ShiftDRState(outCellValue,inCellValue); 
    return S6410_GetRDataByte();
}

static void NF_WRRDATA(U8 rdata)
{   
    S6410_ContRDataBus(HIGH); //RD[15:0]=output
    S6410_SetPin(Xm0INTsm0_FWEn_OUT,LOW);
    S6410_SetRDataByte(rdata);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
    
    S6410_SetPin(Xm0INTsm0_FWEn_OUT,HIGH);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
}

static U16 NF_RDDATA16(void)
{
	S6410_ContRDataBus(LOW); //RD[15:0]=input
    S6410_SetPin(Xm0INTsm1_FREn_OUT,LOW);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
    
    S6410_SetPin(Xm0INTsm1_FREn_OUT,HIGH);
    JTAG_ShiftDRState(outCellValue,inCellValue); 
    return S6410_GetRDataHW();
}

static void NF_WRRDATA16(U16 data)
{   
	S6410_ContRDataBus(HIGH); //RD[15:0]=output
    S6410_SetPin(Xm0INTsm0_FWEn_OUT,LOW);
    S6410_SetRDataHW(data);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
    
    S6410_SetPin(Xm0INTsm0_FWEn_OUT,HIGH);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
}


static void NF_WAITRB(void)
{
	while(1)
    {
		JTAG_ShiftDRState(outCellValue,inCellValue); 
	
		if( S6410_GetPin(Xm0RPn_RnB_OUT)==HIGH)
		break;
	}
}

static int NF_READY(void)
{
	JTAG_ShiftDRState(outCellValue,inCellValue); 	
	return (S6410_GetPin(Xm0RPn_RnB_OUT)==HIGH);
}


struct nandflash_controller s3c6410_nandflash_controller = 
{
	JtagInit,
	NF_nFCE_L,
	NF_nFCE_H,
	NF_READY,
	NF_CMD,
	NF_RADDR,
	NF_RDRDATA,
	NF_WRRDATA,
	NF_RDDATA16,
	NF_WRRDATA16,
	NF_WAITRB	
};

