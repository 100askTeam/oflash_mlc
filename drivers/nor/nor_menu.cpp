
#include "stdafx.h"
#include <common.h>
#include "flash.h"
#include "config.h"
#include "drivers/jtag/jtag.h"
#include "drivers/jtag/mem_rdwr.h"
#include "drivers/ft2232/ft2232.h"

extern int LoadImageFile(U8 *buf,int size);
extern u32 imageSize;

static U32 targetSize;	    // Total byte size 
static U32 srcAddress;

static U32 targetAddress;
static U32 targetOffset;

/*
 * print sizes as "xxx kB", "xxx.y kB", "xxx MB" or "xxx.y MB" as needed;
 * allow for optional trailing string (like "\n")
 */
static void print_size (ulong size, const char *s)
{
	ulong m, n;
	ulong d = 1 << 20;		/* 1 MB */
	char  c = 'M';

	if (size < d) {			/* print in kB */
		c = 'k';
		d = 1 << 10;
	}

	n = size / d;

	m = (10 * (size - (n * d)) + (d / 2) ) / d;

	if (m >= 10) {
		m -= 10;
		n += 1;
	}

	printf ("%2ld", n);
	if (m) {
		printf (".%ld", m);
	}
	printf (" %cB%s", c, s);
}

void print_flinfo(void)
{
	int bank;

	for (bank=0; bank <CFG_MAX_FLASH_BANKS; ++bank) {
		printf("\nBank # %ld: ", bank+1);
		flash_print_info (&flash_info[bank]);
	}
}


void Nor_Program(void)
{
	int bank;
	int i, j;
	ulong end;
	int found_start = 0;
	int found_end = 0;
	
    targetSize = imageSize;

    if (!srcAddress)
		srcAddress = (U32)malloc(targetSize+4);
	
    if (srcAddress == 0)
    {
		printf("Can't alloc enough memory\n");
		return;
    }
	
    LoadImageFile((U8 *)srcAddress, targetSize);

    printf("\nImage Size: 0x%x\n", targetSize);
    printf("\nAvailable Target Offset:\n"); 
	print_flinfo();
    printf("Input target offset:");
	if (g_Offset == -1)
	    scanf("%x", &targetOffset);    
	else
	{
		targetOffset = g_Offset;
		printf("%d\n", targetOffset);
	}

	for (bank = 0; bank < CFG_MAX_FLASH_BANKS; bank++) 
	{
		for (i = 0; i < flash_info[bank].sector_count; i++)
		{
			if (targetOffset == flash_info[bank].start[i])
			{
				found_start = 1;
				j = flash_info[bank].sector_count;
				end = flash_info[bank].size;
				while (end - targetOffset >= targetSize)
				{
					found_end = 1;
					j--;
					end = flash_info[bank].start[j];
				}
				goto out;				
			}		
		}
	}

out:		
	if (!found_start)
	{
		printf("Please enter the correct offset!\n");
		return;
	}
	if (!found_end)
	{
		printf("The file is too big to write at the offset!\n");
		return;
	}

	flash_erase(&flash_info[bank], i, j);

	fprintf(stderr, "write ...");
	flash_write((char *)srcAddress, flash_info[bank].start[i], targetSize);
	jtag_opr->exec_buf();

#if BUILD_FT2232_FTD2XX == 1
		Sleep(1000);  /* 等待一会让nor的烧写完成, 不等待的话后面的代码会立刻复位开发板, 导致最后2字节的烧写有可能失败 */
#else
		sleep(1);
#endif
	fprintf(stderr, "done\n");
		
}

void Nor_Menu(void)
{
	ulong size;

	jtag_opr->clear_buf();
    jtag_mem_rdwr->MRW_JtagInit();
	jtag_opr->exec_buf();
	
	printf("Detect Nor Flash ...\n");
	size = flash_init ();

	if (size)
	{
		printf("Size:");
		print_size (size, "\n");
	}
	else
	{
		printf("No nor flash detected!\n");
		return ;
	}
	Nor_Program();
	
}


