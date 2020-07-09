#ifndef __PPT_H__
#define __PPT_H__

#ifndef WIN32
#include <sys/io.h>
#define _inp(a) inb(a)
#define _outp(a,d) outb(d,a)
#endif

int GetValidPpt(void);
int InstallGiveIo(void);
void SetPptCompMode(void);

extern int validPpt;

#define LPT1 0x378	// the search order is LPT1 then 2 then 3
#define LPT2 0x278	// first valid address found is used (re-order if needed for multiple ports)
#define LPT3 0x3bc	// hardware base address for parallel port

#define OutputPpt(value)    _outp((unsigned short)validPpt,value)
#define InputPpt()	    _inp((unsigned short)(validPpt+0x1))

#endif //__PPT_H__