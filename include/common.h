#ifndef COMMON_H
#define COMMON_H

//#include "stdafx.h"
#ifdef WIN32
#define  _X86_     //   _X86_��Intel��CPU������_ALPHA_��_PPC_�ȣ�����windows.h���ж���   
#include <windef.h>   
#include <winnt.h>   
#include <winbase.h>
#endif
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include "common/types.h"
#include "errno.h"
#include <configs/100ask24x0.h>

#endif /* COMMON_H */
