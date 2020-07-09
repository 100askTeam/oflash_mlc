#include "stdafx.h"

#ifdef WIN32
#define  _X86_     //   _X86_是Intel的CPU，还有_ALPHA_、_PPC_等，都在windows.h中有定义   
#include <windef.h>   
#include <winnt.h>   
#include <winbase.h>
#else  //WIN32
#include <unistd.h>
#endif //! WIN32
#include <stdlib.h>
#include <malloc.h>
#include "drivers/jtag/jtag.h"
#include "common/types.h"
//#define BUILD_FT2232_FTD2XX 1
#if BUILD_FT2232_FTD2XX == 1
#include "lib/FTD2XX.H"
#endif

#if BUILD_FT2232_LIBFTDI == 1
#include "lib/ftdi.h"
#endif


#define LOG_DEBUG printf
#define LOG_WARNING printf
#define LOG_ERROR printf

#if BUILD_FT2232_FTD2XX == 1
static FT_HANDLE ftdih = NULL;
#elif BUILD_FT2232_LIBFTDI == 1
static struct ftdi_context ftdic;
static enum ftdi_chip_type ftdi_device;
char *ft2232_device_desc = NULL;
char *ft2232_serial = NULL;
#endif


static unsigned char ft2232_latency = 2;
u8 *ft2232_buffer = NULL;
int ft2232_buffer_size = 0;
int ft2232_read_pointer = 0;
int ft2232_expect_read = 0;
#define FT2232_BUFFER_SIZE	131072
#define BUFFER_ADD ft2232_buffer[ft2232_buffer_size++]
#define BUFFER_READ ft2232_buffer[ft2232_read_pointer++]

static u8 nTRST, nTRSTnOE, nSRST, nSRSTnOE;
static u8 low_output = 0x0;
static u8 low_direction = 0x0;
static u8 high_output = 0x0;
static u8 high_direction = 0x0;

static int jtag_speed = 0;

#if BUILD_FT2232_FTD2XX == 1
static int ft2232_init_ftd2xx(void)
{
	FT_STATUS status;
	DWORD openex_flags = 0;
	char openex_string[100];
	u8 latency_timer;

	FT_PROGRAM_DATA ftData; 
	char ManufacturerBuf[32]; 
	char ManufacturerIdBuf[16]; 
	char DescriptionBuf[64];
	char SerialNumberBuf[16]; 

	char manufacturer[256];

	ftData.Signature1 = 0x00000000; 
	ftData.Signature2 = 0xffffffff; 
	ftData.Version = 0x00000004; // EEPROM structure with FT4232H extensions 
	ftData.Manufacturer = ManufacturerBuf; 
	ftData.ManufacturerId = ManufacturerIdBuf; 
	ftData.Description = DescriptionBuf; 
	ftData.SerialNumber = SerialNumberBuf;

	manufacturer[0] = 'w';
	manufacturer[1] = 'w';
	manufacturer[2] = 'w';
	manufacturer[3] = '.';
	manufacturer[4] = '1';
	manufacturer[5] = '0';
	manufacturer[6] = '0';
	manufacturer[7] = 'a';
	manufacturer[8] = 's';
	manufacturer[9] = 'k';
	manufacturer[10] = '.';
	manufacturer[11] = 'n';
	manufacturer[12] = 'e';
	manufacturer[13] = 't';
	manufacturer[14] = '\0';

	//openex_string = "USB<=>JTAG&RS232 A";
	openex_string[0] = 'U';
	openex_string[1] = 'S';
	openex_string[2] = 'B';
	openex_string[3] = '<';
	openex_string[4] = '=';
	openex_string[5] = '>';
	openex_string[6] = 'J';
	openex_string[7] = 'T';
	openex_string[8] = 'A';
	openex_string[9] = 'G';
	openex_string[10] = '&';
	openex_string[11] = 'R';
	openex_string[12] = 'S';
	openex_string[13] = '2';
	openex_string[14] = '3';
	openex_string[15] = '2';
	openex_string[16] = ' ';
	openex_string[17] = 'A';
	openex_string[18] = '\0';
	openex_flags = FT_OPEN_BY_DESCRIPTION;

	if ((status = FT_OpenEx(openex_string, openex_flags, &ftdih)) != FT_OK)
	{
		int num_devices;
		
		LOG_ERROR("unable to open ftdi device: %lu\n", status);
		status = FT_ListDevices(&num_devices, NULL, FT_LIST_NUMBER_ONLY);
		if (status == FT_OK)
		{
			char **desc_array = (char **)malloc(sizeof(char*) * (num_devices + 1));
			int i;

			for (i = 0; i < num_devices; i++)
				desc_array[i] = (char *)malloc(64);
			desc_array[num_devices] = NULL;

			status = FT_ListDevices(desc_array, &num_devices, FT_LIST_ALL | openex_flags);

			if (status == FT_OK)
			{
				LOG_ERROR("ListDevices: %lu\n", num_devices);
				for (i = 0; i < num_devices; i++)
					LOG_ERROR("%i: %s\n", i, desc_array[i]);
			}
			
			for (i = 0; i < num_devices; i++)
				free(desc_array[i]);
			free(desc_array);
		}
		else
		{
			LOG_ERROR("ListDevices: NONE\n");
		}
		return ERROR_JTAG_INIT_FAILED;
	}

	if ((status = FT_EE_Read(ftdih, &ftData)) != FT_OK)
	{
		LOG_ERROR("unable to read eeprom: %lu\n", status);
		return ERROR_JTAG_INIT_FAILED;
	}
	
	if (strcmp(ManufacturerBuf, manufacturer) != 0)
	{
		LOG_ERROR("the manufacturer isn't correct\n");
		return ERROR_JTAG_INIT_FAILED;
	}
		
	if ((status = FT_SetLatencyTimer(ftdih, ft2232_latency)) != FT_OK)
	{
		LOG_ERROR("unable to set latency timer: %lu\n", status);
		return ERROR_JTAG_INIT_FAILED;
	}
	
	if ((status = FT_GetLatencyTimer(ftdih, &latency_timer)) != FT_OK)
	{
		LOG_ERROR("unable to get latency timer: %lu\n", status);
		return ERROR_JTAG_INIT_FAILED;
	}
	else
	{
		//LOG_DEBUG("current latency timer: %i\n", latency_timer);
	}
	
	if ((status = FT_SetTimeouts(ftdih, 5000, 5000)) != FT_OK)
	{
		LOG_ERROR("unable to set timeouts: %lu\n", status);
		return ERROR_JTAG_INIT_FAILED;
	}

	/*
	  * FT_STATUS FT_SetBitmode (FT_HANDLE ftHandle, UCHAR ucMask, UCHAR ucMode)
	  * ucMask: Required value for bit mode mask. This sets up which bits are inputs and outputs. 
	  *              A bit value of 0 sets the corresponding pin to an input, a bit value of 1 sets the 
	  *              corresponding pin to an output.
	  *
	  *              In the case of CBUS Bit Bang, the upper nibble of this value controls which pins are
	  *              inputs and outputs, while the lower nibble controls which of the outputs are high and low.
	  * 0xb: 1011, ADBUS0->TCK output, ADBUS1->TDI output, ADBUS2->TDO intput, ADBUS3->TMS output
	  *
	  * ucMode: 0x2 = MPSSE (FT4232H, FT2232H and FT2232 devices only)
	  */
	if ((status = FT_SetBitMode(ftdih, 0x0b, 2)) != FT_OK)
	{
		LOG_ERROR("unable to enable bit i/o mode: %lu\n", status);
		return ERROR_JTAG_INIT_FAILED;
	}

	return ERROR_OK;
}

static int ft2232_purge_ftd2xx(void)
{
	FT_STATUS status;

	if ((status = FT_Purge(ftdih, FT_PURGE_RX | FT_PURGE_TX)) != FT_OK)
	{
		LOG_ERROR("error purging ftd2xx device: %lu\n", status);
		return ERROR_JTAG_INIT_FAILED;
	}

	return ERROR_OK;
}
#endif //BUILD_FT2232_FTD2XX == 1


#if BUILD_FT2232_LIBFTDI == 1
static int ft2232_init_libftdi(void)
{
	uint8_t latency_timer;
	char openex_string[100];
	
	uint16_t vid = 0x1457;
	uint16_t pid = 0x5118;

	//openex_string = "USB<=>JTAG&RS232 A";
	openex_string[0] = 'U';
	openex_string[1] = 'S';
	openex_string[2] = 'B';
	openex_string[3] = '<';
	openex_string[4] = '=';
	openex_string[5] = '>';
	openex_string[6] = 'J';
	openex_string[7] = 'T';
	openex_string[8] = 'A';
	openex_string[9] = 'G';
	openex_string[10] = '&';
	openex_string[11] = 'R';
	openex_string[12] = 'S';
	openex_string[13] = '2';
	openex_string[14] = '3';
	openex_string[15] = '2';
	openex_string[16] = '\0';

	LOG_DEBUG("'ft2232' interface using libftdi with '%s' layout (%4.4x:%4.4x)\n",
			openex_string, vid, pid);

	if (ftdi_init(&ftdic) < 0)
		return ERROR_JTAG_INIT_FAILED;
	
	/* context, vendor id, product id */
	ft2232_device_desc = openex_string;
	if (ftdi_usb_open_desc(&ftdic, vid, pid, ft2232_device_desc,
				ft2232_serial) < 0)
	{
		LOG_ERROR("unable to open ftdi device: %s\n", ftdic.error_str);
		return ERROR_JTAG_INIT_FAILED;
	}

	if (ftdi_set_interface(&ftdic, INTERFACE_A) < 0)
	{
		LOG_ERROR("unable to select FT2232 channel A: %s\n", ftdic.error_str);
		return ERROR_JTAG_INIT_FAILED;
	}

	/* There is already a reset in ftdi_usb_open_desc, this should be redundant */
	if (ftdi_usb_reset(&ftdic) < 0)
	{
		LOG_ERROR("unable to reset ftdi device\n");
		return ERROR_JTAG_INIT_FAILED;
	}

	if (ftdi_set_latency_timer(&ftdic, ft2232_latency) < 0)
	{
		LOG_ERROR("unable to set latency timer\n");
		return ERROR_JTAG_INIT_FAILED;
	}

	if (ftdi_get_latency_timer(&ftdic, &latency_timer) < 0)
	{
		LOG_ERROR("unable to get latency timer\n");
		return ERROR_JTAG_INIT_FAILED;
	}
	else
	{
		LOG_DEBUG("current latency timer: %i\n", latency_timer);
	}

	ftdi_set_bitmode(&ftdic, 0x0b, 2); /* ctx, JTAG I/O mask */

	ftdi_device = ftdic.type;
	static const char* type_str[] =
		{"AM", "BM", "2232C", "R", "2232H", "4232H", "Unknown"};
	unsigned no_of_known_types = sizeof(type_str) / sizeof(type_str[0]) - 1;
	unsigned type_index = ((unsigned)ftdi_device < no_of_known_types)
		? ftdi_device : no_of_known_types;
	LOG_DEBUG("FTDI chip type: %i \"%s\"\n", (int)ftdi_device, type_str[type_index]);
	return ERROR_OK;
}

static int ft2232_purge_libftdi(void)
{
	if (ftdi_usb_purge_buffers(&ftdic) < 0)
	{
		LOG_ERROR("ftdi_purge_buffers: %s\n", ftdic.error_str);
		return ERROR_JTAG_INIT_FAILED;
	}

	return ERROR_OK;
}

#endif /* BUILD_FT2232_LIBFTDI == 1 */

int ft2232_read(u8* buf, int size, u32* bytes_read)
{
#if BUILD_FT2232_FTD2XX == 1
	DWORD dw_bytes_read;
	FT_STATUS status;
	int timeout = 5;
	*bytes_read = 0;

	while ((*bytes_read < (u32)size) && timeout--)
	{
		if ((status = FT_Read(ftdih, buf + *bytes_read, size - 
			*bytes_read, &dw_bytes_read)) != FT_OK)		
		{
			*bytes_read = 0; 
			LOG_ERROR("FT_Read returned: %lu\n", status);
			return ERROR_JTAG_DEVICE_ERROR;
		}
		*bytes_read += dw_bytes_read; 
	}
#elif BUILD_FT2232_LIBFTDI == 1
	int retval;
	int timeout = 100;
	*bytes_read = 0;
	
	while ((*bytes_read < (u32)size) && timeout--)
	{
		if ((retval = ftdi_read_data(&ftdic, buf + *bytes_read, size - *bytes_read)) < 0)
		{
			*bytes_read = 0;
			LOG_ERROR("ftdi_read_data: %s\n", ftdi_get_error_string(&ftdic));
			return ERROR_JTAG_DEVICE_ERROR;
		}
		*bytes_read += retval;
	}
#endif

	if (*bytes_read < (u32)size)
	{
		LOG_ERROR("couldn't read the requested number of bytes from FT2232 device (%i < %i)\n", *bytes_read, size);
		return ERROR_JTAG_DEVICE_ERROR;
	}
	
	return ERROR_OK;
}

int ft2232_write(u8 *buf, int size, u32* bytes_written)
{
#if BUILD_FT2232_FTD2XX == 1
	FT_STATUS status;
	DWORD dw_bytes_written;
	if ((status = FT_Write(ftdih, buf, size, &dw_bytes_written)) != FT_OK)
	{
		*bytes_written = dw_bytes_written;
		LOG_ERROR("FT_Write returned: %lu\n", status);
		return ERROR_JTAG_DEVICE_ERROR;
	}
	else
	{
		*bytes_written = dw_bytes_written;
		return ERROR_OK;	
	}
#elif BUILD_FT2232_LIBFTDI == 1
	int retval;
	if ((retval = ftdi_write_data(&ftdic, buf, size)) < 0)
	{
		*bytes_written = 0;
		LOG_ERROR("ftdi_write_data: %s\n", ftdi_get_error_string(&ftdic));
		return ERROR_JTAG_DEVICE_ERROR;
	}
	else
	{
		*bytes_written = retval;
		return ERROR_OK;	
	}
#endif
}

int jtagkey_init(void)
{
	u8 buf[3];
	u32 bytes_written;
	
	low_output = 0x08;
	low_direction = 0x1b;
	
	/* initialize low byte for jtag 
	  * 0x80: This will setup the direction of the first 8 lines and force a value on the bits 
	  * that are set as output. A '1' in the Direction byte will make that bit an output.
	  *
	  * ADBUS0->TCK output, ADBUS1->TDI output, ADBUS2->TDO intput, ADBUS3->TMS output
	  * ADBUS3->nOE output
	  */
	buf[0] = 0x80; 			/* command "set data bits low byte" */
	buf[1] = low_output; 	/* value (0x8 = 0b1000 : TCK=0, TDI=0, TDO = 0, TMS=1, nOE=0) */
	buf[2] = low_direction; /* dir (output=1), TCK/TDI/TMS=out, TDO=in, nOE=out */
	//LOG_DEBUG("%2.2x %2.2x %2.2x\n", buf[0], buf[1], buf[2]);
	
	if (((ft2232_write(buf, 3, &bytes_written)) != ERROR_OK) || (bytes_written != 3))
	{
		LOG_ERROR("couldn't initialize FT2232 with 'JTAGkey' layout\n"); 
		return ERROR_JTAG_INIT_FAILED;
	}
	
	nTRST = 0x01;		/* ACBUS0 */
	nTRSTnOE = 0x4;		/* ACBUS2 */
	nSRST = 0x02;		/* ACBUS1 */
	nSRSTnOE = 0x08;	/* ACBUS3 */	

	/* reset JTAG and chip, thisway.diy@163.com */
	/* step 1: reset JTAG */
	high_output = 0x0;
	high_direction = 0x0f;

	high_output &= ~nTRSTnOE;
	high_output &= ~nTRST;  	/* nTRST enable */
	
	high_output |= nSRSTnOE;
	high_output |= nSRST;		/* nSRST disable */

	buf[0] = 0x82; 				/* command "set data bits high byte" */
	buf[1] = high_output; 		/* value */
	buf[2] = high_direction;   	/* Direction: all outputs (xRST and xRSTnOE) */
	//LOG_DEBUG("%2.2x %2.2x %2.2x\n", buf[0], buf[1], buf[2]);
	
	if (((ft2232_write(buf, 3, &bytes_written)) != ERROR_OK) || (bytes_written != 3))
	{
		LOG_ERROR("couldn't initialize FT2232 with 'JTAGkey' layout\n"); 
		return ERROR_JTAG_INIT_FAILED;
	}

#ifdef WIN32
	Sleep(100);
#else
	sleep(1);
#endif
#if 0
	/* step 2: reset chip */
	high_output = 0x0;
	high_direction = 0x0f;

	high_output |= nTRSTnOE;
	high_output |= nTRST;  	/* nTRST disable */
	
	high_output &= ~nSRSTnOE;
	high_output &= ~nSRST;		/* nSRST low */

	buf[0] = 0x82; 				/* command "set data bits high byte" */
	buf[1] = high_output; 		/* value */
	buf[2] = high_direction;   	/* Direction: all outputs (xRST and xRSTnOE) */
	//LOG_DEBUG("%2.2x %2.2x %2.2x\n", buf[0], buf[1], buf[2]);
	
	if (((ft2232_write(buf, 3, &bytes_written)) != ERROR_OK) || (bytes_written != 3))
	{
		LOG_ERROR("couldn't reset the target board\n"); 
		return ERROR_JTAG_INIT_FAILED;
	}
#ifdef WIN32
	Sleep(200);
#else
	sleep(1);
#endif
#endif
	/* end of reset JTAG and chip */
	
	high_output = 0x0;
	high_direction = 0x0f;

	high_output &= ~nTRSTnOE;
	high_output |= nTRST;  		/* nTRST output high */
	
	high_output |= nSRSTnOE;
	high_output &= ~nSRST;		/* nSRST disable */
	
	/* initialize high port 
	  * 0x82: This will setup the direction of the high 4 lines and force a value on 
	  * the bits that are set as output. A '1' in the Direction byte will make that bit an output.
	  *
	  */
	
	buf[0] = 0x82; 				/* command "set data bits high byte" */
	buf[1] = high_output; 		/* value */
	buf[2] = high_direction;   	/* Direction: all outputs (xRST and xRSTnOE) */
	//LOG_DEBUG("%2.2x %2.2x %2.2x\n", buf[0], buf[1], buf[2]);
	
	if (((ft2232_write(buf, 3, &bytes_written)) != ERROR_OK) || (bytes_written != 3))
	{
		LOG_ERROR("couldn't initialize FT2232 with 'JTAGkey' layout\n"); 
		return ERROR_JTAG_INIT_FAILED;
	}
	
	return ERROR_OK;
}


int ft2232_speed(int speed)
{
	u8 buf[3];
	int retval;
	u32 bytes_written;

	buf[0] = 0x86; 					/* command "set divisor" */
	buf[1] = speed & 0xff; 			/* valueL (0=6MHz, 1=3MHz, 2=2.0MHz, ...*/
	buf[2] = (speed >> 8) & 0xff; 	/* valueH */
	
	if (((retval = ft2232_write(buf, 3, &bytes_written)) != ERROR_OK) || (bytes_written != 3))
	{
		LOG_ERROR("couldn't set FT2232 TCK speed\n");
		return retval;
	}

	return ERROR_OK;
}

int ft2232_init(void)
{
	u8 buf[1];
	int retval;
	u32 bytes_written;
	static int OpenFlag = 0;

	if (OpenFlag)	// has Open
		return 0;

#if BUILD_FT2232_FTD2XX == 1
	retval = ft2232_init_ftd2xx( );
#elif BUILD_FT2232_LIBFTDI == 1
	retval = ft2232_init_libftdi();
#endif	

	if (retval < 0)
		return retval;

	ft2232_buffer_size = 0;
	ft2232_buffer = (unsigned char *)malloc(FT2232_BUFFER_SIZE);

	if (jtagkey_init() != ERROR_OK)
		return ERROR_JTAG_INIT_FAILED;

	ft2232_speed(jtag_speed);

	buf[0] = 0x85; /* Disconnect TDI/DO to TDO/DI for Loopback */
	if (((retval = ft2232_write(buf, 1, &bytes_written)) != ERROR_OK) || (bytes_written != 1))
	{
		LOG_ERROR("couldn't write to FT2232 to disable loopback\n");
		return ERROR_JTAG_INIT_FAILED;
	}

#if BUILD_FT2232_FTD2XX == 1
	if (!ft2232_purge_ftd2xx())
	{
		OpenFlag = 1;
		return 0;
	}
	else
		return -1;
#elif BUILD_FT2232_LIBFTDI == 1
	if (!ft2232_purge_libftdi())
	{
		OpenFlag = 1;
		return 0;
	}
	else
		return -1;
#endif		
}


int ft2232_readID(void)
{
	int retval;
	u32 bytes_written;
	u32 bytes_read;
	int id;

	ft2232_buffer_size = 0;
	/* make sure the device is in TLR status */	
	BUFFER_ADD = 0x4b;	/* command "Clock Data to TMS/CS Pin (no Read)" */	
	BUFFER_ADD = 0x6;	/* scan 7 bit */
	BUFFER_ADD = TAP_MOVE(TAP_TLR, TAP_TLR);	/* TMS data bits */

	/* make the device enter Shift-IR Status */
	BUFFER_ADD = 0x4b;	/* command "Clock Data to TMS/CS Pin (no Read)" */
	BUFFER_ADD = 0x6;	/* scan 7 bit */
	BUFFER_ADD = TAP_MOVE(TAP_TLR, TAP_SI);		/* TMS data bits */

	/* send S3C24x0 IDCODE Instruction "1110", first send "110"*/
	BUFFER_ADD = 0x1b;	/* Clock Data Bits Out on -ve Clock Edge LSB First (no Read) */
	BUFFER_ADD = 2;		/* send 3 bit */
	BUFFER_ADD = 0x6;
	/*second: make the device enter Shift-DR Status */
	BUFFER_ADD = 0x4b;	/* command "Clock Data to TMS/CS Pin (no Read)" */
	BUFFER_ADD = 4;		/* scan 5 bit */
	BUFFER_ADD = (1<<7)|0x7;		/* IDCODE's last "1" and TMS data bits, -> Shift DR */
	
	BUFFER_ADD = 0x39;
	BUFFER_ADD = 3;		/* write/read 4 bytes */
	BUFFER_ADD = 0;
	for (int i = 0; i < 4; i++)
		BUFFER_ADD = 0xff;
	BUFFER_ADD = 0x4b;	/* command "Clock Data to TMS/CS Pin (no Read)" */
	BUFFER_ADD = 0x6;	/* scan 7 bit */
	BUFFER_ADD = 0X7f;	/* to TLR status */

	if ((retval = ft2232_write(ft2232_buffer, ft2232_buffer_size, &bytes_written)) != ERROR_OK)
	{
		LOG_ERROR("couldn't write MPSSE commands to FT2232\n");
		return retval;
	}
		
	ft2232_expect_read = 4;
	if ((retval = ft2232_read(ft2232_buffer, ft2232_expect_read, &bytes_read)) != ERROR_OK)
	{
		LOG_ERROR("couldn't read from FT2232, bytes_read = %d\n", bytes_read);
//		return retval;
	}

	id = *((int *)ft2232_buffer);
	
	return id;
}

void ft2232_clear_buf(void)
{
	ft2232_buffer_size = 0;
}

int ft2232_exec_buf(void)
{
	int retval;
	u32 bytes_written;
	
	if ((retval = ft2232_write(ft2232_buffer, ft2232_buffer_size, &bytes_written)) != ERROR_OK)
	{
		LOG_ERROR("couldn't write MPSSE commands to FT2232\n");
		return -1;
	}
	ft2232_buffer_size = 0;
	return 0;
}

int ft2232_approach_full(void)
{
	return ft2232_buffer_size > FT2232_BUFFER_SIZE * 5 / 10;
}

int ft2232_quit(void)
{
#if BUILD_FT2232_FTD2XX == 1
	FT_STATUS status;
#endif
	u8 buf[3];
	u32 bytes_written;

	high_output = 0x0;
	high_direction = 0x0f;

	high_output |= nTRSTnOE;
	high_output |= nTRST;  	/* nTRST disable */
	
	high_output &= ~nSRSTnOE;
	high_output &= ~nSRST;		/* nSRST low */

	buf[0] = 0x82; 				/* command "set data bits high byte" */
	buf[1] = high_output; 		/* value */
	buf[2] = high_direction;   	/* Direction: all outputs (xRST and xRSTnOE) */
	//LOG_DEBUG("%2.2x %2.2x %2.2x\n", buf[0], buf[1], buf[2]);
	
	if (((ft2232_write(buf, 3, &bytes_written)) != ERROR_OK) || (bytes_written != 3))
	{
		LOG_ERROR("couldn't initialize FT2232 with 'JTAGkey' layout\n"); 
		return ERROR_JTAG_INIT_FAILED;
	}

#ifdef WIN32	
	Sleep(500);
#else
	sleep(1);
#endif
	high_output = 0x0;
	high_direction = 0x0f;

	high_output |= nTRSTnOE;
	high_output |= nTRST;  	/* nTRST disable */
	
	high_output |= nSRSTnOE;
	high_output |= nSRST;		/* nSRST disable */
	
	/* initialize high port 
	  * 0x82: This will setup the direction of the high 4 lines and force a value on 
	  * the bits that are set as output. A '1' in the Direction byte will make that bit an output.
	  *
	  */
	
	buf[0] = 0x82; 				/* command "set data bits high byte" */
	buf[1] = high_output; 		/* value */
	buf[2] = high_direction;   	/* Direction: all outputs (xRST and xRSTnOE) */
	//LOG_DEBUG("%2.2x %2.2x %2.2x\n", buf[0], buf[1], buf[2]);
	
	if (((ft2232_write(buf, 3, &bytes_written)) != ERROR_OK) || (bytes_written != 3))
	{
		LOG_ERROR("couldn't initialize FT2232 with 'JTAGkey' layout\n"); 
		return ERROR_JTAG_INIT_FAILED;
	}

#if BUILD_FT2232_FTD2XX == 1
	status = FT_Close(ftdih);
#endif
#if BUILD_FT2232_LIBFTDI == 1
//	ftdi_disable_bitbang(&ftdic);
	ftdi_usb_close(&ftdic);
	ftdi_deinit(&ftdic);
#endif
	free(ft2232_buffer);
	ft2232_buffer = NULL;

	return ERROR_OK;
}


#define CEIL(m, n)	((m + n - 1) / n)

int ft2232_predict_scan_in(int scan_size) //, enum scan_type type)
{
	int predicted_size = 0;
	
	if (1) //type != SCAN_OUT)
	{
		/* complete bytes */
		predicted_size += (CEIL(scan_size, 8) > 1) ? (CEIL(scan_size, 8) - 1) : 0;
		/* remaining bits - 1 */
		predicted_size += ((scan_size - 1) % 8) ? 1 : 0;
		/* last bit (from TMS scan) */
		predicted_size += 1;
	}
	
	/* LOG_DEBUG("scan_size: %i, predicted_size: %i", scan_size, predicted_size); */

	return predicted_size;
}

void ft2232_read_scan(u8* buffer, int scan_size)
{
	int num_bytes = ((scan_size + 7) / 8);
	int bits_left = scan_size;
	int cur_byte = 0;

	while(num_bytes-- > 1)
	{
		buffer[cur_byte] = BUFFER_READ;
		cur_byte++;
		bits_left -= 8;
	}

	buffer[cur_byte] = 0x0;

	if (bits_left > 1)
	{
		buffer[cur_byte] = BUFFER_READ >> 1;
	}

	buffer[cur_byte] = (buffer[cur_byte] | ((BUFFER_READ & 0x02) << 6)) >> (8 - bits_left);

}


