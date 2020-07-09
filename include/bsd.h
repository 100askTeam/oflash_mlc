
#ifndef BSD_H
#define BSD_H

#include "common/types.h"

struct cpu_bsd {
	void (*Bsd_InitCell) (void);
	void (*Bsd_SetPin) (int index, char value);
	char (*Bsd_GetPin) (int index);
	
	void (*Bsd_SetAddr) (u32 addr);
	
	void (*Bsd_SetDataByte) (u8);
	void (*Bsd_SetDataHW) (u16);
	void (*Bsd_SetDataWord) (u32);
	
	u8 (*Bsd_GetDataByte) (void);
	u16 (*Bsd_GetDataHW) (void);
	u32 (*Bsd_GetDataWord) (void);	
};

#endif /* BSD_H */

