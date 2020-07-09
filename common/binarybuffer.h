#ifndef BINARYBUFFER_H
#define BINARYBUFFER_H

/*
  * str is '1' and '0' string, convert it to val
  * return the bit number
  */
int binstr2bytes(char *str, u8 *bytes);
void bytes2binstr(u8 *bytes, char *str, int bitnum);
void buf_set_u32(void *_buffer, unsigned first, unsigned num, uint32_t value);
uint32_t buf_get_u32(const void *_buffer, unsigned first, unsigned num);

#endif /* BINARYBUFFER_H */

