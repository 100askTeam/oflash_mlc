#ifndef __PIN6410_H__
#define __PIN6410_H__

#include "common/types.h"

/*************************************************/
/* BSC Index Definition of S3C6410					 */
/* Refer to S3C6410X01_EVT1.bsdl file                             */ 
/* 2008/6/26 (H.M Noh)                                                  */
/*************************************************/

#define S6410_MAX_CELL_INDEX	789	//0~789..EVT1
//#define S6410_MAX_CELL_INDEX	791	//0~797...EVT0

// Xm0ADDR0 ~ 15 Boundary Scan Cell 
#define Xm0ADDR15_OUT		(142)
#define Xm0ADDR15_CON		(143)
#define Xm0ADDR15_IN		(144)
#define Xm0ADDR14_OUT		(145)
#define Xm0ADDR14_CON		(146)
#define Xm0ADDR14_IN		(147)
#define Xm0ADDR13_OUT		(148)
#define Xm0ADDR13_CON		(149)
#define Xm0ADDR13_IN		(150)
#define Xm0ADDR12_OUT		(151)
#define Xm0ADDR12_CON		(152)
#define Xm0ADDR12_IN		(153)
#define Xm0ADDR11_OUT		(154)
#define Xm0ADDR11_CON		(155)
#define Xm0ADDR11_IN		(156)
#define Xm0ADDR10_OUT		(157)
#define Xm0ADDR10_CON		(158)
#define Xm0ADDR10_IN		(159)
#define Xm0ADDR9_OUT		(160)
#define Xm0ADDR9_CON		(161)
#define Xm0ADDR9_IN		(162)
#define Xm0ADDR8_OUT		(163)
#define Xm0ADDR8_CON		(164)
#define Xm0ADDR8_IN		(165)
#define Xm0ADDR7_OUT		(166)
#define Xm0ADDR7_CON		(167)
#define Xm0ADDR7_IN		(168)
#define Xm0ADDR6_OUT		(169)
#define Xm0ADDR6_CON		(170)
#define Xm0ADDR6_IN		(171)
#define Xm0ADDR5_OUT		(172)
#define Xm0ADDR5_CON		(173)
#define Xm0ADDR5_IN		(174)
#define Xm0ADDR4_OUT		(175)
#define Xm0ADDR4_CON		(176)
#define Xm0ADDR4_IN		(177)
#define Xm0ADDR3_OUT		(178)
#define Xm0ADDR3_CON		(179)
#define Xm0ADDR3_IN		(180)
#define Xm0ADDR2_OUT		(181)
#define Xm0ADDR2_CON		(182)
#define Xm0ADDR2_IN		(183)
#define Xm0ADDR1_OUT		(184)
#define Xm0ADDR1_CON		(185)
#define Xm0ADDR1_IN		(186)
#define Xm0ADDR0_OUT		(187)
#define Xm0ADDR0_CON		(188)
#define Xm0ADDR0_IN		(189)


// Xm0DATA0 ~ 15 Boundary Scan Cell 
#define Xm0DATA15_OUT		(134)
#define Xm0DATA15_CON		(135) //
#define Xm0DATA15_IN		(136)
#define Xm0DATA14_OUT		(137)
#define Xm0DATA14_CON		(138) //
#define Xm0DATA14_IN		(139)
#define Xm0DATA13_OUT		(129)
#define Xm0DATA13_CON		(130) //
#define Xm0DATA13_IN		(131)
#define Xm0DATA12_OUT		(120)
#define Xm0DATA12_CON		(121) //
#define Xm0DATA12_IN		(122)
#define Xm0DATA11_OUT		(123)
#define Xm0DATA11_CON		(124) //
#define Xm0DATA11_IN		(125)
#define Xm0DATA10_OUT		(126)
#define Xm0DATA10_CON		(127) //
#define Xm0DATA10_IN		(128)
#define Xm0DATA9_OUT		(114)
#define Xm0DATA9_CON		(115) //
#define Xm0DATA9_IN		(116)
#define Xm0DATA8_OUT		(117)
#define Xm0DATA8_CON		(118) //
#define Xm0DATA8_IN		(119)
#define Xm0DATA7_OUT		(90)
#define Xm0DATA7_CON		(91) //
#define Xm0DATA7_IN		(92)
#define Xm0DATA6_OUT		(93)
#define Xm0DATA6_CON		(94) //
#define Xm0DATA6_IN		(95)
#define Xm0DATA5_OUT		(96)
#define Xm0DATA5_CON		(97) //
#define Xm0DATA5_IN		(98)
#define Xm0DATA4_OUT		(99)
#define Xm0DATA4_CON		(100) //
#define Xm0DATA4_IN		(101)
#define Xm0DATA3_OUT		(102)
#define Xm0DATA3_CON		(103) //
#define Xm0DATA3_IN		(104)
#define Xm0DATA2_OUT		(105)
#define Xm0DATA2_CON		(106) //
#define Xm0DATA2_IN		(107)
#define Xm0DATA1_OUT		(108)
#define Xm0DATA1_CON		(109) //
#define Xm0DATA1_IN		(110)
#define Xm0DATA0_OUT		(111)
#define Xm0DATA0_CON		(112) //
#define Xm0DATA0_IN		(113)

#define Xm0DQM0_nBE0		(0)		//6410. Not assign -> temp. assign
#define Xm0DQM0_nBE0_CON	(1)		//6410. Not assign -> temp. assign
#define Xm0DQM1_nBE1		(9)		//6410. Not assign -> temp. assign
#define Xm0DQM1_nBE1_CON	(10)		//6410. Not assign -> temp. assign

//to control the data buffer direction of 6410 SMDK.
#define Xm0OEata_OUT		(3)		//6410. Not assign -> temp. assign
#define Xm0OEata_CON		(4)
#define Xm0OEata_IN			(5)

#define Xm0WEn				(140)
#define Xm0WEn_CON			(141)
#define Xm0OEn				(132)
#define Xm0OEn_CON			(133)

#define Xm0WAITn_OUT		(60)
#define Xm0WAITn_CON		(61)
#define Xm0WAITn_IN		(62)

#define Xm0CSn7_OUT	    	(66)
#define Xm0CSn7_CON		(67)
#define Xm0CSn7_IN			(68)
#define Xm0CSn6_OUT	   	(69)
#define Xm0CSn6_CON		(70)
#define Xm0CSn6_IN			(71)
#define Xm0CSn5_OUT	    	(72)
#define Xm0CSn5_CON		(73)
#define Xm0CSn5_IN			(74)
#define Xm0CSn4_OUT	    	(75)
#define Xm0CSn4_CON		(76)
#define Xm0CSn4_IN			(77)
#define Xm0CSn3_OUT	    	(78)
#define Xm0CSn3_CON		(79)
#define Xm0CSn3_IN			(80)
#define Xm0CSn2_OUT	    	(81)
#define Xm0CSn2_CON		(82)
#define Xm0CSn2_IN			(83)
#define Xm0CSn1_OUT	    	(84)
#define Xm0CSn1_CON		(85)
#define Xm0CSn1_IN			(86)
#define Xm0CSn0_OUT	    	(87)
#define Xm0CSn0_CON		(88)
#define Xm0CSn0_IN			(89)


// NAND control pins
#define Xm0RPn_RnB_OUT				(21)
#define Xm0RPn_RnB_CON				(22)
#define Xm0RPn_RnB_IN				(23)
#define Xm0INTsm1_FREn_OUT		(27)
#define Xm0INTsm1_FREn_CON		(28)
#define Xm0INTsm1_FREn_IN			(29)
#define Xm0INTsm0_FWEn_OUT		(30)
#define Xm0INTsm0_FWEn_CON		(31)
#define Xm0INTsm0_FWEn_IN			(32)
#define Xm0RDY0_ALE_OUT			(36)
#define Xm0RDY0_ALE_CON			(37)
#define Xm0RDY0_ALE_IN				(38)
#define Xm0RDY1_CLE_OUT			(33)
#define Xm0RDY1_CLE_CON			(34)
#define Xm0RDY1_CLE_IN				(35)

// OneNand control pins....rb1004
#define Xm0ADRVALIDn_OUT			(24)
#define Xm0ADRVALIDn_CON			(25)
#define Xm0ADRVALIDn_IN			(26)

#define XnRSTOUT_OUT			(787)
#define XnRSTOUT_CON			(788)
#define XnRSTOUT_IN				(789)

#define XEINT0_OUT				(784)
#define XEINT0_CON				(785)
#define XEINT0_IN				(786)
#define XEINT1_OUT				(781)
#define XEINT1_CON				(782)
#define XEINT1_IN				(783)
#define XEINT2_OUT				(778)
#define XEINT2_CON				(779)
#define XEINT2_IN				(780)


/*****************************************************************************/      
/* Exported Functions                                                        */      
/*****************************************************************************/      
void S6410_InitCell(void);                                                           
void S6410_SetPin(int index, char value);                                            
char S6410_GetPin(int index);                                                        
                                                                                     
void S6410_SetRAddr(U32 raddr);                                                        
void S6410_SetSAddr(U32 raddr);                                                        

void S6410_ContRDataBus(U8 cmd);
void S6410_ContSDataBus(U8 cmd);
void S6410_ContRAddrBus(U8 cmd); // RAddr[25:0] LOW=output
                                                                                     
void S6410_SetRDataByte(U8);                                                          
void S6410_SetRDataHW(U16);                                                           
void S6410_SetRDataWord(U32);                                                         
                                                                                     
U8 S6410_GetRDataByte(void);                                                          
U16 S6410_GetRDataHW(void);                                                           
U32 S6410_GetRDataWord(void);                                                         
                                                                                     
extern char outCellValue[S6410_MAX_CELL_INDEX+2];                                    
extern char inCellValue[S6410_MAX_CELL_INDEX+2];                                     
//extern int  rdataOutCellIndex[16];                                                    
//extern int  rdataInCellIndex[16];                                                     
//extern int  raddrCellIndex[26];        
//extern int  raddrCellIndex[16];                                                
                                                                                     
//MACRO for speed up                                                                 
//#define S6410_SetPin(index,value)   outCellValue[index] = value                    
//#define S6410_GetPin(index)	    inCellValue[index]                                 
                                                                                     
#endif  //__PIN6410_H__                                                              
                                                                                     
                                                                                     
