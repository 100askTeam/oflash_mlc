#ifndef S3C2440_MEM_RDWR_H
#define S3C2440_MEM_RDWR_H

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

extern struct jtag_mem_rdwr sc32440_jtag_mem_rdwr;

#endif //S3C2440_MEM_RDWR_H
