
obj-m := mod4_mmio.o
KDIR ?= /lib/modules/$(shell uname -r)/build

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(shell pwd) modules

install:
	$(MAKE) -C $(KDIR) SUBDIRS=$(shell pwd) modules_install

clean:
	$(RM) -f *.[oas] .*flags *.ko .*.cmd .*.d .*.tmp *.mod.c

