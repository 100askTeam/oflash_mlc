/* - 16/05/2002 [Jaewook Cheong] : first writing for S3C2410 */

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
#include "cpu/arm/s3c24x0/pin2440.h"
#include "drivers/jtag/jtag.h"
#include "drivers/ft2232/ft2232.h"

void S2440_InitCell(void)
{
    int i;
	
	dataOutCellIndex[0] = DATA0_OUT;  //refer to S3C2440_jtag_buf.bsd
    dataOutCellIndex[1] = DATA1_OUT; 
    dataOutCellIndex[2] = DATA2_OUT;
    dataOutCellIndex[3] = DATA3_OUT;
    dataOutCellIndex[4] = DATA4_OUT;
    dataOutCellIndex[5] = DATA5_OUT;
    dataOutCellIndex[6] = DATA6_OUT;
    dataOutCellIndex[7] = DATA7_OUT;	
    dataOutCellIndex[8] = DATA8_OUT;
    dataOutCellIndex[9] = DATA9_OUT;
    dataOutCellIndex[10] = DATA10_OUT;
    dataOutCellIndex[11] = DATA11_OUT;
    dataOutCellIndex[12] = DATA12_OUT;
    dataOutCellIndex[13] = DATA13_OUT;
    dataOutCellIndex[14] = DATA14_OUT;
    dataOutCellIndex[15] = DATA15_OUT;	
    dataOutCellIndex[16] = DATA16_OUT;
    dataOutCellIndex[17] = DATA17_OUT;
    dataOutCellIndex[18] = DATA18_OUT;
    dataOutCellIndex[19] = DATA19_OUT;
    dataOutCellIndex[20] = DATA20_OUT;
    dataOutCellIndex[21] = DATA21_OUT;
    dataOutCellIndex[22] = DATA22_OUT;
    dataOutCellIndex[23] = DATA23_OUT;	
    dataOutCellIndex[24] = DATA24_OUT;
    dataOutCellIndex[25] = DATA25_OUT;
    dataOutCellIndex[26] = DATA26_OUT;
    dataOutCellIndex[27] = DATA27_OUT;
    dataOutCellIndex[28] = DATA28_OUT;
    dataOutCellIndex[29] = DATA29_OUT;
    dataOutCellIndex[30] = DATA30_OUT;
    dataOutCellIndex[31] = DATA31_OUT;	

    dataInCellIndex[0] = DATA0_IN;	//refer to S3C2440_jtag_buf.bsd 
    dataInCellIndex[1] = DATA1_IN;
    dataInCellIndex[2] = DATA2_IN;
    dataInCellIndex[3] = DATA3_IN;
    dataInCellIndex[4] = DATA4_IN;
    dataInCellIndex[5] = DATA5_IN;
    dataInCellIndex[6] = DATA6_IN;
    dataInCellIndex[7] = DATA7_IN;
    dataInCellIndex[8] = DATA8_IN;  
    dataInCellIndex[9] = DATA9_IN;
    dataInCellIndex[10] = DATA10_IN;
    dataInCellIndex[11] = DATA11_IN;
    dataInCellIndex[12] = DATA12_IN;
    dataInCellIndex[13] = DATA13_IN;
    dataInCellIndex[14] = DATA14_IN;
    dataInCellIndex[15] = DATA15_IN;
    dataInCellIndex[16] = DATA16_IN;  
    dataInCellIndex[17] = DATA17_IN;
    dataInCellIndex[18] = DATA18_IN;
    dataInCellIndex[19] = DATA19_IN;
    dataInCellIndex[20] = DATA20_IN;
    dataInCellIndex[21] = DATA21_IN;
    dataInCellIndex[22] = DATA22_IN;
    dataInCellIndex[23] = DATA23_IN;
    dataInCellIndex[24] = DATA24_IN;  
    dataInCellIndex[25] = DATA25_IN;
    dataInCellIndex[26] = DATA26_IN;
    dataInCellIndex[27] = DATA27_IN;
    dataInCellIndex[28] = DATA28_IN;
    dataInCellIndex[29] = DATA29_IN;
    dataInCellIndex[30] = DATA30_IN;
    dataInCellIndex[31] = DATA31_IN;
    
    addrCellIndex[0]=ADDR0;  //refer to S3C2440_jtag_buf.bsd
    addrCellIndex[1]=ADDR1;
    addrCellIndex[2]=ADDR2;
    addrCellIndex[3]=ADDR3;
    addrCellIndex[4]=ADDR4;
    addrCellIndex[5]=ADDR5;
    addrCellIndex[6]=ADDR6;
    addrCellIndex[7]=ADDR7;
    addrCellIndex[8]=ADDR8;
    addrCellIndex[9]=ADDR9;
    addrCellIndex[10]=ADDR10;
    addrCellIndex[11]=ADDR11;
    addrCellIndex[12]=ADDR12;
    addrCellIndex[13]=ADDR13;
    addrCellIndex[14]=ADDR14;
    addrCellIndex[15]=ADDR15;
    addrCellIndex[16]=ADDR16;
    addrCellIndex[17]=ADDR17;
    addrCellIndex[18]=ADDR18;
    addrCellIndex[19]=ADDR19;
    addrCellIndex[20]=ADDR20;
    addrCellIndex[21]=ADDR21;
    addrCellIndex[22]=ADDR22;
    addrCellIndex[23]=ADDR23;
    addrCellIndex[24]=ADDR24;
    addrCellIndex[25]=ADDR25;
    addrCellIndex[26]=ADDR26;

    //outCellValue[] must be initialized by dummy values for JTAG_ShiftDRState();
    for(i=0;i<=S2440_MAX_CELL_INDEX;i++) //From i=0 to i=426 
    {
		outCellValue[i]=HIGH; //'1'
		inCellValue[i]='u';
    }

    outCellValue[S2440_MAX_CELL_INDEX+1]='\0';
    inCellValue[S2440_MAX_CELL_INDEX+1]='\0';

	jtag_opr->clear_buf();
    JTAG_RunTestldleState();
	JTAG_ShiftIRState(SAMPLE_PRELOAD);
    JTAG_ShiftDRState(outCellValue,inCellValue); //inCellValue[] is initialized.

    for(i=0;i<=S2440_MAX_CELL_INDEX;i++)
    {
		outCellValue[i]=inCellValue[i];	//outCellValue[] is initialized.
    }

    //Memory control signal initialization.
    S2440_SetPin(DATA0_7_CON,HIGH);	//HIGH=input, LOW=output
    S2440_SetPin(DATA8_15_CON,HIGH); 
    S2440_SetPin(DATA16_23_CON,HIGH); 
    S2440_SetPin(DATA24_31_CON,HIGH); 
    
    S2440_SetPin(ADDR0_CON,LOW); 
    S2440_SetPin(ADDR1_15_CON,LOW);
    S2440_SetPin(ADDR16_CON,LOW);
    S2440_SetPin(ADDR17_CON,LOW);
    S2440_SetPin(ADDR18_CON,LOW);
    S2440_SetPin(ADDR19_CON,LOW);
    S2440_SetPin(ADDR20_CON,LOW);
    S2440_SetPin(ADDR21_CON,LOW);
    S2440_SetPin(ADDR22_CON,LOW);
    S2440_SetPin(ADDR23_CON,LOW);
    S2440_SetPin(ADDR24_CON,LOW);
    S2440_SetPin(ADDR25_CON,LOW);
    S2440_SetPin(ADDR26_CON,LOW);
    
	S2440_SetPin(nGCS67_CON,LOW); 
    S2440_SetPin(nGCS7,HIGH); 
    S2440_SetPin(nGCS6,HIGH); 
    S2440_SetPin(nGCS5_CON,LOW); 
    S2440_SetPin(nGCS5,HIGH); 
    S2440_SetPin(nGCS4_CON,LOW); 
    S2440_SetPin(nGCS4,HIGH); 
    S2440_SetPin(nGCS3_CON,LOW); 
    S2440_SetPin(nGCS3,HIGH); 
    S2440_SetPin(nGCS2_CON,LOW); 
    S2440_SetPin(nGCS2,HIGH); 
    S2440_SetPin(nGCS1_CON,LOW); 
    S2440_SetPin(nGCS1,HIGH); 
    S2440_SetPin(nGCS0_ETC_CON,LOW);
    S2440_SetPin(nGCS0,HIGH); 
    
	S2440_SetPin(nWE,HIGH); 
    S2440_SetPin(nOE,HIGH); 
    
	S2440_SetPin(nBE0,HIGH); 
    S2440_SetPin(nBE1,HIGH); 
    S2440_SetPin(nBE2,HIGH); 
    S2440_SetPin(nBE3,HIGH); 

    S2440_SetPin(nFRE,HIGH); 
    S2440_SetPin(nFWE,HIGH); 
    S2440_SetPin(nFCE,HIGH); 
}


void S2440_SetPin(int index, char value)
{
    outCellValue[index] = value;
}


char S2440_GetPin(int index)
{
    return inCellValue[index];
}


void S2440_SetAddr(u32 addr)
{
    int i;

    for(i=0;i<=26;i++)
    {
    	if(addr & (1<<i))
	    outCellValue[addrCellIndex[i]]=HIGH;
	else
	    outCellValue[addrCellIndex[i]]=LOW;
    }
}


void S2440_SetDataByte(u8 data)
{
    int i;

    for(i=0;i<8;i++)
    {
    	if(data & (1<<i))
	    outCellValue[dataOutCellIndex[i]]=HIGH;
	else
	    outCellValue[dataOutCellIndex[i]]=LOW;
    }
}


void S2440_SetDataHW(u16 data)
{
    int i;

    for(i=0;i<16;i++)
    {
    	if(data & (1<<i))
	    outCellValue[dataOutCellIndex[i]]=HIGH;
	else
	    outCellValue[dataOutCellIndex[i]]=LOW;
    }
}


void S2440_SetDataWord(u32 data)
{
    int i;

    for(i=0;i<32;i++)
    {
    	if(data & (1<<i))
	    outCellValue[dataOutCellIndex[i]]=HIGH;
	else
	    outCellValue[dataOutCellIndex[i]]=LOW;
    }
}


u8 S2440_GetDataByte(void)
{
    int	i;
    u8 data=0;

    for(i=0;i<8;i++)
    {
	if(inCellValue[dataInCellIndex[i]]==HIGH)
	{
	    data = (data | (1<<i));
	}
    }
    return data;
}


u16 S2440_GetDataHW(void)
{
    int	i;
    u16 data=0;

    for(i=0;i<16;i++)
    {
	if(inCellValue[dataInCellIndex[i]]==HIGH)
	{
	    data = (data | (1<<i));
	}
    }
    return data;
}


u32 S2440_GetDataWord(void)
{
    int	i;
    u32 data=0;

    for(i=0;i<32;i++)
    {
	if(inCellValue[dataInCellIndex[i]]==HIGH)
	{
	    data = (data | (1<<i));
	}
    }
    return data;
}

