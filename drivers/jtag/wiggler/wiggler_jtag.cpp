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
#include "wiggler_jtag.h"
#include "ppt.h"

//No delay
#define Wiggler_JTAG_DELAY()		    //delay=0 
//#define Wiggler_JTAG_DELAY() Delay(1)	    //delay=delayLoopCount

void Wiggler_JTAG_Reset(void);
void Wiggler_JTAG_RunTestldleState( void );


void Wiggler_JTAG_RunTestldleState( void )
{
	Wiggler_JTAG_Reset(); //TMS=1, 5*TCK Snychronous Reset 

	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY(); // Why 3 times?
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); // Run-Test/Idle Status
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); // Run-Test/Idle Status
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); // Run-Test/Idle Status

}


void  Wiggler_JTAG_ShiftIRState(char *wrIR)
{
	int size;
	int i;
	int tdi;

	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_H);Wiggler_JTAG_DELAY(); 	// Select-DR-Scan 
	
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_H);Wiggler_JTAG_DELAY(); 	//Select-IR-Scan 

	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); 	//Capture-IR 

	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); 	//Shift-IR 

	size=strlen(wrIR);
	
	for( i=0;i<(size-1);i++)//i=0, 1, 2 
	{
	    tdi= (wrIR[i] ==HIGH) ? TDI_H:TDI_L;
	    Wiggler_JTAG_SET(tdi|TMS_L|TCK_L);
		Wiggler_JTAG_DELAY();
	    Wiggler_JTAG_SET(tdi|TMS_L|TCK_H);
		Wiggler_JTAG_DELAY(); 	//Shift-IR 
	}
	
	tdi=(wrIR[i] ==HIGH) ? TDI_H:TDI_L; //i=3
	Wiggler_JTAG_SET(tdi|TMS_H|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(tdi|TMS_H|TCK_H);Wiggler_JTAG_DELAY(); 	//Exit1-IR

	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_H);Wiggler_JTAG_DELAY(); 	//Update-IR

	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); 	//Run-Test/Idle
}


void  Wiggler_JTAG_ShiftDRState(char *wrDR, char *rdDR)
{
	int size;
	int i;
	int tdi;

	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_H);Wiggler_JTAG_DELAY(); 	//Select-DR-Scan 

	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); 	//Capture-DR 

	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); 	//Shift-DR 

	size=strlen(wrDR);

	for(i=0;i<(size-1);i++)
	{
	    tdi=(wrDR[i]==HIGH) ? TDI_H:TDI_L;
	    Wiggler_JTAG_SET(tdi|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	    Wiggler_JTAG_SET(tdi|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); 	//Shift-DR 
	    
		rdDR[i]=Wiggler_JTAG_GET_TDO();
	}

	tdi=(wrDR[i]==HIGH) ? TDI_H:TDI_L;	//i=S3C2410_MAX_CELL_INDEX
	Wiggler_JTAG_SET(tdi|TMS_H|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(tdi|TMS_H|TCK_H);Wiggler_JTAG_DELAY();	 //Exit1-DR
	
	rdDR[i] = Wiggler_JTAG_GET_TDO();
	
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_H);Wiggler_JTAG_DELAY(); 	//Update-DR

	//Run-Test/Idle
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); 	//Update-DR
}


void  Wiggler_JTAG_ShiftDRStateNoTdo(char *wrDR)
{
	int size;
	int i;
	int tdi;

	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_H);Wiggler_JTAG_DELAY(); 	//Select-DR-Scan 

	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); 	//Capture-DR 

	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); 	//Shift-DR 

	size=strlen(wrDR);

	for(i=0;i<(size-1);i++)
	{
	    tdi=(wrDR[i]==HIGH) ? TDI_H:TDI_L;
	    Wiggler_JTAG_SET(tdi|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	    Wiggler_JTAG_SET(tdi|TMS_L|TCK_H);Wiggler_JTAG_DELAY();   //Shift-DR 
	    //rdDR[i]=Wiggler_JTAG_GET_TDO();
	}

	tdi=(wrDR[i]==HIGH) ? TDI_H:TDI_L;	//i=S3C2410_MAX_CELL_INDEX
	Wiggler_JTAG_SET(tdi|TMS_H|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(tdi|TMS_H|TCK_H);Wiggler_JTAG_DELAY();	   //Exit1-DR
	//rdDR[i] = Wiggler_JTAG_GET_TDO();

	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_H);Wiggler_JTAG_DELAY(); 	//Update-DR

	//Run-Test/Idle
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); 	//Update-DR
}


void Wiggler_JTAG_Reset(void)
{
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_L);
	Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_H);
	Wiggler_JTAG_DELAY();

	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_L);
	Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_H);
	Wiggler_JTAG_DELAY();

	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_L);
	Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_H);
	Wiggler_JTAG_DELAY();

	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_L);
	Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_H);
	Wiggler_JTAG_DELAY();
	
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_L);
	Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_H);
	Wiggler_JTAG_DELAY();

	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_L);
	Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_H);
	Wiggler_JTAG_DELAY();
}


int Wiggler_JTAG_ReadId(void)
{
	int i;
	char id[32];
	int id32;
	
	Wiggler_JTAG_Reset(); //TDI, TMS = 1, TCK = 6clock 

	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY(); // Why 4 times?
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); // Run-Test/Idle Status
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); // Run-Test/Idle Status
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); // Run-Test/Idle Status
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); // Run-Test/Idle Status  

	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_H);Wiggler_JTAG_DELAY(); // Select-DR Scan Status
	
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_H);Wiggler_JTAG_DELAY(); // Select-IR Scan Status

	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); // Capture-IR Status

	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); // Shift-IR Status


	//S3C2410 IDCODE Instruction "1110"
	Wiggler_JTAG_SET(TDI_L|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_L|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); // '0'

	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); // '1'

	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); // '1'

	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_H);Wiggler_JTAG_DELAY(); // '1', //Exit1-IR


	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_H);Wiggler_JTAG_DELAY(); // Update_IR

	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_H);Wiggler_JTAG_DELAY(); // Select-DR-Scan

	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); //Capture-DR

	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); //Shift-DR

	// 	Read IDcode..
	for( i=0 ; i<=30 ; i++)
	{
	    Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	    Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); //Shift-DR
	    id[i]=(char)Wiggler_JTAG_GET_TDO();
	}

	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_H);Wiggler_JTAG_DELAY(); //Exit1_DR
	id[i]=(char)Wiggler_JTAG_GET_TDO();

	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_H|TCK_H);Wiggler_JTAG_DELAY(); // Update_DR

	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY(); // Why 3 times?	
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); // Run-Test/Idle
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); // Run-Test/Idle
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_L);Wiggler_JTAG_DELAY();
	Wiggler_JTAG_SET(TDI_H|TMS_L|TCK_H);Wiggler_JTAG_DELAY(); // Run-Test/Idle


	id32=0;                          
	for(i=31 ;i>=0 ;i--)
	{
	    if(id[i]==HIGH)
		id32|=(1<<i);
	}

	return id32;
}


static int dongle_delayLoopCount = 5;

void Wiggler_Wiggler_Delay(int count) // unit = 100ns
{
    
    int i,j;
    for(i=0 ; i<count ; i++)
        for(j=0;j<dongle_delayLoopCount;j++);
}

void Wiggler_clear_buf(void)
{
	return;
}
int Wiggler_exec_buf(void)
{
	return 0;
}


int Wiggler_OpenPpt(void)
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

int Wiggler_quit(void)
{
	return 0;
}

struct jtag_operations Wiggler_opr = 
{
	Wiggler_OpenPpt,
	Wiggler_JTAG_Reset,
	Wiggler_JTAG_RunTestldleState,
	Wiggler_JTAG_ShiftIRState,
	Wiggler_JTAG_ShiftDRState,
	Wiggler_JTAG_ShiftDRStateNoTdo,
	Wiggler_JTAG_ReadId,
	Wiggler_clear_buf,
	Wiggler_exec_buf,
	Wiggler_quit
};

