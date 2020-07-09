#ifndef _CONFIG_H_
#define _CONFIG_H_

#define CFG_MAXARGS 	7

extern int g_bUseCfgFile;
extern int g_JTAGType;		// 0 : OpenJTAG,  1 : dongle(²¢¿Ú)
extern int g_CPUType;		// 1 : s3c2410,  2 : s3c2440
extern int g_FlashType;	// 0 : nand flash, 1 : nor flash
extern int g_ReadOrWrite;	// 0 : write, 1 : read
extern unsigned int g_Offset;
extern char *g_FileName;

extern int g_argc;
extern char *g_argv[CFG_MAXARGS];

void printusage(void);
int pase_argv(int argc, char **argv);
int parse_line (char *line, char *argv[]);
int parse_file(void);
int isUseCfgFile(void);


#endif
