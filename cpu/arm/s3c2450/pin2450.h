#ifndef __PIN2450_H__
#define __PIN2450_H__

#include "common/types.h"

/*****************************************************************************/
/* BSC Index Definition of S3C2443											 */
/* Refer to S3C2443_jtag_buf.bsd file                                        */ 
/* 2006/9/9 (Junon.Jeon)                                                     */
/*****************************************************************************/


#define S2450_MAX_CELL_INDEX	684	//0~684


#define GPB5			(213)		//write protect pin for MiTAC
#define GPB5_CON		(214)		//write protect pin for MiTAC

// ROM bus RRADDR0 ~ 25 Boundary Scan Cell 
//#define RADDR1_16_CON	(416)
//#define RADDR17_25_CON	(416)

#define RADDR0				448
#define RADDR0_CON			449
#define RADDR1				450
#define RADDR1_CON			451
#define RADDR2				452
#define RADDR2_CON			453
#define RADDR3				454
#define RADDR3_CON			455
#define RADDR4				456
#define RADDR4_CON			457
#define RADDR5				458
#define RADDR5_CON			459
#define RADDR6				460
#define RADDR6_CON			461
#define RADDR7				462
#define RADDR7_CON			463
#define RADDR8				464
#define RADDR8_CON			465
#define RADDR9				466
#define RADDR9_CON			467
#define RADDR10				468
#define RADDR10_CON      	469
#define RADDR11				470
#define RADDR11_CON     	471
#define RADDR12				472
#define RADDR12_CON     	473
#define RADDR13				474
#define RADDR13_CON     	475
#define RADDR14				476
#define RADDR14_CON     	477
#define RADDR15				478
#define RADDR15_CON    		479
#define RADDR16				480
#define RADDR16_CON     	481
#define RADDR17				482
#define RADDR17_CON      	483
#define RADDR18				484
#define RADDR18_CON    		485
#define RADDR19				486
#define RADDR19_CON    		487
#define RADDR20				488
#define RADDR20_CON     	489
#define RADDR21				490
#define RADDR21_CON      	491
#define RADDR22				492
#define RADDR22_CON      	493
#define RADDR23				494
#define RADDR23_CON      	495
#define RADDR24				496
#define RADDR24_CON    		497
#define RADDR25				498
#define RADDR25_CON      	499

//RRDATA0 ~ 7 Boundary Scan Cell 
#define RDATA0_OUT			362
#define RDATA0_CON			363
#define RDATA0_IN			364
#define RDATA1_OUT			365
#define RDATA1_CON			366
#define RDATA1_IN			367
#define RDATA2_OUT			368
#define RDATA2_CON			369
#define RDATA2_IN			370
#define RDATA3_OUT			371
#define RDATA3_CON			372
#define RDATA3_IN			373
#define RDATA4_OUT			374
#define RDATA4_CON			375
#define RDATA4_IN			376
#define RDATA5_OUT			377
#define RDATA5_CON			378
#define RDATA5_IN			379
#define RDATA6_OUT			380
#define RDATA6_CON			381
#define RDATA6_IN			382
#define RDATA7_OUT			383
#define RDATA7_CON			384
#define RDATA7_IN			385
#define RDATA8_OUT			386
#define RDATA8_CON			387
#define RDATA8_IN			388
#define RDATA9_OUT			389
#define RDATA9_CON			390
#define RDATA9_IN			391
#define RDATA10_OUT			392
#define RDATA10_CON			393
#define RDATA10_IN			394
#define RDATA11_OUT			395
#define RDATA11_CON			396
#define RDATA11_IN			397
#define RDATA12_OUT			398
#define RDATA12_CON			399
#define RDATA12_IN			400
#define RDATA13_OUT			401
#define RDATA13_CON			402
#define RDATA13_IN			403
#define RDATA14_OUT			404
#define RDATA14_CON			405
#define RDATA14_IN			406
#define RDATA15_OUT			407
#define RDATA15_CON			408
#define RDATA15_IN			409

//#define RDATA0_7_CON	(416)
//#define RDATA8_15_CON	(433)
//#define RDATA16_23_CON	(450)
//#define RDATA24_31_CON	(467)


#define nWE				(440)
#define nWE_CON			(441)
#define nOE				(442)
#define nOE_CON			(443)
#define nBE0			(444)
#define nBE0_CON		(445) //
#define nBE1			(446)
#define nBE1_CON		(447) //
#define nWAIT			(421)
//#define nBE2		(355)
//#define nBE3		(356)

#define nRCS5	        (422)
#define nRCS5_CON		(423)
#define nRCS4	        (424)
#define nRCS4_CON		(425)
#define nRCS3	        (426)
#define nRCS3_CON		(427)
#define nRCS2	        (434)
#define nRCS2_CON		(435)
#define nRCS1	        (436)
#define nRCS1_CON		(437)
#define nRCS0	        (438)
#define nRCS0_CON		(439)
//#define nRCS67_CON	(360)
//#define nRCS7		(335)
//#define nRCS6		(336)
//#define nRCS0_ETC_CON	(357)

// OneNAND control pins
#define RSMBWAIT		(428)
#define RSMVAD			(429)
#define RSMVAD_CON		(430)
#define RSMCLK_OUT		(431)
#define RSMCLK_CON		(432)
#define RSMCLK_IN		(433)

// NAND control pins
#define RnB				(410)
#define nFCE			(411)
#define nFCE_CON		(412)
#define nFRE			(413)
#define nFRE_CON		(414)
#define nFWE			(415)
#define nFWE_CON		(416)
#define ALE				(417)
#define ALE_CON			(418)
#define CLE				(419)
#define CLE_CON			(420)
//#define NCON0			(51)


// SDRAM control pins
#define nSCAS			536
#define nSCAS_CON		537
#define nSRAS			538
#define nSRAS_CON		539
#define SCKE			540
#define SCKE_CON		541
#define nSCLK			542
#define nSCLK_CON		543
#define SCLK			544
#define SCLK_CON		545
#define SCLK_IN			546
#define nSWE			547
#define nSWE_CON		548
#define nSCS1			549
#define nSCS1_CON		550
#define nSCS0			551
#define nSCS0_CON		552
#define DQM0			553
#define DQM0_CON		554
#define DQM1			555
#define DQM1_CON		556
#define DQM2			557
#define DQM2_CON		558
#define DQM3			559
#define DQM3_CON		560
#define DQS0			561
#define DQS0_CON		562
#define DQS0_IN			563
#define DQS1			564
#define DQS1_CON		565
#define DQS1_IN			566

//#define nSRAS_CAS_CON	(360)//Added to 2443 BSCs

// SDRAM raddr bus
#define SADDR0			534
#define SADDR0_CON		535
#define SADDR1			532
#define SADDR1_CON		533
#define SADDR2			530
#define SADDR2_CON		531
#define SADDR3			528
#define SADDR3_CON		529
#define SADDR4			526
#define SADDR4_CON		527
#define SADDR5			524
#define SADDR5_CON		525
#define SADDR6			522
#define SADDR6_CON		523
#define SADDR7			520
#define SADDR7_CON		521
#define SADDR8			518
#define SADDR8_CON		519
#define SADDR9			516
#define SADDR9_CON		517
#define SADDR10			514
#define SADDR10_CON		515
#define SADDR11			512
#define SADDR11_CON		513
#define SADDR12			510
#define SADDR12_CON		511
#define SADDR13			508
#define SADDR13_CON		509
#define SADDR14			506
#define SADDR14_CON		507
#define SADDR15			504
#define SADDR15_CON		505


//SDATA0 ~ 31 Boundary Scan Cell   
#define SDATA0_OUT		567
#define SDATA0_CON		568
#define SDATA0_IN		569
#define SDATA1_OUT		570
#define SDATA1_CON		571
#define SDATA1_IN		572
#define SDATA2_OUT		573
#define SDATA2_CON		574
#define SDATA2_IN		575
#define SDATA3_OUT		576
#define SDATA3_CON		577
#define SDATA3_IN		578
#define SDATA4_OUT		579
#define SDATA4_CON		580
#define SDATA4_IN		581
#define SDATA5_OUT		582
#define SDATA5_CON		583
#define SDATA5_IN		584
#define SDATA6_OUT		585
#define SDATA6_CON		586
#define SDATA6_IN		587
#define SDATA7_OUT		588
#define SDATA7_CON		589
#define SDATA7_IN		590
#define SDATA8_OUT		591
#define SDATA8_CON		592
#define SDATA8_IN		593
#define SDATA9_OUT		594
#define SDATA9_CON		595
#define SDATA9_IN		596
#define SDATA10_OUT		597
#define SDATA10_CON		598
#define SDATA10_IN		599
#define SDATA11_OUT		600
#define SDATA11_CON		601
#define SDATA11_IN		602
#define SDATA12_OUT		603
#define SDATA12_CON		604
#define SDATA12_IN		605
#define SDATA13_OUT		606
#define SDATA13_CON		607
#define SDATA13_IN		608
#define SDATA14_OUT		609
#define SDATA14_CON		610
#define SDATA14_IN		611
#define SDATA15_OUT		612
#define SDATA15_CON		613
#define SDATA15_IN		614
#define SDATA16_OUT		615
#define SDATA16_CON		616
#define SDATA16_IN		617
#define SDATA17_OUT		618
#define SDATA17_CON		619
#define SDATA17_IN		620
#define SDATA18_OUT		621
#define SDATA18_CON		622
#define SDATA18_IN		623
#define SDATA19_OUT		624
#define SDATA19_CON		625
#define SDATA19_IN		626
#define SDATA20_OUT		627
#define SDATA20_CON		628
#define SDATA20_IN		629
#define SDATA21_OUT		630
#define SDATA21_CON		631
#define SDATA21_IN		632
#define SDATA22_OUT		633
#define SDATA22_CON		634
#define SDATA22_IN		635
#define SDATA23_OUT		636
#define SDATA23_CON		637
#define SDATA23_IN		638
#define SDATA24_OUT		639
#define SDATA24_CON		640
#define SDATA24_IN		641
#define SDATA25_OUT		642
#define SDATA25_CON		643
#define SDATA25_IN		644
#define SDATA26_OUT		645
#define SDATA26_CON		646
#define SDATA26_IN		647
#define SDATA27_OUT		648
#define SDATA27_CON		649
#define SDATA27_IN		650
#define SDATA28_OUT		651
#define SDATA28_CON		652
#define SDATA28_IN		653
#define SDATA29_OUT		654
#define SDATA29_CON		655
#define SDATA29_IN		656
#define SDATA30_OUT		657
#define SDATA30_CON		658
#define SDATA30_IN		659
#define SDATA31_OUT		660
#define SDATA31_CON		661
#define SDATA31_IN		662

    
/*****************************************************************************/      
/* Exported Functions                                                        */      
/*****************************************************************************/      
void S2450_InitCell(void);                                                           
void S2450_SetPin(int index, char value);                                            
char S2450_GetPin(int index);                                                        
                                                                                     
void S2450_SetRAddr(U32 raddr);                                                        
void S2450_SetSAddr(U32 raddr);                                                        

void S2450_ContRDataBus(U8 cmd);
void S2450_ContSDataBus(U8 cmd);
void S2450_ContRAddrBus(U8 cmd); // RAddr[25:0] HIGH=output
                                                                                     
void S2450_SetRDataByte(U8);                                                          
void S2450_SetRDataHW(U16);                                                           
void S2450_SetRDataWord(U32);                                                         
                                                                                     
U8 S2450_GetRDataByte(void);                                                          
U16 S2450_GetRDataHW(void);                                                           
U32 S2450_GetRDataWord(void);                                                         
                                                                                     
extern char outCellValue[S2450_MAX_CELL_INDEX+2];                                    
extern char inCellValue[S2450_MAX_CELL_INDEX+2];                                     
extern int  rdataOutCellIndex[16];                                                    
extern int  rdataInCellIndex[16];                                                     
extern int  raddrCellIndex[26];                                                       
                                                                                     
//MACRO for speed up                                                                 
//#define S2450_SetPin(index,value)   outCellValue[index] = value                    
//#define S2450_GetPin(index)	    inCellValue[index]                                 
                                                                                     
#endif  //__PIN2450_H__                                                              
                                                                                     
                                               
