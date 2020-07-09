// oflash.cpp : Defines the entry point for the console application.
//

/*
  * v1.0 : 基本功能
  * v1.1 : 支持CFI Nor Flash, write buffer
  * v1.2 : 支持配置文件
  */

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
#include "config.h"
#include "drivers/ft2232/ft2232.h"
#include "drivers/jtag/jtag.h"
#include "cpu/arm/s3c24x0/pin2440.h"
#include "drivers/jtag/mem_rdwr.h"
#include "drivers/nand/nand_menu.h"
#include "drivers/nor/nor_menu.h"


static const void *function[]=
{
    "Nand Flash prog    ",
    "Nor Flash prog  ",
    "Memory Rd/Wr    ",
    "Exit            ",
    0
};

FILE *stream;
U32 imageSize;

char srcFileName[256];
void GetFileName(char *filename);
int OpenImageFile(char *filename);
void ErrorBanner(void);
int delayLoopCount = 5;

int main(int argc, char* argv[])
{
    int delayLoopCount=0;
	int i;
	U32 cpuID;
	void test(void);
	char str[100];

	/*"OpenJTAG of www.100ask.net     |"*/
	str[0] = 'O';
	str[1] = 'p';
	str[2] = 'e';
	str[3] = 'n';
	str[4] = 'J';
	str[5] = 'T';
	str[6] = 'A';
	str[7] = 'G';
	str[8] = ' ';
	str[9] = 'o';
	str[10] = 'f';
	str[11] = ' ';
	str[12] = 'w';
	str[13] = 'w';
	str[14] = 'w';
	str[15] = '.';
	str[16] = '1';
	str[17] = '0';
	str[18] = '0';
	str[19] = 'a';
	str[20] = 's';
	str[21] = 'k';
	str[22] = '.';
	str[23] = 'n';
	str[24] = 'e';
	str[25] = 't';
	str[26] = ' ';
	str[27] = ' ';
	str[28] = '|';
	str[29] = '\0';
	
    printf("\n");
    printf("+---------------------------------------------------------+\n");
    //printf("|     Flash Programmer for OpenJTAG of www.100ask.net     |\n");
    printf("|   Flash Programmer v1.5.6 for ");
	printf("%s\n", str);
	printf("|   OpenJTAG is a USB to JTAG & RS232 tool based FT2232   |\n");
    printf("|   This programmer supports both of S3C24X0 & S3C6410    |\n");
    //printf("+   Author: Email/MSN(thisway.diy@163.com), QQ(17653039)  |\n");
    str[0] = '|';
	str[1] = ' ';
	str[2] = ' ';
	str[3] = ' ';
	str[4] = 'A';
	str[5] = 'u';
	str[6] = 't';
	str[7] = 'h';
	str[8] = 'o';
	str[9] = 'r';
	str[10] = ':';
	str[11] = ' ';
	str[12] = 'E';
	str[13] = 'm';
	str[14] = 'a';
	str[15] = 'i';
	str[16] = 'l';
	str[17] = '/';
	str[18] = 'M';
	str[19] = 'S';
	str[20] = 'N';
	str[21] = '(';
	str[22] = 't';
	str[23] = 'h';
	str[24] = 'i';
	str[25] = 's';
	str[26] = 'w';
	str[27] = 'a';
	str[28] = 'y';
	str[29] = '.';
	str[30] = 'd';
	str[31] = 'i';
	str[32] = 'y';
	str[33] = '@';
	str[34] = '1';
	str[35] = '6';
	str[36] = '3';
	str[37] = '.';
	str[38] = 'c';
	str[39] = 'o';
	str[40] = 'm';
	str[41] = ')';
	str[42] = ',';
	str[43] = ' ';
	str[44] = 'Q';
	str[45] = 'Q';
	str[46] = '(';
	str[47] = '1';
	str[48] = '7';
	str[49] = '6';
	str[50] = '5';
	str[51] = '3';
	str[52] = '0';
	str[53] = '3';
	str[54] = '9';
	str[55] = ')';  
	str[56] = ' ';
	str[57] = ' ';
	str[58] = '|';
	str[59] = '\0';
	printf("%s\n", str);
    printf("+---------------------------------------------------------+\n");
    printf("Usage: \n");
	printusage();
	
	pase_argv(argc - 1, &argv[1]);

	if (isUseCfgFile())
		parse_file();
	do
	{

	    srcFileName[0]='\0';

		if (g_FileName)
			strcpy(srcFileName, g_FileName);

		printf("Select the JTAG type: \n");
		printf("0. OpenJTAG\n");
		printf("1. Dongle JTAG(parallel port)\n");
		printf("2. Wiggler JTAG(parallel port)\n");
		printf("Enter the number: ");
		if (g_JTAGType == -1)
			scanf("%d", &g_JTAGType);
		else
			printf("%d\n\r", g_JTAGType);

		if (g_JTAGType == 0)
		{
			jtag_opr = &OpenJTAG_opr;
		}
		else if(g_JTAGType == 1)
		{
			jtag_opr = &Dongle_opr;
		}
		else
		{
			jtag_opr = &Wiggler_opr;
		}

		printf("Select the CPU:\n");
		i = 0;
		while (all_jtag_mem_rdwr[i])
		{
			printf("%d. %s\n", i, all_jtag_mem_rdwr[i]->CPUName);
			i++;
		}
		printf("Enter the number: ");
		
		if (g_CPUType == -1)
			scanf("%d", &g_CPUType);
		else
			printf("%d\n\r", g_CPUType);

		if (g_CPUType == 0)
		{
			jtag_mem_rdwr = &sc32410_jtag_mem_rdwr;
			nandflash_controller = &s3c2410_nandflash_controller;
		}
		else if (g_CPUType == 1)
		{
			jtag_mem_rdwr = &sc32440_jtag_mem_rdwr;
			nandflash_controller = &s3c2440_nandflash_controller;
		}
		else
		{
			jtag_mem_rdwr = &sc36410_jtag_mem_rdwr;
			nandflash_controller = &s3c6410_nandflash_controller;
		}

		if (0 != jtag_opr->JTAG_Open())
		{
			if (g_JTAGType == 0)
			{
				printf("Failed to open ftdi device: \n");
				printf("1. Have you connected the OpenJTAG?\n");
				printf("2. Are another program using the OpenJTAG?\n");
				printf("Please check that.\n");
			}
			else if (g_JTAGType == 1)
			{
				printf("Failed to open Dongle JTAG(parallel port)\n");
			}
			else
			{
				printf("Failed to open Wiggler JTAG(parallel port)\n");
			}
			goto err_exit;
		}		
		
		cpuID = jtag_opr->ReadCPUID();
		if (cpuID == all_jtag_mem_rdwr[g_CPUType]->CPUID)
		{
			printf("%s detected, cpuID = 0x%08x\n", all_jtag_mem_rdwr[g_CPUType]->CPUName, cpuID);
		}
		else
		{
			printf("No CPU detectred, cpuID = 0x%08x\n", cpuID);
			goto err_exit;
		}

		jtag_mem_rdwr->InitCell();

		//test();
	    printf("\n[Main Menu]\n");
	
	    i=0;
	    while(1)
	    {
			//display menu
			printf("%2d:%s",i,(char *)function[i]);
			i++;
			if((int)(function[i])==0)
			{
			    printf("\n");
			    break;
			}
			if((i%4)==0)
			    printf("\n");
	    }
    
	    printf("Select the function to test:");
		if (g_FlashType == -1)
		    scanf("%d",&g_FlashType);
		else
			printf("%d\n\r", g_FlashType);
		
	    switch(g_FlashType)
	    {
		    case 0:	/* Nand prog */
				if(srcFileName[0]==0)
				{
				    GetFileName(srcFileName);
				}
				if (OpenImageFile(srcFileName))
					goto err_exit;
				Nand_Menu();
				break;

		    case 1:	/* Nor prog */
				if(srcFileName[0]==0)
				{
				    GetFileName(srcFileName);
				}
				if (OpenImageFile(srcFileName))
					goto err_exit;
				Nor_Menu();
				break;

		    case 2:	/* Mem rd/wr */
				jtag_mem_rdwr->MRW_Menu();
				break;

		    default:
				return 0;
				break; //Exit menu
	    }
		
	}while (parse_file() == 0);

	jtag_opr->quit();
	return 0;
	
err_exit:
	printf("Press 'x' to exit: ");
	while (1)
	{
		scanf("%c", &str[0]);
		if (str[0] == 'x' || str[0] == 'X')
			break;
	}
	return -1;
}


int LoadImageFile(U8 *buf,int size)
{
    int i,readSize=size;
    for(i=0;i<size;i++)
    {
	if(feof(stream))
	{
	    readSize=i;
	    for(;i<size;i++)buf[i]=0;
	    break;
	}
	buf[i] = fgetc(stream);
    }
    return readSize;
}

void GetFileName(char *filename)
{
	printf("Enter the file name: ");
	scanf("%s", filename);
}
	

//////////////////////////////////////////////////////////
int OpenImageFile(char *filename)
{
    U32 fileEnd,fileStart;
    stream = fopen(filename,"rb");
    if(stream==NULL)
    {
		printf("ERROR:can't find the file : %s.\n", filename);
		return -1;
    }

    fseek(stream,0L,SEEK_END);
    fileEnd=ftell(stream);
    fseek(stream,0L,SEEK_SET);
    fileStart=ftell(stream);

    imageSize=fileEnd-fileStart;  /* fileend == peof+1 */
	return 0;
}


