#include "stdafx.h"
#include <common.h>
#include <string.h>
#include <malloc.h>
#include <mtd/compat.h>
#include <mtd/mtd.h>
#include <mtd/nand.h>
#include <nand.h>
#include "config.h"
#include "drivers/jtag/mem_rdwr.h"
#include "drivers/ft2232/ft2232.h"

extern int LoadImageFile(U8 *buf,int size);

void Nand_PrintBlock(void);
void Nand_Program(void);

extern u32 imageSize;

static U32 targetBlock;	    // Block number (0 ~ 4095)
static U32 targetSize;	    // Total byte size 
static U8 *blockBuf = NULL;

static const void *function[][2]=
{
    (void *)Nand_Program,			"Nand Flash Program     ",
    (void *)Nand_PrintBlock,		"Nand Flash Print BlkPage  ",
    (void *)1,			    		"Exit                ",
    0,0
};

void Nand_Menu(void)
{
    int i;

    printf("\n[NAND Flash JTAG Programmer]\n");
	if (nand_init() == 0)
	{
		printf("Can't detect NAND Flash\n");
		return;
	}


	i=0;
	while(1)
	{   //display menu
	    printf("%2d:%s",i,function[i][1]);
	    i++;
	    if((int)(function[i][0])==0)
	    {
		printf("\n");
		break;
	    }
	    if((i%4)==0)
		printf("\n");
	}

	printf("Select the function to test :");
	if (g_ReadOrWrite == -1)
		scanf("%d",&g_ReadOrWrite);
	else
		printf("%d\n\r", g_ReadOrWrite);
	if( g_ReadOrWrite>=0 && (g_ReadOrWrite<((sizeof(function)/8)-2)) ) 
	    ( (void (*)(void)) (function[g_ReadOrWrite][0]) )();  
}


void Nand_Program(void)
{
    int i;
    int programError=0;
    U32 blockIndex;
    int noLoad=0;
    U8 *srcPt;
    U32 progSize=0;
	U32 retlen;

	struct mtd_info *mtd = &nand_info[0];
	struct erase_info instr;

	memset(&instr, 0, sizeof(instr));

	if (!blockBuf)
	{
		blockBuf =(U8 *)malloc(mtd->erasesize);
	}

    printf("\n[NAND Flash Writing Program]\n");
    
    printf("\nSource size: 0x%x\n",imageSize);
    printf("\nAvailable target block number: 0~%d\n", mtd->size / mtd->erasesize - 1);
    printf("Input target block number:");
	if (g_Offset == -1)
	    scanf("%d",&targetBlock);
	else
	{
		targetBlock = g_Offset / mtd->erasesize;
		printf("%d\n\r", targetBlock);
	}
    targetSize=((imageSize+mtd->erasesize-1)/mtd->erasesize)*mtd->erasesize;
    printf("target start block number     =%d\n",targetBlock);
    printf("target size        (0x%x*%d) =0x%x\n",mtd->erasesize, targetSize/mtd->erasesize, targetSize);
    printf("STATUS:\n");
    blockIndex=targetBlock;
    while(1)
    {
		if(noLoad==0)
		{
		    LoadImageFile(blockBuf, mtd->erasesize);
		}
		noLoad=0;

		/* Erase this block */			  
		instr.addr = blockIndex * mtd->erasesize;
		instr.len = mtd->erasesize;
		if(mtd->erase(mtd, &instr))
		{
			printf("\nerase failed, block %d\n", blockIndex);
		    blockIndex++;   // for next block
		    noLoad=1;
		    continue;
		}

		fprintf(stderr, "E");
		srcPt=blockBuf;

		for (i=0; i < (int)(mtd->erasesize / mtd->oobblock); i++)
		{
			if(mtd->write(mtd, instr.addr+i*mtd->oobblock, mtd->oobblock, &retlen, srcPt))
		    {
				fprintf(stderr, "\nwrite failed, block %d, page %d\n", blockIndex, i);
		        programError=1;
		        break;
		    }

		    srcPt += mtd->oobblock;	// Increase buffer addr one pase size
		    fprintf(stderr, "p");
			progSize+=mtd->oobblock;
			if(progSize>=imageSize)
			{
				fprintf(stderr, "\n");
				return;	// Exit while loop
			}
		}
		fprintf(stderr, "\n");

        if(programError==1)
		{
		    blockIndex++;
		    noLoad=1;
		    programError=0;
		    continue;
		}
		blockIndex++;
    }
}

void Nand_PrintBlock(void)// Printf one page
{
    U32 i;
    U32 block,page;
	U32 retlen;
    static U8 *buffer = NULL;
	struct mtd_info *mtd = &nand_info[0];

	if (!buffer)
	{
		buffer =(U8 *)malloc(mtd->oobblock+mtd->oobsize);
	}

    printf("\n[NAND Flash block read]\n");	
    
    printf("Input target block number:");
	if (g_Offset == -1)
	    scanf("%d",&block);
	else
	{
		block = g_Offset / mtd->eccsize;
		printf("%d\n", block);
	}
    printf("Input target page number:");   
	if (g_Offset == -1)
	    scanf("%d",&page);
	else
	{
		page = (g_Offset % mtd->eccsize) / mtd->oobblock;
		printf("%d\n", page);
	}
    
	mtd->read(mtd, block*mtd->erasesize+page*mtd->oobblock, mtd->oobblock, &retlen, buffer);
	mtd->read_oob(mtd, block*mtd->erasesize+page*mtd->oobblock, mtd->oobsize, &retlen, buffer+mtd->oobblock);
		
    printf("block=%d, page=%d:",block,page);
    for (i = 0; i < mtd->oobblock; i++)
    {
        if(i%16==0)
	    printf("\n%3xh:",i);
        printf("%02x ",buffer[i]);
    }
    printf("\nS.A.:",i);

    for (i = mtd->oobblock; i< mtd->oobblock+mtd->oobsize; i++)
    {
        printf("%02x ",buffer[i]);
    }

    printf("\n");    	
}


