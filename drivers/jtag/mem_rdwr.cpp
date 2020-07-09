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
#include "mem_rdwr.h"

#define MAX_CELL_INDEX	1024	//0~471

int bankWidth[8]={16,16,16,16,16,16,16,16};
int bank_nBE[8]={0,0,1,1,1,1,1,1};
//struct jtag_mem_rdwr *all_jtag_mem_rdwr[] = {&sc32410_jtag_mem_rdwr, &sc32440_jtag_mem_rdwr, &sc32450_jtag_mem_rdwr, &sc36410_jtag_mem_rdwr, 0} ;
struct jtag_mem_rdwr *all_jtag_mem_rdwr[] = {&sc32410_jtag_mem_rdwr, &sc32440_jtag_mem_rdwr, &sc36410_jtag_mem_rdwr, 0} ;
struct jtag_mem_rdwr *jtag_mem_rdwr;

// Boundary Scan Cell In/Out Interface    
char outCellValue[MAX_CELL_INDEX+2]; //471+2 => 472(#BSC of 2440) + 1(End of String)
char inCellValue[MAX_CELL_INDEX+2]; //471+2 => 472(#BSC of 2440) + 1(End of String)

u8 outCellValueBytes[(MAX_CELL_INDEX+2+7)/8];
u8 inCellValueBytes[(MAX_CELL_INDEX+2+7)/8];

// Index Array for Data In/Out and Addr BSC      
int  dataOutCellIndex[32];
int  dataInCellIndex[32];
int  addrCellIndex[27];


