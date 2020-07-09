
#include "../jtag.h"

#ifndef JTAG_H
#define JTAG_H

#define LOW 			 '0'               
#define HIGH			 '1'

// JTAG Instruction Definition for S3C2440
#define EXTEST		"0000"  //LSB...MSB
#define BYPASS		"1111"
#define IDCODE		"0111"
#define SAMPLE_PRELOAD	"1100"

#define ERROR_JTAG_INIT_FAILED			(-100)
#define ERROR_JTAG_INVALID_INTERFACE	(-101)
#define ERROR_JTAG_NOT_IMPLEMENTED		(-102)
#define ERROR_JTAG_TRST_ASSERTED		(-103)
#define ERROR_JTAG_QUEUE_FAILED			(-104)
#define ERROR_JTAG_DEVICE_ERROR			(-107)

/* general failures
 * error codes < 100
 */
#define ERROR_OK					(0)
#define ERROR_INVALID_ARGUMENTS		ERROR_COMMAND_SYNTAX_ERROR
#define ERROR_NO_CONFIG_FILE		(-2)
#define ERROR_BUF_TOO_SMALL			(-3)
/* see "Error:" log entry for meaningful message to the user. The caller should 
 * make no assumptions about what went wrong and try to handle the problem.
 */
#define ERROR_FAIL       			(-4)

/* Tap States
 * TLR - Test-Logic-Reset, RTI - Run-Test/Idle,
 * SDS - Select-DR-Scan, CD - Capture-DR, SD - Shift-DR, E1D - Exit1-DR,
 * PD - Pause-DR, E2D - Exit2-DR, UD - Update-DR,
 * SIS - Select-IR-Scan, CI - Capture-IR, SI - Shift-IR, E1I - Exit1-IR,
 * PI - Pause-IR, E2I - Exit2-IR, UI - Update-IR
 */
#if 0
enum tap_state
{
	TAP_TLR = 0x0, TAP_RTI = 0x8,
	TAP_SDS = 0x1, TAP_CD = 0x2, TAP_SD = 0x3, TAP_E1D = 0x4,
	TAP_PD = 0x5, TAP_E2D = 0x6, TAP_UD = 0x7,
	TAP_SIS = 0x9, TAP_CI = 0xa, TAP_SI = 0xb, TAP_E1I = 0xc,
	TAP_PI = 0xd, TAP_E2I = 0xe, TAP_UI = 0xf
};
#endif

#define TAP_MOVE(from, to) tap_move[tap_move_map[from]][tap_move_map[to]]

extern unsigned char tap_move[6][6];
extern int tap_move_map[];

extern struct jtag_operations OpenJTAG_opr;

void OpenJTAG_Reset(void);
void OpenJTAG_RunTestldleState( void );
void OpenJTAG_ShiftIRState(char *wrIR);
void OpenJTAG_ShiftDRState(char *wrDR, char *rdDR);
void OpenJTAG_ShiftDRStateNoTdo(char *wrDR);

#endif /* JTAG_H */

