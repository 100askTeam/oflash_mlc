# 前言
根据OpenJTAG的使用文档，是先把一个特制的u-boot下载到开发板的内存去运行；<br>
然后通过u-boot来下载、烧写Nor、Nand Flash。<br>
这种方法的烧写速度极快，但是需要认真阅读文档，并稍微熟悉u-boot的使用。<br>
对于初学者，可能更喜欢一拿来就用。<br>


# oflash 
同时支持S3C2410、S3C2440、S3C6410、同时支持OpenJTAG、并口JTAG、支持所有Nor、Nand Flash的烧写工具oflash发布<br>

* windows版本：oflash.exe
* linux版本：oflash


使用方法(注意：在Linux下前面oflash前要加上sudo)：<br>
* 1. 直接运行oflash，按照提示选择CPU、NAND或NOR、读或写
* 2. oflash  <要烧写的文件>
* 3. 使用配置文件，建一个名为cfg.txt的文件，下面是一个例子：

> 每个配置占一行，每行有6个参数，分别表示：JTAG类型、CPU类型、Flash类型、读写类型、偏移地址、文件，可以在配置文件里写多个配置用来把不同的文件写到不同的Flash、不同的偏移地址去<br>
| jtag(0:openjtag, 1:并口)  | cpu(0:s3c2410, 1:s3c2440, 2:s3c6410)  | flash(0:nand, 1:nor)  | read/write(0:write, 1:read)   | offset      | file
| :----: | :----: | :----: | :----: | :----: | :----: |
|    1   | 2      | 1      | 0      | 0      | u-boot.bin |
|    1   | 2      | 0      | 0      | 0      | leds.bin |

* 4. oflash -f <其他名字的配置文件>
* 5. 仿照配置文件的方式，把配置项直接放在oflash命令后面，比如：

```bash
oflash 0 2 1 0 0 leds_nor.bin   //通过OpenJTAG把leds_nor.bin烧到S3C2440的Nor flash上
oflash 1 2 0 0 0 leds_nand.bin  //通过OpenJTAG把leds_nand.bin烧到S3C2440的Nand flash上
```


# sjf24x0_ft2232
在春节期间，我仿照sjf24x0.exe编写了sjf24x0_ft2232.exe。<br>
sjf24x0.exe只支持并口JTAG，所能烧写的Nor、Nand Flash只有1、2种。<br>

## sjf24x0_ft2232的功能
sjf24x0_ft2232.exe添加了更多功能：<br>
* 1. 可以支持S3C2410、S3C2440
* 2. 可以烧写各类NOR、NAND FLASH
* 3. 速度比并口JTAG工具快：在S3C2440上测试，烧写NOR Flash速度是1KB/S；烧NAND Flash速度是1.9KB/S，是并口的6倍

使用sjf24x0.exe通过并口烧写一个200K的程序，烧NOR Flash需要20分钟，烧Nand Flash需要10分钟；<br>
而同样的程序使用sjf24x0_ft2232.exe通过OpenJTAG来烧写，烧NOR Flash需要3分钟，烧Nand Flash不到2分钟。<br>

下一步是改进程序，使用传说中的“DCC”方法，应该可以大辐提高烧写速度。<br>


## 工作原理
并口JTAG的数据传送方法是：拉低TCK，设置TDI，拉高TCK，读取TDO，……。<br>
另一方面，USB协议限制了全速设备的发包间隙是1ms，高速设备的发包协议是125us。<br>

从上面两点来看，提高USB JTAG的速率不外乎两种方法：<br>
* 1. PC端发出JTAG命令时，先尽可能多地缓冲，然后一次发出
* 2. 接收USB信号端，使用很高的TCK进行数据JTAG读写

FT2232D本身具备几百字节的buffer，TCK的最大频率是6M。从这点看，使用FT2232应该比并口有先天的优势。<br>
实际上，我自己写了一个S3C2410/S3C2440的烧写程序，烧写NOR Flash速度是1KB/S；烧NAND Flash速度是1.9KB/S，是并口的6倍。<br>
使用并口烧写一个200K的程序，烧NOR Flash需要20分钟，烧Nand Flash需要10分钟；<br>
同样的程序使用FT2232来烧写，烧NOR Flash需要3分钟，烧Nand Flash不到2分钟。<br>

但是，h-jtag使用并口烧写速度也能达到10、20KB/S，原因在于它使用了DCC功能。<br>
我想，在FT2232的基础上，也通过DCC方法来烧写，速率应该还有几十倍的提高。<br>

可惜，DCC是啥，至今没找到比较好资料。<br>

```bash
size: 194700
NAND: 11.24.20~11.28.58 240+38=278s=  700B/S
		  11.48.50~11.53.36 346s
		  
		  diable optimization: 11.55.00~11.56.42 102S 1.9K/S

      Debug: 11.40.100~11.41.51 111s = 1.8 KB/S
      

NOR: 11.43:00 ~ 11.46.14 180+14=194 1KB/S

diable optimization: 11.57.40~12.00.44 184S 1KB/S
```

并口：<br>
```bash
NAND：12.04.40~12.14.40 600S 325B/S
NOR： 12.16.00~12.36.30 1230S 158B/s
```


上传一个基于FT2232的仿sjf2410.exe的S3C2410、S3C2440烧写程序： <br>

sjf24x0.exe只支持并口JTAG，所能烧写的Nor、Nand Flash只有1、2种。 <br>
sjf24x0_ft2232.exe添加了更多功能： <br>
* 1. 可以支持S3C2410、S3C2440 
* 2. 可以烧写各类NOR、NAND FLASH 
* 3. 速度比并口JTAG工具快：在S3C2440上测试，烧写NOR Flash速度是1KB/S；烧NAND Flash速度是1.9KB/S，是并口的6倍 

使用sjf24x0.exe通过并口烧写一个200K的程序，烧NOR Flash需要20分钟，烧Nand Flash需要10分钟； <br>
而同样的程序使用sjf24x0_ft2232.exe通过OpenJTAG来烧写，烧NOR Flash需要3分钟，烧Nand Flash不到2分钟。 <br>

sjf24x0_ft2232.exe的操作界面如下： <br>
<center class="half">
   <img src="http://b12.photo.store.qq.com/http_imgload.cgi?/rurl4_b=5ad81df6a61831a7953b1ea373f8d488c5d8bc76d06e5880a45af64e2bdb86684ca330ace77405d59ac5b591f072ae3ea1cab65430320a1c551853136290b07ae4a9846ca663606371920c0a3abc5de57bd29c21" />
</center>


OpenJTAG的外观如下： <br>
<center class="half">
   <img src="http://b12.photo.store.qq.com/http_imgload.cgi?/rurl4_b=5ad81df6a61831a7953b1ea373f8d48886f2a69ff3175f1c3912445636568ea9731a5308ffd0445f59dff8b5f53cf0105461588e6d8ebb777eafd775322f19ad9ab4caa663a6cf02d569447a32be5807c27de505" />
</center>

调试点LED的程序界面如下： <br>
<center class="half">
   <img src="http://b10.photo.store.qq.com/http_imgload.cgi?/rurl4_b=5ad81df6a61831a7953b1ea373f8d48885a49543c2ab83771eb477fbc6f5bd6eb00aff0fda3f44e74945fa3adca91d28fd3d7fd030812e3554775415ce8181eaf85b532d242a23dbff6d6761a3eea96d44e359c4" />
</center>

调试u-boot的界面如下： <br>
<center class="half">
   <img src="http://b11.photo.store.qq.com/http_imgload.cgi?/rurl4_b=5ad81df6a61831a7953b1ea373f8d4888e70e52c9313e4b93d60754407fd4a9319db3d461a0cfd43bbfa0cc10b698b2c4297307fb63eea79e506a030657a820bee9edd4224c46e430c8bd9a08f46686c0b9065f8" />
</center>

调试Linux内核的界面如下： <br>
<center class="half">
   <img src="http://b10.photo.store.qq.com/http_imgload.cgi?/rurl4_b=5ad81df6a61831a7953b1ea373f8d488967e5831f6ce922d853c75a9ee77d595c16c9d3ba07abba5dc688abd77577ab2dcbe69fb9fd9d165e0d8bbb38fb76cfd1f1788bb416669a3c8e4a1a23129bfdd2e212281" />
</center>


淘宝地址为：[http://item.taobao.com/auction/item_detail-0db1-47aeeccb54e9c1c3889a53276d52d7f1.jhtml](http://item.taobao.com/auction/item_detail-0db1-47aeeccb54e9c1c3889a53276d52d7f1.jhtml)



由于结构原因，TST、ERASE引脚只能同时通过跳线拉高。<br>
AT91SAM7S64进入TST模式后，将会烧写内部的BOOT程序；<br>
而ERASE引脚拉高时，却会擦除。<br>
不知它们的优先级谁高？<br>

我想达到：这两个引脚同时拉高时是擦除，不知能否实现<br>

请问AT91SAM7S64的TST、ERASE引脚同时拉高会发生什么情况？是烧写内部的BOOT程序，还是擦除？<br>

```bash
Nor/Nand flash programmer based FT2232 for S3C2410 & S3C2440

With openocd, it seems to be inefficient  to program Nor/Nand flash for S3C2410 and S3C2440.
I have wrote a programmer for that. But it does not use the DCC, so the speed is just 6 times to wiggler.

I called this programmer sjf24x0_ft2232, because it is based sjf2410 from samsung.

It's features compared to sjf2410 are:
1. support both s3c2410 and s3c2440.
2. support CFI/JEDEC Nor flash, as sjf2410 supports only AM29LV800.
3. support many Nand flash(512 bytes/page), as sjf2410 supports only K9F1208.
4. 6 time faster than sjf2410.

The test result is below:
1. for Nor flash: the programmer speed is 1KB/S
2. for Nand flash: the programmer speed is 1.9KB/S

For a 200KB file, by wiggler and sjf2410,  the time of programming is: 20 minutes for Nor, 10 minutes for Nand.

By FT2232 and sjf24x0_ft2232, the time is: 3 minutes for Nor, less than 2 minutes for Nand.

If somebody can give me some tutorial of DCC, I think I can update it for faster speed.

Sjf24x0_ft2232 can be download from: http://www.100ask.net/bbs/attachment.aspx?attachmentid=301

The picture of Sjf24x0_ft2232 is: http://www.100ask.net/bbs/attachment.aspx?attachmentid=300

If you have an FT2232 based USB to JTAG tool ,you should change the product description to "USB<=>JTAG&RS232" by Mprog(It is a tool to change the EEPROM of FT2232).

The sjf24x0_ft2232 is still developing, any advisement is welcom.
My MSN/Email is : thisway.diy@163.com
```


# 关注我们
- 百问网官方wiki：[http://wiki.100ask.org](http://wiki.100ask.org)
- 百问网官方论坛：[http://bbs.100ask.net](http://bbs.100ask.net)
- 百问网官网：[http://www.100ask.net](http://www.100ask.net)
- 微信公众号：百问科技
- CSDN：[韦东山](https://edu.csdn.net/lecturer/90)
- B站：[韦东山](https://space.bilibili.com/275908810?from=search&seid=10505231074028238949)
- 知乎：[韦东山嵌入式](https://www.zhihu.com/people/www.100ask/)
- 微博：[百问科技](https://weibo.com/888wds?topnav=1&wvr=6)
- 电子发烧友学院：[韦东山](http://t.elecfans.com/teacher/3.html)


微信公众号  |  视频教程在线学习平台
<center class="half">
   <img src="http://photos.100ask.net//ELADCMSecond/aboutus/followus.png" width="800"/>
</center>


