#include "stdafx.h"
#ifdef WIN32
#define  _X86_     //   _X86_��Intel��CPU������_ALPHA_��_PPC_�ȣ�����windows.h���ж���   
#include <windef.h>   
#include <winnt.h>   
#include <winbase.h>
#endif
#include <stdlib.h>
#include <malloc.h>
#include "common/types.h"
#include "openjtag/openjtag_jtag.h"
#include "dongle/dongle_jtag.h"

struct jtag_operations *jtag_opr;

void JTAG_Reset(void)
{
	jtag_opr->JTAG_Reset();
}

void JTAG_RunTestldleState( void )
{
	jtag_opr->JTAG_RunTestldleState();
}

void  JTAG_ShiftIRState(char *wrIR)
{
	jtag_opr->JTAG_ShiftIRState(wrIR);
}

void  JTAG_ShiftDRState(char *wrDR, char *rdDR)
{
	jtag_opr->JTAG_ShiftDRState(wrDR, rdDR);
}

void  JTAG_ShiftDRStateNoTdo(char *wrDR)
{
	jtag_opr->JTAG_ShiftDRStateNoTdo(wrDR);
}

