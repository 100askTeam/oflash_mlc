#ifndef NAND_MENU_H
#define NAND_MENU_H

void Nand_Menu(void);
extern struct nandflash_controller s3c2410_nandflash_controller;
extern struct nandflash_controller s3c2440_nandflash_controller;
extern struct nandflash_controller s3c6410_nandflash_controller;
extern struct nandflash_controller *nandflash_controller;


#endif  //NAND_MENU_H

