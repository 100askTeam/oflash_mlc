/***************************************************************************
 *   Copyright (C) 2004, 2005 by Dominic Rath                              *
 *   Dominic.Rath@gmx.de                                                   *
 *                                                                         *
 *   Copyright (C) 2007,2008 Øyvind Harboe                                 *
 *   oyvind.harboe@zylin.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef TYPES_H
#define TYPES_H

// wy***********
#ifdef BUILD_FT2232_LIBFTDI
#define UINT64 unsigned long long
#else
#define UINT64 _int64
#endif

#ifndef u8
typedef unsigned char u8;
#endif

#ifndef u16
typedef unsigned short u16;
#endif

#ifndef u32
typedef unsigned int u32;
#endif

#ifndef WIN32
#ifndef u64
typedef long long u64;
#endif
#endif

#ifndef U8
typedef unsigned char U8;
#endif

#ifndef U16
typedef unsigned short U16;
#endif

#ifndef U32
typedef unsigned int U32;
#endif

/* bsd */
typedef unsigned char		u_char;
typedef unsigned short		u_short;
typedef unsigned int		u_int;
typedef unsigned long		u_long;

/* sysv */

typedef unsigned char		unchar;
typedef unsigned char		uchar;
typedef unsigned short		ushort;
typedef unsigned int		uint;
typedef unsigned long		ulong;

typedef unsigned char		__u8;
typedef unsigned short		__u16;
typedef unsigned int		__u32;

typedef		unsigned char		uint8_t;
typedef		unsigned short		uint16_t;
typedef		unsigned int		uint32_t;

#ifndef int8_t
//typedef		char				int8_t;
#endif
typedef		short				int16_t;
typedef		int					int32_t;

typedef char            CHAR;    /* 8-bit signed data    */
typedef unsigned char   BYTE;    /* 8-bit unsigned data  */
typedef unsigned short  WORD;    /* 16-bit unsigned data */
typedef long            LONG;    /* 32-bit signed data   */
typedef unsigned long   ULONG;   /* 32-bit unsigned data */
typedef unsigned long   DWORD;   /* 32-bit unsigned data */

#ifdef WIN32
typedef 	__int64				u64;
typedef		unsigned char		u_int8_t;
typedef		unsigned short		u_int16_t;
typedef		unsigned int		u_int32_t;
typedef 	unsigned int loff_t;
#endif

typedef 	unsigned int size_t;

typedef unsigned long phys_addr_t;
typedef unsigned long phys_size_t;


typedef struct jtag_tap_s jtag_tap_t;

#define cpu_to_le16(x) (x)
#define le16_to_cpu(x) (x)

#define cpu_to_le32(x) (x)
#define le32_to_cpu(x) (x)


/* DANGER!!!! here be dragons! Note that the pointer in 
 * memory might be unaligned. On some CPU's, i.e. ARM7,
 * the 2 lsb are ignored for 32 bit access, on others
 * it will cause an exception and on e.g. x86, it works
 * the same as if aligned.
 */
#define le_to_h_u32(x) ((u32)((x)[0] | (x)[1] << 8 | (x)[2] << 16 | (x)[3] << 24))
#define le_to_h_u16(x) ((u16)((x)[0] | (x)[1] << 8))
#define be_to_h_u32(x) ((u32)((x)[3] | (x)[2] << 8 | (x)[1] << 16 | (x)[0] << 24))
#define be_to_h_u16(x) ((u16)((x)[1] | (x)[0] << 8))

#define h_u32_to_le(buf, val) do {\
(buf)[3] = ((val) & 0xff000000) >> 24;\
(buf)[2] = ((val) & 0x00ff0000) >> 16;\
(buf)[1] = ((val) & 0x0000ff00) >> 8;\
(buf)[0] = ((val) & 0x000000ff);\
} while (0)
#define h_u32_to_be(buf, val) do {\
(buf)[0] = ((val) & 0xff000000) >> 24;\
(buf)[1] = ((val) & 0x00ff0000) >> 16;\
(buf)[2] = ((val) & 0x0000ff00) >> 8;\
(buf)[3] = ((val) & 0x000000ff);\
} while (0)

#define h_u16_to_le(buf, val) do {\
(buf)[1] = ((val) & 0xff00) >> 8;\
(buf)[0] = ((val) & 0x00ff) >> 0;\
} while (0)
#define h_u16_to_be(buf, val) do {\
(buf)[0] = ((val) & 0xff00) >> 8;\
(buf)[1] = ((val) & 0x00ff) >> 0;\
} while (0)




/**
 * Compute the number of elements of a variable length array.
 * <code>
 * const char *strs[] = { "a", "b", "c" };
 * unsigned num_strs = ARRAY_SIZE(strs);
 * </code>
 */
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*(x)))


/**
 * Cast a member of a structure out to the containing structure.
 * @param ptr The pointer to the member.
 * @param type The type of the container struct this is embedded in.
 * @param member The name of the member within the struct.
 *
 * This is a mechanism which is used throughout the Linux kernel.
 */
#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})


/**
 * Rounds @c m up to the nearest multiple of @c n using division.
 * @param m The value to round up to @c n.
 * @param n Round @c m up to a multiple of this number.
 * @returns The rounded integer value.
 */
#define DIV_ROUND_UP(m, n)	(((m) + (n) - 1) / (n))


/* DANGER!!!! here be dragons!
 *
 * Leave these fn's as byte accesses because it is safe
 * across architectures. Clever usage of 32 bit access
 * will create problems on some hosts.
 *
 * Note that the "buf" pointer in memory is probably unaligned.
 *
 * Were these functions to be re-written to take a 32 bit wide or 16 bit wide
 * memory access shortcut, then on some CPU's, i.e. ARM7, the 2 lsbytes of the address are
 * ignored for 32 bit access, whereas on other CPU's a 32 bit wide unaligned memory access
 * will cause an exception, and lastly on x86, an unaligned "greater than bytewide"
 * memory access works as if aligned.  So what follows below will work for all
 * platforms and gives the compiler leeway to do its own platform specific optimizations.
 *
 * Again, note that the "buf" pointer in memory is probably unaligned.
 */



#if defined(__ECOS)

/* eCos plain lacks these definition... A series of upstream patches
 * could probably repair it, but it seems like too much work to be
 * worth it.
 */

#if !defined(_STDINT_H)
#define PRIx32 "x"
#define PRId32 "d"
#define SCNx32 "x"
#define PRIi32 "i"
#define PRIu32 "u"
#define PRId8 PRId32

typedef CYG_ADDRWORD intptr_t;
typedef int64_t intmax_t;
typedef uint64_t uintmax_t;
#define INT8_MAX 0x7f
#define INT8_MIN (-INT8_MAX - 1)
# define UINT8_MAX		(255)
#define INT16_MAX 0x7fff
#define INT16_MIN (-INT16_MAX - 1)
# define UINT16_MAX		(65535)
#define INT32_MAX 0x7fffffffL
#define INT32_MIN (-INT32_MAX - 1L)
# define UINT32_MAX		(4294967295U)
#define INT64_MAX 0x7fffffffffffffffLL
#define INT64_MIN (-INT64_MAX - 1LL)
#define UINT64_MAX (__CONCAT(INT64_MAX, U) * 2ULL + 1ULL)
#endif

    #ifndef LLONG_MAX
	#define ULLONG_MAX	UINT64_C(0xFFFFFFFFFFFFFFFF)
	#define LLONG_MAX	INT64_C(0x7FFFFFFFFFFFFFFF)
	#define LLONG_MIN	ULLONG_MAX
    #endif


#define ULLONG_MAX 18446744073709551615

/* C99, eCos is C90 compliant (with bits of C99) */
#define isblank(c) ((c) == ' ' || (c) == '\t')


#endif

#endif // __TYPES_H

