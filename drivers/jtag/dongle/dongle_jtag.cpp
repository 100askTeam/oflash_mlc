/* - 16/05/2002 [Jaewook Cheong] : first writing for S3C2410                 */


/*****************************************************************************/
/*	    [[ JTAG PIN assignment	]]                                   */
/*---------------------------------------------------------------------------*/
/* JTAG Pin          Parallel Port Pin                                       */
/*---------------------------------------------------------------------------*/
/*   TCK---------------->DATA0   (2)                                         */
/*   TDI---------------->DATA1   (2)                                         */
/*   TMS---------------->DATA2   (2)                                         */
/*   TDO---------------->STATUS7 (11)                                        */
/*****************************************************************************/  


#include "stdafx.h"
#include <common.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <conio.h>
#endif
#include "dongle_jtag.h"
#include "ppt.h"

//No delay
#define Dongle_JTAG_DELAY()		    //delay=0 
//#define Dongle_JTAG_DELAY() Delay(1)	    //delay=delayLoopCount

void Dongle_JTAG_Reset(void);
void Dongle_JTAG_RunTestldleState( void );


void Dongle_JTAG_RunTestldleState( void )
{
	Dongle_JTAG_Reset(); //TMS=1, 5*TCK Snychronous Reset 

	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY(); // Why 3 times?
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); // Run-Test/Idle Status
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); // Run-Test/Idle Status
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); // Run-Test/Idle Status

}


void  Dongle_JTAG_ShiftIRState(char *wrIR)
{
	int size;
	int i;
	int tdi;

	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_H);Dongle_JTAG_DELAY(); 	// Select-DR-Scan 
	
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_H);Dongle_JTAG_DELAY(); 	//Select-IR-Scan 

	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); 	//Capture-IR 

	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); 	//Shift-IR 

	size=strlen(wrIR);
	
	for( i=0;i<(size-1);i++)//i=0, 1, 2 
	{
	    tdi= (wrIR[i] ==HIGH) ? TDI_H:TDI_L;
	    Dongle_JTAG_SET(tdi|TMS_L|TCK_L);
		Dongle_JTAG_DELAY();
	    Dongle_JTAG_SET(tdi|TMS_L|TCK_H);
		Dongle_JTAG_DELAY(); 	//Shift-IR 
	}
	
	tdi=(wrIR[i] ==HIGH) ? TDI_H:TDI_L; //i=3
	Dongle_JTAG_SET(tdi|TMS_H|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(tdi|TMS_H|TCK_H);Dongle_JTAG_DELAY(); 	//Exit1-IR

	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_H);Dongle_JTAG_DELAY(); 	//Update-IR

	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); 	//Run-Test/Idle
}


void  Dongle_JTAG_ShiftDRState(char *wrDR, char *rdDR)
{
	int size;
	int i;
	int tdi;

	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_H);Dongle_JTAG_DELAY(); 	//Select-DR-Scan 

	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); 	//Capture-DR 

	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); 	//Shift-DR 

	size=strlen(wrDR);

	for(i=0;i<(size-1);i++)
	{
	    tdi=(wrDR[i]==HIGH) ? TDI_H:TDI_L;
	    Dongle_JTAG_SET(tdi|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	    Dongle_JTAG_SET(tdi|TMS_L|TCK_H);Dongle_JTAG_DELAY(); 	//Shift-DR 
	    
		rdDR[i]=Dongle_JTAG_GET_TDO();
	}

	tdi=(wrDR[i]==HIGH) ? TDI_H:TDI_L;	//i=S3C2410_MAX_CELL_INDEX
	Dongle_JTAG_SET(tdi|TMS_H|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(tdi|TMS_H|TCK_H);Dongle_JTAG_DELAY();	 //Exit1-DR
	
	rdDR[i] = Dongle_JTAG_GET_TDO();
	
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_H);Dongle_JTAG_DELAY(); 	//Update-DR

	//Run-Test/Idle
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); 	//Update-DR
}


void  Dongle_JTAG_ShiftDRStateNoTdo(char *wrDR)
{
	int size;
	int i;
	int tdi;

	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_H);Dongle_JTAG_DELAY(); 	//Select-DR-Scan 

	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); 	//Capture-DR 

	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); 	//Shift-DR 

	size=strlen(wrDR);

	for(i=0;i<(size-1);i++)
	{
	    tdi=(wrDR[i]==HIGH) ? TDI_H:TDI_L;
	    Dongle_JTAG_SET(tdi|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	    Dongle_JTAG_SET(tdi|TMS_L|TCK_H);Dongle_JTAG_DELAY();   //Shift-DR 
	    //rdDR[i]=Dongle_JTAG_GET_TDO();
	}

	tdi=(wrDR[i]==HIGH) ? TDI_H:TDI_L;	//i=S3C2410_MAX_CELL_INDEX
	Dongle_JTAG_SET(tdi|TMS_H|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(tdi|TMS_H|TCK_H);Dongle_JTAG_DELAY();	   //Exit1-DR
	//rdDR[i] = Dongle_JTAG_GET_TDO();

	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_H);Dongle_JTAG_DELAY(); 	//Update-DR

	//Run-Test/Idle
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); 	//Update-DR
}


void Dongle_JTAG_Reset(void)
{
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_L);
	Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_H);
	Dongle_JTAG_DELAY();

	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_L);
	Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_H);
	Dongle_JTAG_DELAY();

	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_L);
	Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_H);
	Dongle_JTAG_DELAY();

	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_L);
	Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_H);
	Dongle_JTAG_DELAY();
	
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_L);
	Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_H);
	Dongle_JTAG_DELAY();

	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_L);
	Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_H);
	Dongle_JTAG_DELAY();
}


int Dongle_JTAG_ReadId(void)
{
	int i;
	char id[32];
	int id32;
	
	Dongle_JTAG_Reset(); //TDI, TMS = 1, TCK = 6clock 

	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY(); // Why 4 times?
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); // Run-Test/Idle Status
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); // Run-Test/Idle Status
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); // Run-Test/Idle Status
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); // Run-Test/Idle Status  

	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_H);Dongle_JTAG_DELAY(); // Select-DR Scan Status
	
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_H);Dongle_JTAG_DELAY(); // Select-IR Scan Status

	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); // Capture-IR Status

	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); // Shift-IR Status


	//S3C2410 IDCODE Instruction "1110"
	Dongle_JTAG_SET(TDI_L|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_L|TMS_L|TCK_H);Dongle_JTAG_DELAY(); // '0'

	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); // '1'

	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); // '1'

	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_H);Dongle_JTAG_DELAY(); // '1', //Exit1-IR


	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_H);Dongle_JTAG_DELAY(); // Update_IR

	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_H);Dongle_JTAG_DELAY(); // Select-DR-Scan

	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); //Capture-DR

	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); //Shift-DR

	// 	Read IDcode..
	for( i=0 ; i<=30 ; i++)
	{
	    Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	    Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); //Shift-DR
	    id[i]=(char)Dongle_JTAG_GET_TDO();
	}

	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_H);Dongle_JTAG_DELAY(); //Exit1_DR
	id[i]=(char)Dongle_JTAG_GET_TDO();

	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_H|TCK_H);Dongle_JTAG_DELAY(); // Update_DR

	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY(); // Why 3 times?	
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); // Run-Test/Idle
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); // Run-Test/Idle
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_L);Dongle_JTAG_DELAY();
	Dongle_JTAG_SET(TDI_H|TMS_L|TCK_H);Dongle_JTAG_DELAY(); // Run-Test/Idle


	id32=0;                          
	for(i=31 ;i>=0 ;i--)
	{
	    if(id[i]==HIGH)
		id32|=(1<<i);
	}

	return id32;
}


static int dongle_delayLoopCount = 5;

void Dongle_Dongle_Delay(int count) // unit = 100ns
{
    
    int i,j;
    for(i=0 ; i<count ; i++)
        for(j=0;j<dongle_delayLoopCount;j++);
}

void Dongle_clear_buf(void)
{
	return;
}
int Dongle_exec_buf(void)
{
	return 0;
}


int Dongle_OpenPpt(void)
{
	static int OpenFlag = 0;

	if (OpenFlag)	// has Open
		return 0;
	
#ifdef WIN32
    if(!InstallGiveIo())
    {
        printf("ERROR: Couldn't open giveio.sys\n");
        return -1;
    }
#endif
    validPpt = GetValidPpt();
    if(!validPpt)
    {
		printf("ERROR: Unable to find a parallel port\n");
		return -1;
    }
    SetPptCompMode();	
	OpenFlag = 1;
	return 0;
}

int Dongle_quit(void)
{
	return 0;
}

struct jtag_operations Dongle_opr = 
{
	Dongle_OpenPpt,
	Dongle_JTAG_Reset,
	Dongle_JTAG_RunTestldleState,
	Dongle_JTAG_ShiftIRState,
	Dongle_JTAG_ShiftDRState,
	Dongle_JTAG_ShiftDRStateNoTdo,
	Dongle_JTAG_ReadId,
	Dongle_clear_buf,
	Dongle_exec_buf,
	Dongle_quit
};

