#include "stdafx.h"
#ifdef WIN32
#define  _X86_     //   _X86_是Intel的CPU，还有_ALPHA_、_PPC_等，都在windows.h中有定义   
#include <windef.h>   
#include <winnt.h>   
#include <winbase.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include "types.h"


/*
  * str is '1' and '0' string, convert it to val
  * return the bit number
  */
int binstr2bytes(char *str, u8 *bytes)
{
	unsigned int i = 0, j = 0, bit = 0;
	u8 val = 0;

	for (; i < strlen(str); i++)
	{
		val |= ((str[i] == '1') << bit);
		bit++;
		if (bit == 8)
		{
			bytes[j++] = val;
			val = 0;
			bit = 0;
		}
	}

	if (bit)
	{
		bytes[j] = val;
	}
	
	return i;
}



/*
  * 
  * 
  */
void bytes2binstr(u8 *bytes, char *str, int bitnum)
{
	int i, j;
	for (i = 0; i < bitnum / 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			str[i*8+j] = ((bytes[i] >> j) & 0x01) ? '1' : '0';
		}
	}

	bitnum -= (i*8);
	for (j = 0; j < bitnum; j++)
	{
		str[i*8+j] = ((bytes[i] >> j) & 0x01) ? '1' : '0';
	}
	
	str[i*8+j] = '\0';
}

/**
 * Sets @c num bits in @c _buffer, starting at the @c first bit,
 * using the bits in @c value.  This routine fast-paths writes
 * of little-endian, byte-aligned, 32-bit words.
 * @param _buffer The buffer whose bits will be set.
 * @param first The bit offset in @c _buffer to start writing (0-31).
 * @param num The number of bits from @c value to copy (1-32).
 * @param value Up to 32 bits that will be copied to _buffer.
 */
void buf_set_u32(void *_buffer,
		unsigned first, unsigned num, uint32_t value)
{
	uint8_t *buffer = (uint8_t *)_buffer;

	if ((num == 32) && (first == 0)) {
		buffer[3] = (value >> 24) & 0xff;
		buffer[2] = (value >> 16) & 0xff;
		buffer[1] = (value >> 8) & 0xff;
		buffer[0] = (value >> 0) & 0xff;
	} else {
		for (unsigned i = first; i < first + num; i++)
		{
			if (((value >> (i - first)) & 1) == 1)
				buffer[i / 8] |= 1 << (i % 8);
			else
				buffer[i / 8] &= ~(1 << (i % 8));
		}
	}
}
/**
 * Retrieves @c num bits from @c _buffer, starting at the @c first bit,
 * returning the bits in a 32-bit word.  This routine fast-paths reads
 * of little-endian, byte-aligned, 32-bit words.
 * @param _buffer The buffer whose bits will be read.
 * @param first The bit offset in @c _buffer to start reading (0-31).
 * @param num The number of bits from @c _buffer to read (1-32).
 * @returns Up to 32-bits that were read from @c _buffer.
 */
uint32_t buf_get_u32(const void *_buffer,
		unsigned first, unsigned num)
{
	uint8_t *buffer = (uint8_t *)_buffer;

	if ((num == 32) && (first == 0)) {
		return (((uint32_t)buffer[3]) << 24) |
			(((uint32_t)buffer[2]) << 16) |
			(((uint32_t)buffer[1]) << 8) |
			(((uint32_t)buffer[0]) << 0);
	} else {
		uint32_t result = 0;
		for (unsigned i = first; i < first + num; i++)
		{
			if (((buffer[i / 8] >> (i % 8)) & 1) == 1)
				result |= 1 << (i - first);
		}
		return result;
	}
}


