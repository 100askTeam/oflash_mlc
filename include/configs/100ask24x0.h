#ifndef HundredASK24X0_H
#define HundredASK24X0_H


#define CONFIG_S3C2410 1

#define CFG_FLASH_CFI				1
#define CFG_FLASH_CFI_DRIVER 		1
#define CFG_FLASH_USE_BUFFER_WRITE	1
#define CONFIG_FLASH_CFI_LEGACY		1

#define CFG_MAX_FLASH_BANKS	1
#define CFG_FLASH_BASE				0x0000000

#ifdef CFG_FLASH_CFI_DRIVER
#define CFG_MAX_FLASH_SECT	 512   /* max number of sectors on one chip */
#define CFG_FLASH_CFI_CHIP_WIDTH  FLASH_CFI_16BIT
#define CFG_FLASH_CFI_PORT_WIDTH  FLASH_CFI_16BIT  /* FLASH_CFI_16BIT */
#endif

#undef __LITTLE_ENDIAN
#define __LITTLE_ENDIAN	1

/*-----------------------------------------------------------------------
 * NAND flash settings
 */
#define CFG_NAND_BASE           0
#define CFG_MAX_NAND_DEVICE     1
#define NAND_MAX_CHIPS          1
#define NAND_ALLOW_ERASE_ALL	1

#endif /* HundredASK24X0_H */
