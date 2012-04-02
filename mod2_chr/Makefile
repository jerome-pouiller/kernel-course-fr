#
# Licence: GPL
# Created: 2012-04-02 11:52:35+02:00
# Main authors:
#     - Jérôme Pouiller <jezz@sysmic.org>
#

obj-m := mod2_chr.o

KDIR ?= /lib/modules/$(shell uname -r)/build

default: modules ioctl_test
install: modules_install

modules modules_install help clean:
	$(MAKE) -C $(KDIR) M=$(shell pwd) $@

clean: ioctl_clean

ioctl_test: ioctl_test.o
	$(CC) $^ -o $@

ioctl_clean:
	rm -f *.o ioctl_test


