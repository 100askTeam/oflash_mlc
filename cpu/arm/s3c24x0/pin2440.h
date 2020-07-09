#ifndef __PIN2440_H__
#define __PIN2440_H__

#include "common/types.h"
/*****************************************************************************/
/* BSC Index Definition of S3C2440											 */
/* Refer to S3C2440_jtag_buf.bsd file                                        */ 
/* 2003/9/13 (Y.H.Lee)                                                                 */
/*****************************************************************************/

#define S2440_MAX_CELL_INDEX	471	//0~471

//DATA0 ~ 7 Boundary Scan Cell 
#define DATA0_7_CON	(416)
#define DATA0_IN	(402) 
#define DATA0_OUT	(401)

#define DATA1_IN	(404)  
#define DATA1_OUT	(403)

#define DATA2_IN	(406)  
#define DATA2_OUT	(405)

#define DATA3_IN	(408)  
#define DATA3_OUT	(407)

#define DATA4_IN	(410)  
#define DATA4_OUT	(409)

#define DATA5_IN	(412)  
#define DATA5_OUT	(411)

#define DATA6_IN	(414)  
#define DATA6_OUT	(413)

#define DATA7_IN	(417) 
#define DATA7_OUT	(415)  
//DATA8 ~ 15 Boundary Scan Cell 
#define DATA8_15_CON	(433)
#define DATA8_IN	(419) 
#define DATA8_OUT	(418)
#define DATA9_IN	(421)  
#define DATA9_OUT	(420)
#define DATA10_IN	(423)  
#define DATA10_OUT	(422)
#define DATA11_IN	(425)  
#define DATA11_OUT	(424)
#define DATA12_IN	(427)  
#define DATA12_OUT	(426)
#define DATA13_IN	(429)  
#define DATA13_OUT	(428)
#define DATA14_IN	(431)  
#define DATA14_OUT	(430)
#define DATA15_IN	(434) 
#define DATA15_OUT	(432)  
//DATA16 ~ 23 Boundary Scan Cell 
#define DATA16_23_CON	(450)
#define DATA16_IN	(436) 
#define DATA16_OUT	(435)
#define DATA17_IN	(438)  
#define DATA17_OUT	(437)
#define DATA18_IN	(440)  
#define DATA18_OUT	(439)
#define DATA19_IN	(442)  
#define DATA19_OUT	(441)
#define DATA20_IN	(444)  
#define DATA20_OUT	(443)
#define DATA21_IN	(446)  
#define DATA21_OUT	(445)
#define DATA22_IN	(448)  
#define DATA22_OUT	(447)
#define DATA23_IN	(451) 
#define DATA23_OUT	(449) 
//DATA24 ~ 31 Boundary Scan Cell 
#define DATA24_31_CON	(467)
#define DATA24_IN	(453) 
#define DATA24_OUT	(452)
#define DATA25_IN	(455)  
#define DATA25_OUT	(454)
#define DATA26_IN	(457)  
#define DATA26_OUT	(456)
#define DATA27_IN	(459)  
#define DATA27_OUT	(458)
#define DATA28_IN	(461)  
#define DATA28_OUT	(460)
#define DATA29_IN	(463)  
#define DATA29_OUT	(462)
#define DATA30_IN	(465)  
#define DATA30_OUT	(464)
#define DATA31_IN	(468) 
#define DATA31_OUT	(466)  

/*****************************/
//ADDR0 Boundary Scan Cell 
#define ADDR0_CON	(362)
#define ADDR0	        (361)
//ADDR1 ~ 15 Boundary Scan Cell 
#define ADDR1_15_CON	(378)
#define ADDR1	        (363)
#define ADDR2	        (364)
#define ADDR3	        (365)
#define ADDR4	        (366)
#define ADDR5	        (367)
#define ADDR6	        (368)
#define ADDR7	        (369)
#define ADDR8	        (370)
#define ADDR9	        (371)
#define ADDR10	        (372)
#define ADDR11	        (373)
#define ADDR12	        (374)
#define ADDR13	        (375)
#define ADDR14	        (376)
#define ADDR15	        (377)
//ADDR16 ~ 26 Boundary Scan Cell 
#define ADDR16_CON      (380)
#define ADDR16		(379)
#define ADDR17_CON      (382)
#define ADDR17		(381)
#define ADDR18_CON      (384)
#define ADDR18		(383)
#define ADDR19_CON      (386)
#define ADDR19		(385)
#define ADDR20_CON      (388)
#define ADDR20		(387)
#define ADDR21_CON      (390)
#define ADDR21		(389)
#define ADDR22_CON      (392)
#define ADDR22		(391)
#define ADDR23_CON      (394)
#define ADDR23		(393)
#define ADDR24_CON      (396)
#define ADDR24		(395)
#define ADDR25_CON      (398)
#define ADDR25		(397)
#define ADDR26_CON      (400)
#define ADDR26		(399)

#define CLE_CON		(332)//Added to 2440 BSCs
#define CLE		(331)
#define ALE_CON		(330)//Added to 2440 BSCs
#define ALE		(329)

#define nFCE_CON		(324)//Added to 2440 BSCs
#define nFCE		(323)
#define nFWE_CON		(328)//Added to 2440 BSCs
#define nFWE		(327)
#define nFRE_CON		(326)//Added to 2440 BSCs
#define nFRE		(325)

#define nWE		(351)
#define nOE		(352)
#define nBE0		(353)
#define nBE1		(354)
#define nBE2		(355)
#define nBE3		(356)
#define nSRAS_CAS_CON	(360)//Added to 2440 BSCs
#define nSRAS		(358)
#define nSCAS		(359)

#define NCON0		(51)
#define RnB			(322)//Added to 2440 BSCs    

#define nGCS67_CON	(360)
#define nGCS7		(335)
#define nGCS6		(336)
#define nGCS5_CON	(338)
#define nGCS5	        (337)
#define nGCS4_CON	(340)
#define nGCS4	        (339)
#define nGCS3_CON	(342)
#define nGCS3	        (341)
#define nGCS2_CON	(344)
#define nGCS2	        (343)
#define nGCS1_CON	(346)
#define nGCS1	        (345)
#define nGCS0_ETC_CON	(357)   //nGCS0,nWE,nOE,nBEn
#define nGCS0	        (347)


/*****************************************************************************/
/* Exported Functions                                                        */
/*****************************************************************************/
void S2440_InitCell(void);
void S2440_SetPin(int index, char value);
char S2440_GetPin(int index);

void S2440_SetAddr(u32 addr);

void S2440_SetDataByte(u8);
void S2440_SetDataHW(u16);
void S2440_SetDataWord(u32);

u8 S2440_GetDataByte(void);
u16 S2440_GetDataHW(void);
u32 S2440_GetDataWord(void);

extern char outCellValue[];
extern char inCellValue[];
extern int  dataOutCellIndex[32];
extern int  dataInCellIndex[32];
extern int  addrCellIndex[27];
extern u8 outCellValueBytes[];
extern u8 inCellValueBytes[];

extern struct cpu_bsd s3c2440_bsd;

//MACRO for speed up
//#define S2440_SetPin(index,value)   outCellValue[index] = value
//#define S2440_GetPin(index)	    inCellValue[index]

#endif  //__PIN2440_H__
