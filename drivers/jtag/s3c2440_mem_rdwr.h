#ifndef S3C2440_MEM_RDWR_H
#define S3C2440_MEM_RDWR_H

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

extern struct jtag_mem_rdwr sc32440_jtag_mem_rdwr;

#endif //S3C2440_MEM_RDWR_H
