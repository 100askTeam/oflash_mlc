#include "stdafx.h"
#ifdef WIN32
#define  _X86_     //   _X86_是Intel的CPU，还有_ALPHA_、_PPC_等，都在windows.h中有定义   
#include <windef.h>   
#include <winnt.h>   
#include <winbase.h>
#endif
#include <stdlib.h>
#include <malloc.h>
#include "openjtag_jtag.h"
#include "common/types.h"
#include "drivers/ft2232/ft2232.h"
#include "common/binarybuffer.h"
#include "cpu/arm/s3c24x0/pin2440.h"
#include "drivers/jtag/jtag.h" 
//#define BUILD_FT2232_FTD2XX 1

#if BUILD_FT2232_FTD2XX == 1
#include "lib\FTD2XX.H"
#endif

#define LOG_DEBUG printf
#define LOG_WARNING printf
#define LOG_ERROR printf


/* tap_move[i][j]: tap movement command to go from state i to state j
 * 0: Test-Logic-Reset
 * 1: Run-Test/Idle
 * 2: Shift-DR
 * 3: Pause-DR
 * 4: Shift-IR
 * 5: Pause-IR
 *
 * SD->SD and SI->SI have to be caught in interface specific code
 */
u8 tap_move[6][6] =
{
/*	  TLR   RTI   SD    PD    SI    PI             */
	{0x7f, 0x00, 0x17, 0x0a, 0x1b, 0x16},	/* TLR */
	{0x7f, 0x00, 0x25, 0x05, 0x2b, 0x0b},	/* RTI */
	{0x7f, 0x31, 0x00, 0x01, 0x0f, 0x2f},	/* SD  */
	{0x7f, 0x30, 0x20, 0x17, 0x1e, 0x2f},	/* PD  */
	{0x7f, 0x31, 0x07, 0x17, 0x00, 0x01},	/* SI  */
	{0x7f, 0x30, 0x1c, 0x17, 0x20, 0x2f}	/* PI  */
};

int tap_move_map[16] = {
	0, -1, -1,  2, -1,  3, -1, -1,
	1, -1, -1,  4, -1,  5, -1, -1
};

void OpenJTAG_Reset(void)
{
	BUFFER_ADD = 0x4b;	/* command "Clock Data to TMS/CS Pin (no Read)" */	
	BUFFER_ADD = 0x6;	/* scan 7 bit */
	BUFFER_ADD = TAP_MOVE(TAP_TLR, TAP_TLR);	/* TMS data bits */
}

void OpenJTAG_RunTestldleState( void )
{
	OpenJTAG_Reset();

	BUFFER_ADD = 0x4b;	/* command "Clock Data to TMS/CS Pin (no Read)" */	
	BUFFER_ADD = 0x6;	/* scan 7 bit */
	BUFFER_ADD = TAP_MOVE(TAP_TLR, TAP_RTI);	/* TMS data bits */

}

void  OpenJTAG_ShiftIRState(char *wrIR)
{
	u8 val;
	int num = binstr2bytes(wrIR, &val);
	
	BUFFER_ADD = 0x4b;	/* command "Clock Data to TMS/CS Pin (no Read)" */	
	BUFFER_ADD = 0x6;	/* scan 7 bit */
	BUFFER_ADD = TAP_MOVE(TAP_RTI, TAP_SI);	/* TMS data bits -> Shift-IR */

	/* send S3C24x0 Instruction */
	BUFFER_ADD = 0x1b;	/* Clock Data Bits Out on -ve Clock Edge LSB First (no Read) */
	BUFFER_ADD = num - 2;		/* send 3 bit */
	BUFFER_ADD = val;
	/*second: make the device enter RTI Status */
	BUFFER_ADD = 0x4b;	/* command "Clock Data to TMS/CS Pin (no Read)" */
	BUFFER_ADD = 6;		/* scan 3 bit */
	BUFFER_ADD = ((val >> (num - 1))<<7)|TAP_MOVE(TAP_SI, TAP_RTI);		/* IDCODE's last "1" and TMS data bits : Shift-IR -> Exit1-IR -> Update-IR -> Run-Test/Idle*/
}


void  OpenJTAG_ShiftDRState(char *wrDR, char *rdDR)
{
	int num_bytes, bits_left;
	int cur_byte = 0;
	int last_bit;
	
	int retval;
	u32 bytes_written;
	u32 bytes_read;
	int scan_size;	

	
	BUFFER_ADD = 0x4b;	/* command "Clock Data to TMS/CS Pin (no Read)" */	
	BUFFER_ADD = 0x6;	/* scan 7 bit */
	BUFFER_ADD = TAP_MOVE(TAP_RTI, TAP_SD);	/* TMS data bits -> Shift-DR */

	scan_size = bits_left = binstr2bytes(wrDR, outCellValueBytes);
	num_bytes = (bits_left + 7) / 8;
//	bytes_to_read = num_bytes;
	ft2232_expect_read = ft2232_predict_scan_in(scan_size);

	/* add command for complete bytes */
	while (num_bytes > 1)
	{
		int thisrun_bytes;

		/* Clock Data Bytes In and Out LSB First */
		BUFFER_ADD = 0x39;
		/* LOG_DEBUG("added TDI bytes (io %i)", num_bytes); */
		
		thisrun_bytes = (num_bytes > 65537) ? 65536 : (num_bytes - 1);
		num_bytes -= thisrun_bytes;
		BUFFER_ADD = (thisrun_bytes - 1) & 0xff;
		BUFFER_ADD = ((thisrun_bytes - 1) >> 8) & 0xff;

		/* add complete bytes */
		while(thisrun_bytes-- > 0)
		{
			BUFFER_ADD = outCellValueBytes[cur_byte];
			cur_byte++;
			bits_left -= 8;
		}
	}
	
	/* the most signifcant bit is scanned during TAP movement */
	last_bit = (outCellValueBytes[cur_byte] >> (bits_left - 1)) & 0x1;

	/* process remaining bits but the last one */
	if (bits_left > 1)
	{
		/* Clock Data Bits In and Out LSB First */
		BUFFER_ADD = 0x3b;
		/* LOG_DEBUG("added TDI bits (io) %i", bits_left - 1); */

		BUFFER_ADD = bits_left - 2;
		BUFFER_ADD = outCellValueBytes[cur_byte];
	}

	BUFFER_ADD = 0x6b;
	BUFFER_ADD = 6;
	BUFFER_ADD = (last_bit << 7) | TAP_MOVE(TAP_SD, TAP_RTI);; /* Exit1-DR */

#if 0	
	BUFFER_ADD = 0;
	BUFFER_ADD = (last_bit << 7) | 0x1; /* Exit1-DR */

	BUFFER_ADD = 0x4b;	/* command "Clock Data to TMS/CS Pin (no Read)" */	
	BUFFER_ADD = 1;		/* scan 2 bit */
	BUFFER_ADD = 0x01;	/* Exit1-DR -> Update-DR -> Run-Test/Idle */
#endif	

	if ((retval = ft2232_write(ft2232_buffer, ft2232_buffer_size, &bytes_written)) != ERROR_OK)
	{
		LOG_ERROR("couldn't write MPSSE commands to FT2232");
		return;
	}
	ft2232_buffer_size = 0;

	if ((retval = ft2232_read(ft2232_buffer, ft2232_expect_read, &bytes_read)) != ERROR_OK)
	{
		LOG_ERROR("couldn't read from FT2232, bytes_read = %d\n", bytes_read);
//		return retval;
	}

	ft2232_expect_read = 0;
	ft2232_read_pointer = 0;	
	ft2232_read_scan(inCellValueBytes, scan_size);
	bytes2binstr(inCellValueBytes, rdDR, scan_size);
	
}

void  OpenJTAG_ShiftDRStateNoTdo(char *wrDR)
{
	int num_bytes, bits_left;
	int cur_byte = 0;
	int last_bit;
	
	BUFFER_ADD = 0x4b;	/* command "Clock Data to TMS/CS Pin (no Read)" */	
	BUFFER_ADD = 0x6;	/* scan 7 bit */
	BUFFER_ADD = TAP_MOVE(TAP_RTI, TAP_SD);	/* TMS data bits -> Shift-DR */

	bits_left = binstr2bytes(wrDR, outCellValueBytes);
	num_bytes = (bits_left + 7) / 8;
	
	/* add command for complete bytes */
	while (num_bytes > 1)
	{
		int thisrun_bytes;

		/* Clock Data Bytes Out on -ve Clock Edge LSB First (no Read) */
		BUFFER_ADD = 0x19;
		/* LOG_DEBUG("added TDI bytes (o)"); */
		
		thisrun_bytes = (num_bytes > 65537) ? 65536 : (num_bytes - 1);
		num_bytes -= thisrun_bytes;
		BUFFER_ADD = (thisrun_bytes - 1) & 0xff;
		BUFFER_ADD = ((thisrun_bytes - 1) >> 8) & 0xff;
		
		/* add complete bytes */
		while(thisrun_bytes-- > 0)
		{
			BUFFER_ADD = outCellValueBytes[cur_byte];
			cur_byte++;
			bits_left -= 8;
		}
	}
	
	/* the most signifcant bit is scanned during TAP movement */
	last_bit = (outCellValueBytes[cur_byte] >> (bits_left - 1)) & 0x1;

	/* process remaining bits but the last one */
	if (bits_left > 1)
	{
		/* Clock Data Bits Out on -ve Clock Edge LSB First (no Read) */
		BUFFER_ADD = 0x1b;
		/* LOG_DEBUG("added TDI bits (o)"); */
		BUFFER_ADD = bits_left - 2;
		BUFFER_ADD = outCellValueBytes[cur_byte];
	}

	BUFFER_ADD = 0x4b;
	BUFFER_ADD = 6;
	BUFFER_ADD = (last_bit << 7) | TAP_MOVE(TAP_SD, TAP_RTI); 
	
	if (ft2232_approach_full())
		ft2232_exec_buf();
}

int ft2232_init(void);
extern int ft2232_readID(void);
extern void ft2232_clear_buf(void);
extern int ft2232_exec_buf(void);
extern int ft2232_quit(void);

struct jtag_operations OpenJTAG_opr = 
{
	ft2232_init,
	OpenJTAG_Reset,
	OpenJTAG_RunTestldleState,
	OpenJTAG_ShiftIRState,
	OpenJTAG_ShiftDRState,
	OpenJTAG_ShiftDRStateNoTdo,
	ft2232_readID,
	ft2232_clear_buf,
	ft2232_exec_buf,
	ft2232_quit
};








