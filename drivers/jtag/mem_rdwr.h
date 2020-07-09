#ifndef MEM_RDWR_H
#define MEM_RDWR_H

#include "s3c2410_mem_rdwr.h"
#include "s3c2440_mem_rdwr.h"
#include "s3c2450_mem_rdwr.h"
#include "s3c6410_mem_rdwr.h"

extern int bankWidth[8];
extern int bank_nBE[8];
extern struct jtag_mem_rdwr *all_jtag_mem_rdwr[];
extern struct jtag_mem_rdwr *jtag_mem_rdwr;

extern char outCellValue[];
extern char inCellValue[];

extern u8 outCellValueBytes[];
extern u8 inCellValueBytes[];

extern int  dataOutCellIndex[];
extern int  dataInCellIndex[];
extern int  addrCellIndex[];


#define 	__raw_readb(addr)			jtag_mem_rdwr->MRW_Rd8((U32)addr)
#define 	__raw_readw(addr)			jtag_mem_rdwr->MRW_Rd16((U32)addr)
#define 	__raw_readl(addr)			jtag_mem_rdwr->MRW_Rd32((U32)addr)

#define 	__raw_writeb(value, addr)	jtag_mem_rdwr->MRW_Wr8((U32)addr, value)
#define 	__raw_writew(value, addr)	jtag_mem_rdwr->MRW_Wr16((U32)addr, value)
#define 	__raw_writel(value, addr)	jtag_mem_rdwr->MRW_Wr32((U32)addr, value)


struct jtag_mem_rdwr {
	char *CPUName;
	U32 CPUID;
	void (*InitCell) (void);
	void (*MRW_Menu) (void);
	void (*MRW_JtagInit) (void);

	U8 (*MRW_Rd8) (U32 addr);
	U16 (*MRW_Rd16) (U32 addr);
	U32 (*MRW_Rd32) (U32 addr);

	void (*MRW_Wr8) (U32 addr,U8 data);
	void (*MRW_Wr16) (U32 addr,U16 data);
	void (*MRW_Wr32) (U32 addr,U32 data);
};

#endif /* MEM_RDWR_H */

