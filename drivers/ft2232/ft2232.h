#ifndef FT2232_H
#define FT2232_H

#include "common/types.h"

extern u8 *ft2232_buffer;
extern int ft2232_buffer_size;
extern int ft2232_read_pointer;
extern int ft2232_expect_read;
#define FT2232_BUFFER_SIZE	131072
#define BUFFER_ADD ft2232_buffer[ft2232_buffer_size++]
#define BUFFER_READ ft2232_buffer[ft2232_read_pointer++]

int ft2232_init(void);
int ft2232_readID(void);
int ft2232_readID2(void);
int ft2232_quit(void);
int ft2232_write(u8 *buf, int size, u32* bytes_written);
int ft2232_read(u8* buf, u32 size, u32* bytes_read);
void ft2232_clear_buf(void);
int ft2232_exec_buf(void);
int ft2232_approach_full(void);
int ft2232_predict_scan_in(int scan_size);
void ft2232_read_scan(u8* buffer, int scan_size);


#endif /* FT2232_H */
