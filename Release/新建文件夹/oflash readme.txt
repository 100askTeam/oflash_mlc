同时支持S3C2410、S3C2440、同时支持OpenJTAG、并口JTAG、支持所有Nor、Nand Flash的烧写工具oflash发布

windows版本：oflash.exe
把FTD2XX.dll放到C:\WINDOWS\system32目录下，
或是放在oflash.exe同一个目录下。

linux版本：oflash


使用方法(注意：在Linux下前面oflash前要加上sudo)：
1. 直接运行oflash，按照提示选择JTAG、CPU、NAND或NOR、读或写
2. oflash  <要烧写的文件>

对于S3C6410，要使用oflash时需要把DBGSEL引脚设为高电平，设置方法需要参考开发板使用手册。