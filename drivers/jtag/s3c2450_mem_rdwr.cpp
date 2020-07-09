#include "stdafx.h"
#ifdef WIN32
#define  _X86_     //   _X86_是Intel的CPU，还有_ALPHA_、_PPC_等，都在windows.h中有定义   
#include <windef.h>   
#include <winnt.h>   
#include <winbase.h>
#endif
#include <stdlib.h>
#include <malloc.h>
#include "common/types.h"
#include "cpu/arm/s3c2450/pin2450.h"
#include "drivers/jtag/jtag.h"
#include "drivers/ft2232/ft2232.h"
#include "mem_rdwr.h"

static int S2450_RAddr2Bank(U32 raddr);
static void MRW_JtagInit(void);
static int MRW_RdByte(U32 raddr,U8 *pRDataByte);
static int MRW_RdHW(U32 raddr,U16 *pRDataHW);
static int MRW_RdWord(U32 raddr,U32 *pRDataWord);
static int MRW_WrByte(U32 raddr,U8 rdataByte);
static int MRW_WrHW(U32 raddr,U16 rdataHW);
static int MRW_WrWord(U32 raddr,U32 rdataWord);

static U8 MRW_Rd8(U32 addr);
static U16 MRW_Rd16(U32 addr,int en_nBE,U32 bs);
static U32 MRW_Rd32(U32 addr,int en_nBE,U32 bs);
static void MRW_Wr8(U32 addr,U8 data);
static void MRW_Wr16(U32 addr,U16 data,int en_nBE,U32 bs);
static void MRW_Wr32(U32 addr,U32 data,int en_nBE,U32 bs);

static U8 MRW_Rd8Q(U32 addr);
static U16 MRW_Rd16Q(U32 addr,int en_nBE,U32 bs);
static U32 MRW_Rd32Q(U32 addr,int en_nBE,U32 bs);
static void MRW_Wr8Q(U32 addr,U8 data);
static void MRW_Wr16Q(U32 addr,U16 data,int en_nBE,U32 bs);
static void MRW_Wr32Q(U32 addr,U32 data,int en_nBE,U32 bs);


static void MRW_Menu(void)
{
    int i,j;
    char command[128];
    U32 raddr;
    U8 rdataByte;
    U16 rdataHW;
    U32 rdataWord;
    int bank;
    int error;

	jtag_opr->clear_buf();
    MRW_JtagInit();

    printf("\n[memory read/write command line]\n");
    printf("h:help x:exit\n");
    while(1)
    {
	error=0;
	printf(">");
	scanf("%s",command);
	switch(command[0])
	{
	case 'h':
	    printf("+---------------------- COMMAND LIST ------------------------+\n");
	    printf("| bs <bank #> <bw> <type> : set bank attribute               |\n");
	    printf("|                           bw=8,16,32 type:0(nWBE),1(nBE)   |\n");
	    printf("| bp                      : print bank attributes            |\n"); 
	    printf("| d <hex_raddr>            : dump 64 bytes                    |\n");
	    printf("| rb <hex_raddr>           : read, byte rdata                  |\n");
	    printf("| rh <hex_raddr>           : read, half-word rdata             |\n");
	    printf("| rw <hex_raddr>           : read, word rdata                  |\n");
   	    printf("| wb <hex_raddr> <hex_rdata>: write, byte rdata                 |\n");
	    printf("| wh <hex_raddr> <hex_rdata>: write, half-word rdata            |\n");
	    printf("| ww <hex_raddr> <hex_rdata>: write, word rdata                 |\n");
	    printf("+-------------------------- NOTE ----------------------------+\n");
	    printf("| 1. nSCS0,1 SDRAM read/write isn't supported now.           |\n");
	    printf("| 2. example: >bs 2 16 1                                     |\n");
	    printf("|             >wb 110003e0 a5                                |\n");
	    printf("+------------------------------------------------------------+\n");
	    break;
	
	case 'x':
	    return;
	
	case 'b':
	    if(command[1]=='s')
	    {
	    scanf("%d",&bank);
	    scanf("%d %d",&bankWidth[bank],&bank_nBE[bank]);
	    printf("nRCS%d= width=%d write_type=%s\n",bank,bankWidth[bank],
		(bank_nBE[bank]==0)? "nWBE":"nBE");
	    }
	    else
	    {
    		for(i=0;i<7;i++)
		{
		    printf("nRCS%d width=%d %s\n",i,bankWidth[i],
			(bank_nBE[i]==0)? "nWBE":"nBE");
		}
	    }
	    break;

	case 'd':
	    scanf("%x",&raddr);
	    raddr=raddr&0xfffffff0;
	    bank=S2450_RAddr2Bank(raddr);
	    for(i=0;i<4;i++)
	    {
		printf("%8x:",raddr);
		for(j=0;j<16;j++)
		{
		    error=MRW_RdByte(raddr++,&rdataByte);
		    if(error!=0)
			printf("?? ");
		    else
			printf("%02x ",rdataByte);
		}
		printf("\n");
	    }
	    break;

	case 'r': //read
	    switch(command[1])
	    {
	    case 'b': //read byte
		scanf("%x",&raddr);
		raddr=raddr&0xffffffff;
		bank=S2450_RAddr2Bank(raddr);
		error=MRW_RdByte(raddr,&rdataByte);
		if(error!=0)
		    printf("ERROR:incorrect bank bus width\n");    
		else
		    printf("rb: %x(%d,%s): %02x\n",raddr,bankWidth[bank],
			(bank_nBE[bank]==0)? "nWBE":"nBE",rdataByte);
		break;
	    case 'h': //read half-word
		scanf("%x",&raddr);
		raddr=raddr&0xfffffffe;
		bank=S2450_RAddr2Bank(raddr);
		error=MRW_RdHW(raddr,&rdataHW);
		if(error!=0)
		    printf("ERROR:incorrect bank bus width\n");    
		else
		    printf("rh: %x(%d,%s): %04x\n",raddr,bankWidth[bank],
			(bank_nBE[bank]==0)? "nWBE":"nBE",rdataHW);
		break;
	    case 'w': //read word
		scanf("%x",&raddr);
		raddr=raddr&0xfffffffc;
		bank=S2450_RAddr2Bank(raddr);
		error=MRW_RdWord(raddr,&rdataWord);
		if(error!=0)
		    printf("ERROR:incorrect bank bus width\n");
		else
		    printf("rw: %x(%d,%s): %08x\n",raddr,bankWidth[bank],
			(bank_nBE[bank]==0)? "nWBE":"nBE",rdataWord);
		break;
	    default:
		printf("ERROR:incorrect command.\n");
		break;
	    }
	    break;

	case 'w': //write
	    switch(command[1])
	    {
	    case 'b': //write byte
		scanf("%x %x",&raddr,&rdataByte);
		raddr=raddr&0xffffffff;
		bank=S2450_RAddr2Bank(raddr);
		error=MRW_WrByte(raddr,rdataByte);
		if(error!=0)
		    printf("ERROR:incorrect bank bus width\n");
		else
		    printf("wb: %x(%d,%s): %02x\n",raddr,bankWidth[bank],
			(bank_nBE[bank]==0)? "nWBE":"nBE",rdataByte);
		break;
	    case 'h': //write half-word
		scanf("%x %x",&raddr,&rdataHW);
		raddr=raddr&0xfffffffe;
		bank=S2450_RAddr2Bank(raddr);
		
		error=MRW_WrHW(raddr,rdataHW);
		
		if(error!=0)
		    printf("ERROR:incorrect bank bus width\n");
		else
		    printf("wh: %x(%d,%s): %04x\n",raddr,bankWidth[bank],
			(bank_nBE[bank]==0)? "nWBE":"nBE",rdataHW);
		break;
	    case 'w': //write word
		scanf("%x %x",&raddr,&rdataWord);
		raddr=raddr&0xfffffffc;
		bank=S2450_RAddr2Bank(raddr);

		error=MRW_WrWord(raddr,rdataWord);

		if(error!=0)
		    printf("ERROR:incorrect bank bus width\n");
		else
		    printf("ww: %x(%d,%s): %08x\n",raddr,bankWidth[bank],
			(bank_nBE[bank]==0)? "nWBE":"nBE",rdataWord);
		break;
	    default:
		printf("ERROR:incorrect command.\n");
		break;
	    }
	    break;
        default:
	    printf("ERROR:incorrect command.\n");
	    break;
	}
    }
}


static int MRW_RdByte(U32 raddr,U8 *pRDataByte)
//raddress should be aligned
{
    U8 rdataByte;
    int bank;
    int error=0;
		
    bank=S2450_RAddr2Bank(raddr);

    switch(bankWidth[bank])
    {
    case 8:
	rdataByte=MRW_Rd8(raddr);
	break;
    case 16:
	if((raddr%2)==0)
	    rdataByte=MRW_Rd16(raddr,bank_nBE[bank],(1<<0))&0xff;
	else
	    rdataByte=(MRW_Rd16(raddr,bank_nBE[bank],(1<<1))&0xff00)>>8;
	break;
    case 32:
	switch(raddr%4)
	{
	case 0:
	    rdataByte=MRW_Rd32(raddr,bank_nBE[bank],(1<<0))&0xff;
	    break;
	case 1:
    	    rdataByte=(MRW_Rd32(raddr,bank_nBE[bank],(1<<1))&0xff00)>>8;
	    break;
	case 2:
	    rdataByte=(MRW_Rd32(raddr,bank_nBE[bank],(1<<2))&0xff0000)>>16;
	    break;
	case 3:
    	    rdataByte=(MRW_Rd32(raddr,bank_nBE[bank],(1<<3))&0xff000000)>>24;
	    break;
	default:
	    error=1;
	    break;
	}
	break;
    default:
	error=1;
	break;
    }
    *pRDataByte=rdataByte;
    return error;
}


static int MRW_RdHW(U32 raddr,U16 *pRDataHW)
//raddress should be aligned
{
    U16 rdataHW;
    int bank;
    int error=0;
		
    bank=S2450_RAddr2Bank(raddr);
    switch(bankWidth[bank])
    {
    case 16:
	rdataHW=MRW_Rd16(raddr,bank_nBE[bank],(3<<0));
	break;
    case 32:
	switch(raddr%4)
	{
	case 0:
	    rdataHW=MRW_Rd32(raddr,bank_nBE[bank],(3<<0))&0xffff;
	    break;
	case 2:
	    rdataHW=(MRW_Rd32(raddr,bank_nBE[bank],(3<<2))&0xffff0000)>>16;
	    break;
	}
	break;
    default:
	error=1;
	break;
    }
    *pRDataHW=rdataHW;
    return error;
}


static int MRW_RdWord(U32 raddr,U32 *pRDataWord)
//raddress should be aligned
{
    U32 rdataWord;
    int bank;
    int error=0;

    bank=S2450_RAddr2Bank(raddr);
    switch(bankWidth[bank])
    {
    case 32:
	rdataWord=MRW_Rd32(raddr,bank_nBE[bank],(0xf<<0));
	break;
    default:
	error=1;
	break;
    }
    *pRDataWord=rdataWord;
    return error;
}


static int MRW_WrByte(U32 raddr,U8 rdataByte)
{
    int bank;
    int error=0;

    bank=S2450_RAddr2Bank(raddr);
    switch(bankWidth[bank])
    {
    case 8:
	MRW_Wr8(raddr,rdataByte);
	break;
    case 16:
	if((raddr%2)==0)
	    MRW_Wr16(raddr,(U16)(rdataByte|(rdataByte<<8)),bank_nBE[bank],(1<<0));
	else
	    MRW_Wr16(raddr,(U16)(rdataByte|(rdataByte<<8)),bank_nBE[bank],(1<<1));
	break;
    case 32:
	switch(raddr%4)
	{
	case 0:
	    MRW_Wr32(raddr,rdataByte|(rdataByte<<8)|(rdataByte<<16)|(rdataByte<<24),
		    bank_nBE[bank],(1<<0));
	    break;
	case 1:
	    MRW_Wr32(raddr,rdataByte|(rdataByte<<8)|(rdataByte<<16)|(rdataByte<<24),
		    bank_nBE[bank],(1<<1));
	    break;
	case 2:
	    MRW_Wr32(raddr,rdataByte|(rdataByte<<8)|(rdataByte<<16)|(rdataByte<<24),
		    bank_nBE[bank],(1<<2));
	    break;
	case 3:
	    MRW_Wr32(raddr,rdataByte|(rdataByte<<8)|(rdataByte<<16)|(rdataByte<<24),
		    bank_nBE[bank],(1<<3));
	    break;
	}
	break;
    default:
	error=1;
	break;
    }
    return error;
}


static int MRW_WrHW(U32 raddr,U16 rdataHW)
//raddress should be aligned
{
    int bank;
    int error=0;

    bank=S2450_RAddr2Bank(raddr);
    switch(bankWidth[bank])
    {
    case 16:
	MRW_Wr16(raddr,rdataHW,bank_nBE[bank],(3<<0));
	break;
    case 32:
	switch(raddr%4)
	{
	case 0:
	    MRW_Wr32(raddr,rdataHW|(rdataHW<<16),bank_nBE[bank],(3<<0));
	    break;
	case 2:
	    MRW_Wr32(raddr,rdataHW|(rdataHW<<16),bank_nBE[bank],(3<<2));
	    break;
	}
    default:
	error=1;
	break;
    }
    return error;
}


static int MRW_WrWord(U32 raddr,U32 rdataWord)
//raddress should be aligned
{
    int bank;
    int error=0;

    bank=S2450_RAddr2Bank(raddr);
    switch(bankWidth[bank])
    {
    case 32:
	MRW_Wr32(raddr,rdataWord,bank_nBE[bank],(0xf<<0));
	break;
    default:
	error=1;
	break;
    }
    return error;
}


//*************************************************
//*************************************************
//**     JTAG dependent primitive functions      **
//************************************************* 
//*************************************************
static void MRW_JtagInit(void)
{
    JTAG_RunTestldleState();
    JTAG_ShiftIRState(EXTEST);

    //The initial value of pins will be defined here.
}


static int S2450_RAddr2Bank(U32 raddr)
{
    if(raddr<0x8000000)
	return 0;
    else if(raddr<0x10000000)
	return 1;
    else if(raddr<0x18000000)
	return 2;
    else if(raddr<0x20000000)
	return 3;
    else if(raddr<0x28000000)
	return 4;
    else if(raddr<0x30000000)
	return 5;
    else if(raddr<0x38000000)
	return 6;
    else if(raddr<0x40000000)
	return 7;

    printf("ERROR:raddress range overflow\n");
    return 0;
}


static void S2450_Assert_nCS(U32 raddr)
{
    if(raddr<0x8000000)
	S2450_SetPin(nRCS0 ,LOW);
    else if(raddr<0x10000000)
	S2450_SetPin(nRCS1 ,LOW);
    else if(raddr<0x18000000)
	S2450_SetPin(nRCS2 ,LOW);
    else if(raddr<0x20000000)
	S2450_SetPin(nRCS3 ,LOW);
    else if(raddr<0x28000000)
	S2450_SetPin(nRCS4 ,LOW);
    else if(raddr<0x30000000)
	S2450_SetPin(nRCS5 ,LOW);
    else if(raddr<0x38000000)
	S2450_SetPin(nSCS0 ,LOW);
    else if(raddr<0x40000000)
	S2450_SetPin(nSCS1 ,LOW);
}


static void S2450_Deassert_nCS(U32 raddr)
{
    if(raddr<0x8000000)
	S2450_SetPin(nRCS0 ,HIGH);
    else if(raddr<0x10000000)
	S2450_SetPin(nRCS1 ,HIGH);
    else if(raddr<0x18000000)
	S2450_SetPin(nRCS2 ,HIGH);
    else if(raddr<0x20000000)
	S2450_SetPin(nRCS3 ,HIGH);
    else if(raddr<0x28000000)
	S2450_SetPin(nRCS4 ,HIGH);
    else if(raddr<0x30000000)
	S2450_SetPin(nRCS5 ,HIGH);
    else if(raddr<0x38000000)
	S2450_SetPin(nSCS0 ,HIGH);
    else if(raddr<0x40000000)
	S2450_SetPin(nSCS1 ,HIGH);
}




//***********************************************************
// Fast Version                   
// xxxQ: fast version by removing nRCS_to_nWEnOE setup time.
// xxxQQ: more fast version by omitting nRCS deassertion.
//***********************************************************
static U8 MRW_Rd8Q(U32 raddr)
{
	S2450_ContRDataBus(LOW); //high // input
	S2450_SetRAddr(raddr);
    S2450_Assert_nCS(raddr);
    S2450_SetPin(nOE,LOW); 
    JTAG_ShiftDRStateNoTdo(outCellValue); 
    
    S2450_SetPin(nOE,HIGH);
    S2450_Deassert_nCS(raddr);
    JTAG_ShiftDRState(outCellValue,inCellValue); 
    return S2450_GetRDataByte();
}


static U16 MRW_Rd16Q(U32 raddr,int en_nBE,U32 bs)
{
	S2450_ContRDataBus(LOW); //high // input
    S2450_SetRAddr(raddr);
    S2450_Assert_nCS(raddr);
    S2450_SetPin(nOE,LOW); 
    if(en_nBE)
    {
	if(bs&(1<<0))S2450_SetPin(nBE0,LOW); 
	if(bs&(1<<1))S2450_SetPin(nBE1,LOW); 
    }
    JTAG_ShiftDRStateNoTdo(outCellValue); 
    
    S2450_SetPin(nOE,HIGH);
    S2450_Deassert_nCS(raddr);
    if(en_nBE)
    {
	S2450_SetPin(nBE0,HIGH); 
	S2450_SetPin(nBE1,HIGH); 
    }
    JTAG_ShiftDRState(outCellValue,inCellValue); 
    return S2450_GetRDataHW();
}


static U32 MRW_Rd32Q(U32 raddr,int en_nBE,U32 bs)
{
	S2450_ContRDataBus(LOW); //high // input
    S2450_SetRAddr(raddr);
    S2450_Assert_nCS(raddr);
    S2450_SetPin(nOE,LOW); 
    if(en_nBE)
    {
	if(bs&(1<<0))S2450_SetPin(nBE0,LOW); 
	if(bs&(1<<1))S2450_SetPin(nBE1,LOW); 
    }
    JTAG_ShiftDRStateNoTdo(outCellValue); 
    
    S2450_SetPin(nOE,HIGH);
    S2450_Deassert_nCS(raddr);
    if(en_nBE)
    {
	S2450_SetPin(nBE0,HIGH); 
	S2450_SetPin(nBE1,HIGH); 
    }
    JTAG_ShiftDRState(outCellValue,inCellValue); 
    return S2450_GetRDataWord();
}


static void MRW_Wr8Q(U32 raddr,U8 rdata)
{
	S2450_ContRDataBus(HIGH); //low // output
    S2450_SetRAddr(raddr);
    S2450_Assert_nCS(raddr);
    S2450_SetPin(nWE,LOW); 
    S2450_SetRDataByte(rdata);
    JTAG_ShiftDRStateNoTdo(outCellValue); 

    S2450_SetPin(nWE,HIGH);
    JTAG_ShiftDRStateNoTdo(outCellValue); 

	S2450_ContRDataBus(LOW); //high // input
    S2450_Deassert_nCS(raddr);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
}


static void MRW_Wr8QQ(U32 raddr,U8 rdata)
{
	S2450_ContRDataBus(HIGH); //low  // output
    S2450_SetRAddr(raddr);
    S2450_Assert_nCS(raddr);
    S2450_SetPin(nWE,LOW); 
    S2450_SetRDataByte(rdata);
    JTAG_ShiftDRStateNoTdo(outCellValue); 

    S2450_SetPin(nWE,HIGH);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
#if 0
	S2450_ContRDataBus(LOW); //high // input
    S2450_Deassert_nCS(raddr);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
#endif
}


static void MRW_Wr16Q(U32 raddr,U16 rdata,int en_nBE,U32 bs)
{
	S2450_ContRDataBus(HIGH); //low // output
    S2450_SetRAddr(raddr);
    S2450_Assert_nCS(raddr);
    S2450_SetPin(nWE,LOW);
    if(bs&(1<<0))S2450_SetPin(nBE0,LOW); //used as nWBE/nBE
    if(bs&(1<<1))S2450_SetPin(nBE1,LOW); 
    S2450_SetRDataHW(rdata);
    JTAG_ShiftDRStateNoTdo(outCellValue); 

    S2450_SetPin(nWE,HIGH);
    if(!en_nBE)
    {
	S2450_SetPin(nBE0,HIGH); //nWBE is deasserted here.
	S2450_SetPin(nBE1,HIGH); 
    }
    JTAG_ShiftDRStateNoTdo(outCellValue); 

	S2450_ContRDataBus(LOW); //high // input
    S2450_Deassert_nCS(raddr);
    if(en_nBE)
    {
	S2450_SetPin(nBE0,HIGH); 
	S2450_SetPin(nBE1,HIGH); 
    }
    JTAG_ShiftDRStateNoTdo(outCellValue); 
}



static void MRW_Wr16QQ(U32 raddr,U16 rdata,int en_nBE,U32 bs)
// Very fast version nCS is never inactive for speed-up.
{
	S2450_ContRDataBus(HIGH); //low // output
    S2450_SetRAddr(raddr);
    S2450_Assert_nCS(raddr);
    S2450_SetPin(nWE,LOW);
    if(bs&(1<<0))S2450_SetPin(nBE0,LOW); //used as nWBE/nBE
    if(bs&(1<<1))S2450_SetPin(nBE1,LOW); 
    S2450_SetRDataHW(rdata);
    JTAG_ShiftDRStateNoTdo(outCellValue); 

    S2450_SetPin(nWE,HIGH);
    if(!en_nBE)
    {
	S2450_SetPin(nBE0,HIGH); //nWBE is deasserted here.
	S2450_SetPin(nBE1,HIGH); 
    }
    JTAG_ShiftDRStateNoTdo(outCellValue); 

#if 0
	S2450_ContRDataBus(LOW);//high  // input
    S2450_Deassert_nCS(raddr);
    if(en_nBE)
    {
	S2450_SetPin(nBE0,HIGH); 
	S2450_SetPin(nBE1,HIGH); 
    }
    JTAG_ShiftDRStateNoTdo(outCellValue); 
#endif
}


static void MRW_Wr32Q(U32 raddr,U32 rdata,int en_nBE,U32 bs)
{
	S2450_ContRDataBus(HIGH); //low // output
    S2450_SetRAddr(raddr);
    S2450_Assert_nCS(raddr);
    S2450_SetPin(nWE,LOW);
    
    if(bs&(1<<0))S2450_SetPin(nBE0,LOW); //used as nWBE/nBE
    if(bs&(1<<1))S2450_SetPin(nBE1,LOW); 
    S2450_SetRDataWord(rdata);
    JTAG_ShiftDRStateNoTdo(outCellValue); 

    S2450_SetPin(nWE,HIGH);
    if(!en_nBE)
    {
	S2450_SetPin(nBE0,HIGH); //nWBE is deasserted here.
	S2450_SetPin(nBE1,HIGH); 
    }

    JTAG_ShiftDRStateNoTdo(outCellValue); 

	S2450_ContRDataBus(LOW); //high // input
    S2450_Deassert_nCS(raddr);
    if(en_nBE)
    {
	S2450_SetPin(nBE0,HIGH); //nBE is deasserted here.
	S2450_SetPin(nBE1,HIGH); 
    }
    JTAG_ShiftDRStateNoTdo(outCellValue); 
}


static void MRW_Wr32QQ(U32 raddr,U32 rdata,int en_nBE,U32 bs)
{
	S2450_ContRDataBus(HIGH); //low // output
    S2450_SetRAddr(raddr);
    S2450_Assert_nCS(raddr);
    S2450_SetPin(nWE,LOW);
    
    if(bs&(1<<0))S2450_SetPin(nBE0,LOW); //used as nWBE/nBE
    if(bs&(1<<1))S2450_SetPin(nBE1,LOW); 
    S2450_SetRDataWord(rdata);
    JTAG_ShiftDRStateNoTdo(outCellValue); 

    S2450_SetPin(nWE,HIGH);
    if(!en_nBE)
    {
	S2450_SetPin(nBE0,HIGH); //nWBE is deasserted here.
	S2450_SetPin(nBE1,HIGH); 
    }

    JTAG_ShiftDRStateNoTdo(outCellValue); 

#if 0
	S2450_ContRDataBus(LOW); //high // input
    S2450_Deassert_nCS(raddr);
    if(en_nBE)
    {
	S2450_SetPin(nBE0,HIGH); //nBE is deasserted here.
	S2450_SetPin(nBE1,HIGH); 
    }
    JTAG_ShiftDRStateNoTdo(outCellValue); 
#endif
}


static U8 MRW_Rd8(U32 raddr)
{
	S2450_ContRDataBus(LOW); //high // input
    S2450_SetRAddr(raddr);
    
    S2450_Assert_nCS(raddr);
    JTAG_ShiftDRStateNoTdo(outCellValue); //tCOS

    S2450_SetPin(nOE,LOW); 
    JTAG_ShiftDRStateNoTdo(outCellValue); 

    S2450_SetPin(nOE,HIGH);
    S2450_Deassert_nCS(raddr);
    JTAG_ShiftDRState(outCellValue,inCellValue); 
    return S2450_GetRDataByte();
}


static U16 MRW_Rd16(U32 raddr,int en_nBE,U32 bs)
{
	S2450_ContRDataBus(LOW); //high // input
    S2450_SetRAddr(raddr);
    S2450_Assert_nCS(raddr);
    if(en_nBE)
    {
	if(bs&(1<<0))S2450_SetPin(nBE0,LOW); 
	if(bs&(1<<1))S2450_SetPin(nBE1,LOW); 
    }
    JTAG_ShiftDRStateNoTdo(outCellValue); //tCOS

    S2450_SetPin(nOE,LOW); 
    JTAG_ShiftDRStateNoTdo(outCellValue); 

    S2450_SetPin(nOE,HIGH);
    S2450_Deassert_nCS(raddr);
    if(en_nBE)
    {
	S2450_SetPin(nBE0,HIGH); 
	S2450_SetPin(nBE1,HIGH); 
    }
    JTAG_ShiftDRState(outCellValue,inCellValue); 
    return S2450_GetRDataHW();
}


static U32 MRW_Rd32(U32 raddr,int en_nBE,U32 bs)
{
	S2450_ContRDataBus(LOW); //high  // input
    S2450_SetRAddr(raddr);
    S2450_Assert_nCS(raddr);
    if(en_nBE)
    {
	if(bs&(1<<0))S2450_SetPin(nBE0,LOW); 
	if(bs&(1<<1))S2450_SetPin(nBE1,LOW); 
    }
    JTAG_ShiftDRStateNoTdo(outCellValue); //tCOS
    
    S2450_SetPin(nOE,LOW); 
    JTAG_ShiftDRStateNoTdo(outCellValue); 

    S2450_SetPin(nOE,HIGH);
    S2450_Deassert_nCS(raddr);
    if(en_nBE)
    {
	S2450_SetPin(nBE0,HIGH); 
	S2450_SetPin(nBE1,HIGH); 
    }
    JTAG_ShiftDRState(outCellValue,inCellValue); 
    return S2450_GetRDataWord();
}


static void MRW_Wr8(U32 raddr,U8 rdata)
{
	S2450_ContRDataBus(HIGH); //low // output
    S2450_SetRAddr(raddr);
    S2450_Assert_nCS(raddr);
    S2450_SetRDataByte(rdata);
    JTAG_ShiftDRStateNoTdo(outCellValue); //tCOS

    S2450_SetPin(nWE,LOW); 
    JTAG_ShiftDRStateNoTdo(outCellValue); 

    S2450_SetPin(nWE,HIGH);
    JTAG_ShiftDRStateNoTdo(outCellValue); //tCOH

	S2450_ContRDataBus(LOW);//high  // input
    S2450_Deassert_nCS(raddr);
    JTAG_ShiftDRStateNoTdo(outCellValue); 
}


static void MRW_Wr16(U32 raddr,U16 rdata,int en_nBE,U32 bs)
{
	S2450_ContRDataBus(HIGH); //low // output
    S2450_SetRAddr(raddr);
    S2450_Assert_nCS(raddr);
    if(en_nBE)
    {
	if(bs&(1<<0))S2450_SetPin(nBE0,LOW); //used as nBE
	if(bs&(1<<1))S2450_SetPin(nBE1,LOW); 
    }
    S2450_SetRDataHW(rdata);
    JTAG_ShiftDRStateNoTdo(outCellValue); //tCOS

    if(!en_nBE)
    {
	if(bs&(1<<0))S2450_SetPin(nBE0,LOW); //used as nWBE
	if(bs&(1<<1))S2450_SetPin(nBE1,LOW); //used as nWBE
    }
    S2450_SetPin(nWE,LOW);
    JTAG_ShiftDRStateNoTdo(outCellValue); 

    S2450_SetPin(nWE,HIGH);
    if(!en_nBE)
    {
	S2450_SetPin(nBE0,HIGH); //nWBE is deasserted here.
	S2450_SetPin(nBE1,HIGH); 
    }
    JTAG_ShiftDRStateNoTdo(outCellValue); //tCOH

	S2450_ContRDataBus(LOW); //high // input
    S2450_Deassert_nCS(raddr);
    if(en_nBE)
    {
	S2450_SetPin(nBE0,HIGH); 
	S2450_SetPin(nBE1,HIGH); 
    }
    JTAG_ShiftDRStateNoTdo(outCellValue); 
}


static void MRW_Wr32(U32 raddr,U32 rdata,int en_nBE,U32 bs)
{
	S2450_ContRDataBus(HIGH); //low // output
    S2450_SetRAddr(raddr);
    S2450_Assert_nCS(raddr);
    if(en_nBE)
    {
	if(bs&(1<<0))S2450_SetPin(nBE0,LOW); //used as nBE
	if(bs&(1<<1))S2450_SetPin(nBE1,LOW); 
    }
    S2450_SetRDataWord(rdata);
    JTAG_ShiftDRStateNoTdo(outCellValue); //tCOS

    if(!en_nBE)
    {
	if(bs&(1<<0))S2450_SetPin(nBE0,LOW); //used as nWBE
	if(bs&(1<<1))S2450_SetPin(nBE1,LOW); 
    }
    S2450_SetPin(nWE,LOW);
    JTAG_ShiftDRStateNoTdo(outCellValue); 

    S2450_SetPin(nWE,HIGH);
    if(!en_nBE)
    {
	S2450_SetPin(nBE0,HIGH); //nWBE is deasserted here.
	S2450_SetPin(nBE1,HIGH); 
    }
    JTAG_ShiftDRStateNoTdo(outCellValue); //tCOH

	S2450_ContRDataBus(LOW);//high // input
    S2450_Deassert_nCS(raddr);
    if(en_nBE)
    {
	S2450_SetPin(nBE0,HIGH); //nBE is deasserted here.
	S2450_SetPin(nBE1,HIGH); 
    }
    JTAG_ShiftDRStateNoTdo(outCellValue); 
}

static U16 MRW_Rd16Q_(U32 addr)
{
	return MRW_Rd16Q((U32)addr, 0, 0x3);
}

static U32 MRW_Rd32_(U32 addr)
{
	return MRW_Rd32((U32)addr, 0, 0xf);
}

static void MRW_Wr16QQ_(U32 addr, U16 value)
{
	MRW_Wr16QQ((U32)addr, value, 0, 0x3);
}

static void MRW_Wr32_(U32 addr,U32 data)
{
	MRW_Wr32((U32)addr, data, 0, 0xf);
}

struct jtag_mem_rdwr sc32450_jtag_mem_rdwr = {
	"S3C2450",
	0xeb509d,
	S2450_InitCell,
	MRW_Menu,
	MRW_JtagInit,
	MRW_Rd8,
	MRW_Rd16Q_,
	MRW_Rd32_,
	
	MRW_Wr8,
	MRW_Wr16QQ_,
	MRW_Wr32_,
};

