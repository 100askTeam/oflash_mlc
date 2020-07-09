/* - 16/05/2002 [Jaewook Cheong] : first writing for S3C6410 */

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
#include "cpu/arm/s3c6410/pin6410.h"
#include "drivers/jtag/jtag.h"
#include "drivers/ft2232/ft2232.h" 


// Boundary Scan Cell In/Out Interface    

// Index Array for RData In/Out and RAddr BSC      
static int  rdataOutCellIndex[16];
static int  rdataInCellIndex[16];
//int  raddrCellIndex[26];
static int  raddrCellIndex[16];

// Index Array for SData In/Out and SAddr BSC      
static int  sdataOutCellIndex[32];
static int  sdataInCellIndex[32];
static int  saddrCellIndex[16];

  
void S6410_InitCell(void)
{
	int i;

	// ROM bus addr & data cell index
	rdataOutCellIndex[0] = Xm0DATA0_OUT;  //refer to S3C6410_070430.bsdl
	rdataOutCellIndex[1] = Xm0DATA1_OUT; 
	rdataOutCellIndex[2] = Xm0DATA2_OUT;
	rdataOutCellIndex[3] = Xm0DATA3_OUT;
	rdataOutCellIndex[4] = Xm0DATA4_OUT;
	rdataOutCellIndex[5] = Xm0DATA5_OUT;
	rdataOutCellIndex[6] = Xm0DATA6_OUT;
	rdataOutCellIndex[7] = Xm0DATA7_OUT;	
	rdataOutCellIndex[8] = Xm0DATA8_OUT;
	rdataOutCellIndex[9] = Xm0DATA9_OUT;
	rdataOutCellIndex[10] = Xm0DATA10_OUT;
	rdataOutCellIndex[11] = Xm0DATA11_OUT;
	rdataOutCellIndex[12] = Xm0DATA12_OUT;
	rdataOutCellIndex[13] = Xm0DATA13_OUT;
	rdataOutCellIndex[14] = Xm0DATA14_OUT;
	rdataOutCellIndex[15] = Xm0DATA15_OUT;	

	rdataInCellIndex[0] = Xm0DATA0_IN;	//refer to S3C6410_070430.bsdl 
	rdataInCellIndex[1] = Xm0DATA1_IN;
	rdataInCellIndex[2] = Xm0DATA2_IN;
	rdataInCellIndex[3] = Xm0DATA3_IN;
	rdataInCellIndex[4] = Xm0DATA4_IN;
	rdataInCellIndex[5] = Xm0DATA5_IN;
	rdataInCellIndex[6] = Xm0DATA6_IN;
	rdataInCellIndex[7] = Xm0DATA7_IN;
	rdataInCellIndex[8] = Xm0DATA8_IN;  
	rdataInCellIndex[9] = Xm0DATA9_IN;
	rdataInCellIndex[10] = Xm0DATA10_IN;
	rdataInCellIndex[11] = Xm0DATA11_IN;
	rdataInCellIndex[12] = Xm0DATA12_IN;
	rdataInCellIndex[13] = Xm0DATA13_IN;
	rdataInCellIndex[14] = Xm0DATA14_IN;
	rdataInCellIndex[15] = Xm0DATA15_IN;

	raddrCellIndex[0]=Xm0ADDR0_OUT;  //refer to S3C6410_070430.bsdl
	raddrCellIndex[1]=Xm0ADDR1_OUT;
	raddrCellIndex[2]=Xm0ADDR2_OUT;
	raddrCellIndex[3]=Xm0ADDR3_OUT;
	raddrCellIndex[4]=Xm0ADDR4_OUT;
	raddrCellIndex[5]=Xm0ADDR5_OUT;
	raddrCellIndex[6]=Xm0ADDR6_OUT;
	raddrCellIndex[7]=Xm0ADDR7_OUT;
	raddrCellIndex[8]=Xm0ADDR8_OUT;
	raddrCellIndex[9]=Xm0ADDR9_OUT;
	raddrCellIndex[10]=Xm0ADDR10_OUT;
	raddrCellIndex[11]=Xm0ADDR11_OUT;
	raddrCellIndex[12]=Xm0ADDR12_OUT;
	raddrCellIndex[13]=Xm0ADDR13_OUT;
	raddrCellIndex[14]=Xm0ADDR14_OUT;
	raddrCellIndex[15]=Xm0ADDR15_OUT;

	//outCellValue[] must be initialized by dummy values for JTAG_ShiftDRState();
	for(i=0;i<=S6410_MAX_CELL_INDEX;i++) //From i=0 to i=818
	{
		outCellValue[i]=LOW;
		inCellValue[i]='u';
	}

	outCellValue[S6410_MAX_CELL_INDEX+1]='\0';
	inCellValue[S6410_MAX_CELL_INDEX+1]='\0';

    jtag_opr->clear_buf();
	JTAG_RunTestldleState();
	jtag_opr->exec_buf();

	JTAG_ShiftIRState(SAMPLE_PRELOAD);

	//for(i=0;i<=S6410_MAX_CELL_INDEX;i++)
	//	printf("\nBefor: inCellValue[%d]=%c", i, inCellValue[i]);

	JTAG_ShiftDRState(outCellValue,inCellValue); //inCellValue[] is initialized.

	//for(i=0;i<=S6410_MAX_CELL_INDEX;i++)
	//printf("\nAfter: inCellValue[%d]=%c", i, inCellValue[i]);

	for(i=0;i<=S6410_MAX_CELL_INDEX;i++)
	{
		outCellValue[i]=inCellValue[i];	//outCellValue[] is initialized.
	}

	/////////////////////////////////////////////
	//S3C6410(65nm) Pad Control : 0(Input), 1(Output)
	//90nm Pad Control : 0(Output), 1(Input)
	/////////////////////////////////////////////
	
	//Memory control signal initialization.
	S6410_ContRDataBus(LOW); // RData input
	//	S6410_ContSDataBus(HIGH); // SData input
	S6410_ContRAddrBus(HIGH); // RAddr output

	S6410_SetPin(Xm0CSn5_CON,HIGH); 
	S6410_SetPin(Xm0CSn5_OUT,HIGH); 
	S6410_SetPin(Xm0CSn4_CON,HIGH); 
	S6410_SetPin(Xm0CSn4_OUT,HIGH); 
	S6410_SetPin(Xm0CSn3_CON,HIGH); 
	S6410_SetPin(Xm0CSn3_OUT,HIGH); 
	S6410_SetPin(Xm0CSn2_CON,HIGH); 
	S6410_SetPin(Xm0CSn2_OUT,HIGH); 
	S6410_SetPin(Xm0CSn1_CON,HIGH); 
	S6410_SetPin(Xm0CSn1_OUT,HIGH); 
	S6410_SetPin(Xm0CSn0_CON,HIGH);
	S6410_SetPin(Xm0CSn0_OUT,HIGH); 
    
	S6410_SetPin(Xm0WEn,HIGH); 
	S6410_SetPin(Xm0OEn,HIGH); 

	S6410_SetPin(Xm0DQM0_nBE0,HIGH); 
	S6410_SetPin(Xm0DQM1_nBE1,HIGH); 

	S6410_SetPin(Xm0INTsm1_FREn_OUT,HIGH); 
	S6410_SetPin(Xm0INTsm0_FWEn_OUT,HIGH); 

	// OneNand.....rb1004
	S6410_SetPin(Xm0ADRVALIDn_CON,HIGH); 
	S6410_SetPin(Xm0ADRVALIDn_OUT,HIGH); 

	//RSTOUT pad control is internally inverted in 6410...rb1004
	S6410_SetPin(XnRSTOUT_CON,LOW);
	
}

// 65nm : Xm0DATA[15:0] HIGH=output, LOW=input
void S6410_ContRAddrBus(U8 cmd) 
{
    S6410_SetPin(Xm0ADDR0_CON,cmd);
    S6410_SetPin(Xm0ADDR1_CON,cmd);
    S6410_SetPin(Xm0ADDR2_CON,cmd);
    S6410_SetPin(Xm0ADDR3_CON,cmd);
    S6410_SetPin(Xm0ADDR4_CON,cmd);
    S6410_SetPin(Xm0ADDR5_CON,cmd);
    S6410_SetPin(Xm0ADDR6_CON,cmd);
    S6410_SetPin(Xm0ADDR7_CON,cmd);
    S6410_SetPin(Xm0ADDR8_CON,cmd);
    S6410_SetPin(Xm0ADDR9_CON,cmd);
    S6410_SetPin(Xm0ADDR10_CON,cmd);
    S6410_SetPin(Xm0ADDR11_CON,cmd);
    S6410_SetPin(Xm0ADDR12_CON,cmd);
    S6410_SetPin(Xm0ADDR13_CON,cmd);
    S6410_SetPin(Xm0ADDR14_CON,cmd);
    S6410_SetPin(Xm0ADDR15_CON,cmd);
}

// 65nm : Xm0DATA[15:0] HIGH=output, LOW=input
void S6410_ContRDataBus(U8 cmd) 
{
	S6410_SetPin(Xm0DATA0_CON ,cmd); 
    S6410_SetPin(Xm0DATA1_CON ,cmd); 
    S6410_SetPin(Xm0DATA2_CON ,cmd); 
    S6410_SetPin(Xm0DATA3_CON ,cmd); 
    S6410_SetPin(Xm0DATA4_CON ,cmd); 
    S6410_SetPin(Xm0DATA5_CON ,cmd); 
    S6410_SetPin(Xm0DATA6_CON ,cmd); 
    S6410_SetPin(Xm0DATA7_CON ,cmd); 
	S6410_SetPin(Xm0DATA8_CON ,cmd);
    S6410_SetPin(Xm0DATA9_CON ,cmd); 
    S6410_SetPin(Xm0DATA10_CON ,cmd); 
    S6410_SetPin(Xm0DATA11_CON ,cmd); 
    S6410_SetPin(Xm0DATA12_CON ,cmd); 
    S6410_SetPin(Xm0DATA13_CON ,cmd); 
    S6410_SetPin(Xm0DATA14_CON ,cmd); 
    S6410_SetPin(Xm0DATA15_CON ,cmd); 
}

/*
void S6410_ContSDataBus(U8 cmd) // SData[31:0] HIGH=input, LOW=output
{
    S6410_SetPin(SDATA0_CON ,cmd); 
    S6410_SetPin(SDATA1_CON ,cmd); 
    S6410_SetPin(SDATA2_CON ,cmd); 
    S6410_SetPin(SDATA3_CON ,cmd); 
    S6410_SetPin(SDATA4_CON ,cmd); 
    S6410_SetPin(SDATA5_CON ,cmd); 
    S6410_SetPin(SDATA6_CON ,cmd); 
    S6410_SetPin(SDATA7_CON ,cmd); 
    S6410_SetPin(SDATA8_CON ,cmd);
    S6410_SetPin(SDATA9_CON ,cmd); 
    S6410_SetPin(SDATA10_CON ,cmd); 
    S6410_SetPin(SDATA11_CON ,cmd); 
    S6410_SetPin(SDATA12_CON ,cmd); 
    S6410_SetPin(SDATA13_CON ,cmd); 
    S6410_SetPin(SDATA14_CON ,cmd); 
    S6410_SetPin(SDATA15_CON ,cmd); 
    S6410_SetPin(SDATA16_CON ,cmd); 
    S6410_SetPin(SDATA17_CON ,cmd); 
    S6410_SetPin(SDATA18_CON ,cmd); 
    S6410_SetPin(SDATA19_CON ,cmd); 
    S6410_SetPin(SDATA20_CON ,cmd); 
    S6410_SetPin(SDATA21_CON ,cmd); 
    S6410_SetPin(SDATA22_CON ,cmd); 
    S6410_SetPin(SDATA23_CON ,cmd); 
    S6410_SetPin(SDATA24_CON ,cmd); 
    S6410_SetPin(SDATA25_CON ,cmd); 
    S6410_SetPin(SDATA26_CON ,cmd); 
    S6410_SetPin(SDATA27_CON ,cmd); 
    S6410_SetPin(SDATA28_CON ,cmd); 
    S6410_SetPin(SDATA29_CON ,cmd); 
    S6410_SetPin(SDATA30_CON ,cmd); 
    S6410_SetPin(SDATA31_CON ,cmd); 
}
*/

void S6410_SetPin(int index, char value)
{
    outCellValue[index] = value;
}


char S6410_GetPin(int index)
{
    return inCellValue[index];
}


void S6410_SetRAddr(U32 raddr)
{
    int i;

    //for(i=0;i<=25;i++)
    for(i=0;i<=15;i++)
    {
    	if(raddr & (1<<i))
	    	outCellValue[raddrCellIndex[i]]=HIGH;
		else
	    	outCellValue[raddrCellIndex[i]]=LOW;
    }
}


void S6410_SetSAddr(U32 saddr)
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


void S6410_SetRDataByte(U8 rdata)
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


void S6410_SetRDataHW(U16 rdata)
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


void S6410_SetRDataWord(U32 rdata)
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


U8 S6410_GetRDataByte(void)
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


U16 S6410_GetRDataHW(void)
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


U32 S6410_GetRDataWord(void)
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





