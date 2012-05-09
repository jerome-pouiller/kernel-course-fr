#
# Licence: GPL
# Created: 2012-04-02 11:52:35+02:00
# Main authors:
#     - Jérôme Pouiller <jezz@sysmic.org>
#

obj-m := mod6_list.o

KDIR ?= /lib/modules/$(shell uname -r)/build

default: modules
install: modules_install

modules modules_install help clean distclean:
	$(MAKE) -C $(KDIR) M=$(shell pwd) $@

