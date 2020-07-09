
MAKE	:= make
#MAKE	:= make --no-print-directory

INCLUDEDIR 	:= $(shell pwd)/include
TOPDIR	:= $(shell pwd)

CROSSCOMPILE := 

#CFLAGS 	:= -Wall -Wstrict-prototypes -ffreestanding -c
CFLAGS 	:= -Wall -O2 -fomit-frame-pointer -ffreestanding -c
CPPFLAGS   	:= -DBUILD_FT2232_LIBFTDI=1 -I$(INCLUDEDIR) -I$(TOPDIR)
LDFLAGS 	:= 

CC 	:= $(CROSSCOMPILE)gcc
CPP 	:= $(CROSSCOMPILE)g++
LD 	:= $(CROSSCOMPILE)ld
STRIP 	:= $(CROSSCOMPILE)strip
OBJCOPY 	:= $(CROSSCOMPILE)objcopy
OBJDUMP 	:= $(CROSSCOMPILE)objdump

export 	MAKE INCLUDEDIR TOPDIR \
	CROSS_COMPILE CFLAGS CPPFLAGS LDFLAGS CC CPP LD OBJCOPY OBJDUMP

ASM_FILES	:= 

C_FILES 	:=

CPP_FILES 	:= oflash.cpp config.cpp stdafx.cpp

SUBDIRS 	:= common/ drivers/ cpu/

OBJFILES := $(patsubst %.S,%.o,$(ASM_FILES)) $(patsubst %.c,%.o,$(C_FILES)) $(patsubst %.cpp,%.o,$(CPP_FILES))

TOPOBJFILES 	:= $(patsubst %/,%/built-in.o,$(SUBDIRS)) $(OBJFILES)

DESTFILE := oflash
$(DESTFILE) : $(TOPOBJFILES)
	$(MAKE) dep
	$(MAKE) subdirs
	$(CPP) $(TOPOBJFILES) lib/libftdi.a lib/libusb.a -o $@ -static
	$(STRIP) $@
clean:
	$(MAKE) subclean
	rm -f $(DESTFILE)

distclean:
	$(MAKE) subdistclean;
	rm -f $(DESTFILE)
	
FORCE:

include $(TOPDIR)/Rules.make



CROSS_COMPILE = 
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm

STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

export AS LD CC CPP AR NM
export STRIP OBJCOPY OBJDUMP

CFLAGS := -Wall -Werror -O2 -g
CFLAGS += -I $(shell pwd)/include

LDFLAGS := lib/libftdi.a lib/libusb.a -static

export CFLAGS LDFLAGS

TOPDIR := $(shell pwd)
export TOPDIR

TARGET := oflash

obj-y += oflash.o
obj-y += config.o
obj-y += stdafx.o
obj-y += common/
obj-y += drivers/
obj-y += cpu/

all : 
	make -C ./ -f $(TOPDIR)/Makefile.build
	$(CC) $(LDFLAGS) -o $(TARGET) built-in.o


clean:
	rm -f $(shell find -name "*.o")
	rm -f $(TARGET)

distclean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(TARGET)
	