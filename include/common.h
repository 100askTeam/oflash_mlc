#ifndef COMMON_H
#define COMMON_H

//#include "stdafx.h"
#ifdef WIN32
#define  _X86_     //   _X86_是Intel的CPU，还有_ALPHA_、_PPC_等，都在windows.h中有定义   
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
