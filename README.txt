同时支持S3C2410、S3C2440、S3C6410、同时支持OpenJTAG、并口JTAG、支持所有Nor、Nand Flash的烧写工具oflash发布

windows版本：oflash.exe


linux版本：oflash


使用方法(注意：在Linux下前面oflash前要加上sudo)：
1. 直接运行oflash，按照提示选择CPU、NAND或NOR、读或写
2. oflash  <要烧写的文件>
3. 使用配置文件，建一个名为cfg.txt的文件，下面是一个例子：

# 说明："#"号开头表示注释
#       每个配置占一行，每行有6个参数，分别表示：JTAG类型、CPU类型、Flash类型、读写类型、偏移地址、文件
#       可以在配置文件里写多个配置用来把不同的文件写到不同的Flash、不同的偏移地址去
# jtag(0:openjtag, 1:并口) cpu(0:s3c2410, 1:s3c2440, 2:s3c6410)  flash(0:nand, 1:nor)  read/write(0:write, 1:read)   offset      file
    1                              2                     			  1                  0                           0           u-boot.bin
    1                              2                                  0                  0                           0           leds.bin
    
4. oflash -f <其他名字的配置文件>
5. 仿照配置文件的方式，把配置项直接放在oflash命令后面，比如：
oflash 0 2 1 0 0 leds_nor.bin   //通过OpenJTAG把leds_nor.bin烧到S3C2440的Nor flash上
oflash 1 2 0 0 0 leds_nand.bin  //通过OpenJTAG把leds_nand.bin烧到S3C2440的Nand flash上


#
# Amontec JTAGkey
#
# http://www.amontec.com/jtagkey.shtml
#

interface ft2232
#ft2232_device_desc "Amontec JTAGkey A"
ft2232_device_desc "USB<=>JTAG&RS232 A"
ft2232_layout jtagkey
ft2232_vid_pid 0x1457 0x5118



