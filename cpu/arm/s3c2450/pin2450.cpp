/* - 16/05/2002 [Jaewook Cheong] : first writing for S3C2450 */

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

// Index Array for RData In/Out and RAddr BSC      
static int  rdataOutCellIndex[16];
static int  rdataInCellIndex[16];
static int  raddrCellIndex[26];

// Index Array for SData In/Out and SAddr BSC      
static int  sdataOutCellIndex[32];
static int  sdataInCellIndex[32];
static int  saddrCellIndex[16];

void S2450_InitCell(void)
{
    int i;
	
	// ROM bus addr & data cell index
	rdataOutCellIndex[0] = RDATA0_OUT;  //refer to S3C2450_jtag_buf.bsd
    rdataOutCellIndex[1] = RDATA1_OUT; 
    rdataOutCellIndex[2] = RDATA2_OUT;
    rdataOutCellIndex[3] = RDATA3_OUT;
    rdataOutCellIndex[4] = RDATA4_OUT;
    rdataOutCellIndex[5] = RDATA5_OUT;
    rdataOutCellIndex[6] = RDATA6_OUT;
    rdataOutCellIndex[7] = RDATA7_OUT;	
    rdataOutCellIndex[8] = RDATA8_OUT;
    rdataOutCellIndex[9] = RDATA9_OUT;
    rdataOutCellIndex[10] = RDATA10_OUT;
    rdataOutCellIndex[11] = RDATA11_OUT;
    rdataOutCellIndex[12] = RDATA12_OUT;
    rdataOutCellIndex[13] = RDATA13_OUT;
    rdataOutCellIndex[14] = RDATA14_OUT;
    rdataOutCellIndex[15] = RDATA15_OUT;	

    rdataInCellIndex[0] = RDATA0_IN;	//refer to S3C2450_jtag_buf.bsd 
    rdataInCellIndex[1] = RDATA1_IN;
    rdataInCellIndex[2] = RDATA2_IN;
    rdataInCellIndex[3] = RDATA3_IN;
    rdataInCellIndex[4] = RDATA4_IN;
    rdataInCellIndex[5] = RDATA5_IN;
    rdataInCellIndex[6] = RDATA6_IN;
    rdataInCellIndex[7] = RDATA7_IN;
    rdataInCellIndex[8] = RDATA8_IN;  
    rdataInCellIndex[9] = RDATA9_IN;
    rdataInCellIndex[10] = RDATA10_IN;
    rdataInCellIndex[11] = RDATA11_IN;
    rdataInCellIndex[12] = RDATA12_IN;
    rdataInCellIndex[13] = RDATA13_IN;
    rdataInCellIndex[14] = RDATA14_IN;
    rdataInCellIndex[15] = RDATA15_IN;

    raddrCellIndex[0]=RADDR0;  //refer to S3C2450_jtag_buf.bsd
    raddrCellIndex[1]=RADDR1;
    raddrCellIndex[2]=RADDR2;
    raddrCellIndex[3]=RADDR3;
    raddrCellIndex[4]=RADDR4;
    raddrCellIndex[5]=RADDR5;
    raddrCellIndex[6]=RADDR6;
    raddrCellIndex[7]=RADDR7;
    raddrCellIndex[8]=RADDR8;
    raddrCellIndex[9]=RADDR9;
    raddrCellIndex[10]=RADDR10;
    raddrCellIndex[11]=RADDR11;
    raddrCellIndex[12]=RADDR12;
    raddrCellIndex[13]=RADDR13;
    raddrCellIndex[14]=RADDR14;
    raddrCellIndex[15]=RADDR15;
    raddrCellIndex[16]=RADDR16;
    raddrCellIndex[17]=RADDR17;
    raddrCellIndex[18]=RADDR18;
    raddrCellIndex[19]=RADDR19;
    raddrCellIndex[20]=RADDR20;
    raddrCellIndex[21]=RADDR21;
    raddrCellIndex[22]=RADDR22;
    raddrCellIndex[23]=RADDR23;
    raddrCellIndex[24]=RADDR24;
    raddrCellIndex[25]=RADDR25;

	// SDRAM bus addr & data cell index
    sdataOutCellIndex[0] = SDATA0_OUT;  //refer to S3C2450_jtag_buf.bsd
    sdataOutCellIndex[1] = SDATA1_OUT; 
    sdataOutCellIndex[2] = SDATA2_OUT;
    sdataOutCellIndex[3] = SDATA3_OUT;
    sdataOutCellIndex[4] = SDATA4_OUT;
    sdataOutCellIndex[5] = SDATA5_OUT;
    sdataOutCellIndex[6] = SDATA6_OUT;
    sdataOutCellIndex[7] = SDATA7_OUT;	
    sdataOutCellIndex[8] = SDATA8_OUT;
    sdataOutCellIndex[9] = SDATA9_OUT;
    sdataOutCellIndex[10] = SDATA10_OUT;
    sdataOutCellIndex[11] = SDATA11_OUT;
    sdataOutCellIndex[12] = SDATA12_OUT;
    sdataOutCellIndex[13] = SDATA13_OUT;
    sdataOutCellIndex[14] = SDATA14_OUT;
    sdataOutCellIndex[15] = SDATA15_OUT;	
    sdataOutCellIndex[16] = SDATA16_OUT;
    sdataOutCellIndex[17] = SDATA17_OUT;
    sdataOutCellIndex[18] = SDATA18_OUT;
    sdataOutCellIndex[19] = SDATA19_OUT;
    sdataOutCellIndex[20] = SDATA20_OUT;
    sdataOutCellIndex[21] = SDATA21_OUT;
    sdataOutCellIndex[22] = SDATA22_OUT;
    sdataOutCellIndex[23] = SDATA23_OUT;	
    sdataOutCellIndex[24] = SDATA24_OUT;
    sdataOutCellIndex[25] = SDATA25_OUT;
    sdataOutCellIndex[26] = SDATA26_OUT;
    sdataOutCellIndex[27] = SDATA27_OUT;
    sdataOutCellIndex[28] = SDATA28_OUT;
    sdataOutCellIndex[29] = SDATA29_OUT;
    sdataOutCellIndex[30] = SDATA30_OUT;
    sdataOutCellIndex[31] = SDATA31_OUT;	

    sdataInCellIndex[0] = SDATA0_IN;	//refer to S3C2450_jtag_buf.bsd 
    sdataInCellIndex[1] = SDATA1_IN;
    sdataInCellIndex[2] = SDATA2_IN;
    sdataInCellIndex[3] = SDATA3_IN;
    sdataInCellIndex[4] = SDATA4_IN;
    sdataInCellIndex[5] = SDATA5_IN;
    sdataInCellIndex[6] = SDATA6_IN;
    sdataInCellIndex[7] = SDATA7_IN;
    sdataInCellIndex[8] = SDATA8_IN;  
    sdataInCellIndex[9] = SDATA9_IN;
    sdataInCellIndex[10] = SDATA10_IN;
    sdataInCellIndex[11] = SDATA11_IN;
    sdataInCellIndex[12] = SDATA12_IN;
    sdataInCellIndex[13] = SDATA13_IN;
    sdataInCellIndex[14] = SDATA14_IN;
    sdataInCellIndex[15] = SDATA15_IN;
    sdataInCellIndex[16] = SDATA16_IN;  
    sdataInCellIndex[17] = SDATA17_IN;
    sdataInCellIndex[18] = SDATA18_IN;
    sdataInCellIndex[19] = SDATA19_IN;
    sdataInCellIndex[20] = SDATA20_IN;
    sdataInCellIndex[21] = SDATA21_IN;
    sdataInCellIndex[22] = SDATA22_IN;
    sdataInCellIndex[23] = SDATA23_IN;
    sdataInCellIndex[24] = SDATA24_IN;  
    sdataInCellIndex[25] = SDATA25_IN;
    sdataInCellIndex[26] = SDATA26_IN;
    sdataInCellIndex[27] = SDATA27_IN;
    sdataInCellIndex[28] = SDATA28_IN;
    sdataInCellIndex[29] = SDATA29_IN;
    sdataInCellIndex[30] = SDATA30_IN;
    sdataInCellIndex[31] = SDATA31_IN;
  
    saddrCellIndex[0]=SADDR0;  //refer to S3C2450_jtag_buf.bsd
    saddrCellIndex[1]=SADDR1;
    saddrCellIndex[2]=SADDR2;
    saddrCellIndex[3]=SADDR3;
    saddrCellIndex[4]=SADDR4;
    saddrCellIndex[5]=SADDR5;
    saddrCellIndex[6]=SADDR6;
    saddrCellIndex[7]=SADDR7;
    saddrCellIndex[8]=SADDR8;
    saddrCellIndex[9]=SADDR9;
    saddrCellIndex[10]=SADDR10;
    saddrCellIndex[11]=SADDR11;
    saddrCellIndex[12]=SADDR12;
    saddrCellIndex[13]=SADDR13;
    saddrCellIndex[14]=SADDR14;
    saddrCellIndex[15]=SADDR15;

    //outCellValue[] must be initialized by dummy values for JTAG_ShiftDRState();
    for(i=0;i<=S2450_MAX_CELL_INDEX;i++) //From i=0 to i=691
    {
		outCellValue[i]=LOW; //for test '0' ordinary 1 //'1'
		inCellValue[i]='u';
    }

    outCellValue[S2450_MAX_CELL_INDEX+1]='\0';
    inCellValue[S2450_MAX_CELL_INDEX+1]='\0';

    jtag_opr->clear_buf();
    JTAG_RunTestldleState();
    
	JTAG_ShiftIRState(SAMPLE_PRELOAD);

	//for(i=0;i<=S2450_MAX_CELL_INDEX;i++)
	//	printf("\nBefor: inCellValue[%d]=%c", i, inCellValue[i]);

    JTAG_ShiftDRState(outCellValue,inCellValue); //inCellValue[] is initialized.

    //for(i=0;i<=S2450_MAX_CELL_INDEX;i++)
		//printf("\nAfter: inCellValue[%d]=%c", i, inCellValue[i]);

    for(i=0;i<=S2450_MAX_CELL_INDEX;i++)
    {
		outCellValue[i]=inCellValue[i];	//outCellValue[] is initialized.
    }

    //Memory control signal initialization.
	S2450_ContRDataBus(LOW); //high // RData input
	S2450_ContSDataBus(LOW); //high // SData input
	S2450_ContRAddrBus(HIGH); //low // RAddr output

    S2450_SetPin(nRCS5_CON,HIGH); //low
    S2450_SetPin(nRCS5,HIGH); 
    S2450_SetPin(nRCS4_CON,HIGH); //low
    S2450_SetPin(nRCS4,HIGH); 
    S2450_SetPin(nRCS3_CON,HIGH); //low
    S2450_SetPin(nRCS3,HIGH); 
    S2450_SetPin(nRCS2_CON,HIGH); //low
    S2450_SetPin(nRCS2,HIGH); 
    S2450_SetPin(nRCS1_CON,HIGH); //low
    S2450_SetPin(nRCS1,HIGH); 
    S2450_SetPin(nRCS0_CON,HIGH); //low
    S2450_SetPin(nRCS0,HIGH); 
    
	S2450_SetPin(nWE,HIGH); 
    S2450_SetPin(nOE,HIGH); 
    
	S2450_SetPin(nBE0,HIGH); 
    S2450_SetPin(nBE1,HIGH); 

    S2450_SetPin(nFRE,HIGH); 
    S2450_SetPin(nFWE,HIGH); 
    S2450_SetPin(nFCE,HIGH); 

	S2450_SetPin(nSCS1_CON,HIGH); //low
    S2450_SetPin(nSCS1,HIGH); 
	S2450_SetPin(nSCS0_CON,HIGH); //low
    S2450_SetPin(nSCS0,HIGH); 

}

void S2450_ContRAddrBus(U8 cmd) // RAddr[25:0] LOW=output
{
    S2450_SetPin(RADDR0_CON,cmd);
    S2450_SetPin(RADDR1_CON,cmd);
    S2450_SetPin(RADDR2_CON,cmd);
    S2450_SetPin(RADDR3_CON,cmd);
    S2450_SetPin(RADDR4_CON,cmd);
    S2450_SetPin(RADDR5_CON,cmd);
    S2450_SetPin(RADDR6_CON,cmd);
    S2450_SetPin(RADDR7_CON,cmd);
    S2450_SetPin(RADDR8_CON,cmd);
    S2450_SetPin(RADDR9_CON,cmd);
    S2450_SetPin(RADDR10_CON,cmd);
    S2450_SetPin(RADDR11_CON,cmd);
    S2450_SetPin(RADDR12_CON,cmd);
    S2450_SetPin(RADDR13_CON,cmd);
    S2450_SetPin(RADDR14_CON,cmd);
    S2450_SetPin(RADDR15_CON,cmd);
    S2450_SetPin(RADDR16_CON,cmd);
    S2450_SetPin(RADDR17_CON,cmd);
    S2450_SetPin(RADDR18_CON,cmd);
    S2450_SetPin(RADDR19_CON,cmd);
    S2450_SetPin(RADDR20_CON,cmd);
    S2450_SetPin(RADDR21_CON,cmd);
    S2450_SetPin(RADDR22_CON,cmd);
    S2450_SetPin(RADDR23_CON,cmd);
    S2450_SetPin(RADDR24_CON,cmd);
    S2450_SetPin(RADDR25_CON,cmd);
}


void S2450_ContRDataBus(U8 cmd) // RData[15:0] HIGH=input, LOW=output
{
	S2450_SetPin(RDATA0_CON ,cmd); 
    S2450_SetPin(RDATA1_CON ,cmd); 
    S2450_SetPin(RDATA2_CON ,cmd); 
    S2450_SetPin(RDATA3_CON ,cmd); 
    S2450_SetPin(RDATA4_CON ,cmd); 
    S2450_SetPin(RDATA5_CON ,cmd); 
    S2450_SetPin(RDATA6_CON ,cmd); 
    S2450_SetPin(RDATA7_CON ,cmd); 
	S2450_SetPin(RDATA8_CON ,cmd);
    S2450_SetPin(RDATA9_CON ,cmd); 
    S2450_SetPin(RDATA10_CON ,cmd); 
    S2450_SetPin(RDATA11_CON ,cmd); 
    S2450_SetPin(RDATA12_CON ,cmd); 
    S2450_SetPin(RDATA13_CON ,cmd); 
    S2450_SetPin(RDATA14_CON ,cmd); 
    S2450_SetPin(RDATA15_CON ,cmd); 
}


void S2450_ContSDataBus(U8 cmd) // SData[31:0] HIGH=input, LOW=output
{
    S2450_SetPin(SDATA0_CON ,cmd); 
    S2450_SetPin(SDATA1_CON ,cmd); 
    S2450_SetPin(SDATA2_CON ,cmd); 
    S2450_SetPin(SDATA3_CON ,cmd); 
    S2450_SetPin(SDATA4_CON ,cmd); 
    S2450_SetPin(SDATA5_CON ,cmd); 
    S2450_SetPin(SDATA6_CON ,cmd); 
    S2450_SetPin(SDATA7_CON ,cmd); 
    S2450_SetPin(SDATA8_CON ,cmd);
    S2450_SetPin(SDATA9_CON ,cmd); 
    S2450_SetPin(SDATA10_CON ,cmd); 
    S2450_SetPin(SDATA11_CON ,cmd); 
    S2450_SetPin(SDATA12_CON ,cmd); 
    S2450_SetPin(SDATA13_CON ,cmd); 
    S2450_SetPin(SDATA14_CON ,cmd); 
    S2450_SetPin(SDATA15_CON ,cmd); 
    S2450_SetPin(SDATA16_CON ,cmd); 
    S2450_SetPin(SDATA17_CON ,cmd); 
    S2450_SetPin(SDATA18_CON ,cmd); 
    S2450_SetPin(SDATA19_CON ,cmd); 
    S2450_SetPin(SDATA20_CON ,cmd); 
    S2450_SetPin(SDATA21_CON ,cmd); 
    S2450_SetPin(SDATA22_CON ,cmd); 
    S2450_SetPin(SDATA23_CON ,cmd); 
    S2450_SetPin(SDATA24_CON ,cmd); 
    S2450_SetPin(SDATA25_CON ,cmd); 
    S2450_SetPin(SDATA26_CON ,cmd); 
    S2450_SetPin(SDATA27_CON ,cmd); 
    S2450_SetPin(SDATA28_CON ,cmd); 
    S2450_SetPin(SDATA29_CON ,cmd); 
    S2450_SetPin(SDATA30_CON ,cmd); 
    S2450_SetPin(SDATA31_CON ,cmd); 
}


void S2450_SetPin(int index, char value)
{
    outCellValue[index] = value;
}


char S2450_GetPin(int index)
{
    return inCellValue[index];
}


void S2450_SetRAddr(U32 raddr)
{
    int i;

    for(i=0;i<=25;i++)
    {
    	if(raddr & (1<<i))
	    outCellValue[raddrCellIndex[i]]=HIGH;
	else
	    outCellValue[raddrCellIndex[i]]=LOW;
    }
}


void S2450_SetSAddr(U32 saddr)
{
    int i;

    for(i=0;i<=15;i++)
    {
    	if(saddr & (1<<i))
	    outCellValue[saddrCellIndex[i]]=HIGH;
	else
	    outCellValue[saddrCellIndex[i]]=LOW;
    }
}


void S2450_SetRDataByte(U8 rdata)
{
    int i;

    for(i=0;i<8;i++)
    {
    	if(rdata & (1<<i))
	    outCellValue[rdataOutCellIndex[i]]=HIGH;
	else
	    outCellValue[rdataOutCellIndex[i]]=LOW;
    }
}


void S2450_SetRDataHW(U16 rdata)
{
    int i;

    for(i=0;i<16;i++)
    {
    	if(rdata & (1<<i))
	    outCellValue[rdataOutCellIndex[i]]=HIGH;
	else
	    outCellValue[rdataOutCellIndex[i]]=LOW;
    }
}


void S2450_SetRDataWord(U32 rdata)
{
    int i;

    for(i=0;i<32;i++)
    {
    	if(rdata & (1<<i))
	    outCellValue[rdataOutCellIndex[i]]=HIGH;
	else
	    outCellValue[rdataOutCellIndex[i]]=LOW;
    }
}


U8 S2450_GetRDataByte(void)
{
    int	i;
    U8 rdata=0;

    for(i=0;i<8;i++)
    {
	if(inCellValue[rdataInCellIndex[i]]==HIGH)
	{
	    rdata = (rdata | (1<<i));
	}
    }
    return rdata;
}


U16 S2450_GetRDataHW(void)
{
    int	i;
    U16 rdata=0;

    for(i=0;i<16;i++)
    {
	if(inCellValue[rdataInCellIndex[i]]==HIGH)
	{
	    rdata = (rdata | (1<<i));
	}
    }
    return rdata;
}


U32 S2450_GetRDataWord(void)
{
    int	i;
    U32 rdata=0;

    for(i=0;i<32;i++)
    {
	if(inCellValue[rdataInCellIndex[i]]==HIGH)
	{
	    rdata = (rdata | (1<<i));
	}
    }
    return rdata;
}





