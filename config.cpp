#include "stdafx.h"
#ifdef WIN32
#define  _X86_     //   _X86_是Intel的CPU，还有_ALPHA_、_PPC_等，都在windows.h中有定义   
#include <windef.h>   
#include <winnt.h>   
#include <winbase.h>
#endif
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include "common/types.h"
#include "config.h"

int g_bUseCfgFile = 0;
int g_JTAGType    = -1;		// 0 : OpenJTAG,  1 : dongle(并口)
int g_CPUType     = -1;		// 1 : s3c2410,  2 : s3c2440, 3 : s3c6410
int g_FlashType   = -1;		// 0 : nand flash, 1 : nor flash
int g_ReadOrWrite = -1;		// 0 : write, 1 : read
unsigned int g_Offset      = -1;
char *g_FileName  = NULL;

int g_argc;
char *g_argv[CFG_MAXARGS];

static char *cfgfile = "cfg.txt";

void printusage(void)
{
	printf("1. oflash, run with cfg.txt or prompt\r\n");
	printf("2. oflash [file], write [file] to flash with prompt\r\n");
	printf("3. oflash [-f config_file]\r\n");
	printf("4. oflash [jtag_type] [cpu_type] [flash_type] [read_or_write] [offset] [file]\r\n");
}

int pase_argv(int argc, char **argv)
{
	if ((argc != 0) && (argc != 1) && (argc != 2) && (argc != 6))
	{
		goto err;
	}
	
	if (argc == 0)
	{
		g_bUseCfgFile = 1;
	}

	if (argc == 1)
	{
		g_FileName = argv[0];
	}

	if (argc == 2)
	{
		if (strcmp(argv[0], "-f") == 0)
		{
			cfgfile = argv[1];
			g_bUseCfgFile = 1;
		}
		else
		{
			goto err;
		}
	}

	if (argc == 6)
	{
		g_JTAGType = strtol(argv[0], NULL, 0);
		if (g_JTAGType != 0 && g_JTAGType != 1)
		{
			g_JTAGType = -1;
			goto err;
		}

		g_CPUType = strtol(argv[1], NULL, 0);
		if (g_CPUType != 0 && g_CPUType != 1 && g_CPUType != 2)
		{
			g_CPUType = -1;
			goto err;
		}

		g_FlashType = strtol(argv[2], NULL, 0);
		if (g_FlashType != 0 && g_FlashType != 1)
		{
			g_FlashType = -1;
			goto err;
		}
		
		g_ReadOrWrite = strtol(argv[3], NULL, 0);
		if (g_ReadOrWrite != 0 && g_ReadOrWrite != 1)
		{
			g_ReadOrWrite = -1;
			goto err;
		}

		g_Offset = strtoul(argv[4], NULL, 0);

		g_FileName = argv[5];
		
	}

	return 0;
err:
	//printusage();
	return -1;
}


/****************************************************************************/

int parse_line (char *line, char *argv[])
{
	int nargs = 0;

	while (nargs < CFG_MAXARGS) {

		/* skip any white space */
		while ((*line == ' ') || (*line == '\t')) {
			++line;
		}

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;
			return (nargs);
		}

		argv[nargs++] = line;	/* begin of argument string	*/

		/* find end of string */
		while (*line && (*line != ' ') && (*line != '\t')) {
			++line;
		}

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;
			return (nargs);
		}

		*line++ = '\0';		/* terminate current arg	 */
	}

	printf ("** Too many args (max. %d) **\n", CFG_MAXARGS);

	return (nargs);
}

int parse_file(void)
{
	static char *buffer = NULL;
	static FILE *file   = NULL;
	static int first = 1;

	g_CPUType     = -1;		// 1 : s3c2410,  2 : s3c2440
	g_FlashType   = -1;		// 0 : nand flash, 1 : nor flash
	g_ReadOrWrite = -1;		// 0 : write, 1 : read
	g_Offset      = -1;
	g_FileName    = NULL;

	if (!g_bUseCfgFile)
		return -1;

	if (file == NULL)
	{
		file = fopen(cfgfile, "r");
		if (file == NULL)
		{
			g_bUseCfgFile = 0;
			if (first)
			{
				printf("Can't open %s, you should follow the prompt\n", cfgfile);
				first = 0;
			}
			return -1;
		}
	}

	first = 0;

	if (buffer==NULL)
	{
		buffer = (char *)malloc(4096);
		if (buffer==NULL)
		{
			printf("Can't malloc buffer\n\r");
			return -1;
		}
	}
	
	while (fgets(buffer, 4096, file) != NULL)
	{
		char *p;
		char *line, *end;
		
		/* stop processing line after a comment (#, !) or a LF, CR were encountered */
		if ((p = strpbrk(buffer, "#!\r\n")))
			*p = 0;

		/* skip over leading whitespace */
		line = buffer;
		while (isspace(*line))
			line++;

		/* empty (all whitespace) line? */
		if (!*line)
			continue;
		
		/* search the end of the current line, ignore trailing whitespace */
		for (p = end = line; *p; p++)
			if (!isspace(*p))
				end = p;
		
		/* terminate end */
		*++end = 0;

		/* parse line */
		g_argc = parse_line(line, g_argv);

		if (g_argc != 0)
		{
			return pase_argv(g_argc, g_argv);
		}
		else
			continue;
	}
	
	// end of file
	free(buffer);
	fclose(file);
	return -1;

}

int isUseCfgFile(void)
{
	return g_bUseCfgFile;
}
