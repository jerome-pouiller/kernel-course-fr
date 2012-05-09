./ioctl_test /dev/test
mount -t debugfs none /sys/kernel/debug/
grep mod2 /sys/kernel/debug/dynamic_debug/control 
echo file mod2_chr.c +p > /sys/kernel/debug/dynamic_debug/control
grep mod2 /sys/kernel/debug/dynamic_debug/control 
./ioctl_test /dev/test
dmesg | tail
echo file mod2_chr.c -p > /sys/kernel/debug/dynamic_debug/control
grep mod2 /sys/kernel/debug/dynamic_debug/control 
./ioctl_test /dev/test
dmesg | tail


