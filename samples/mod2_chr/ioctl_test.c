/*
 * Licence: GPL
 * Created: 2012-04-02 08:08:34+02:00
 * Main authors:
 *     - Jérôme Pouiller <jezz@sysmic.org>
 */
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "mod2_chr.h"


int main(int argc, char **argv) {
	int ret, arg = 42;
	int fd = open(argv[1], O_RDONLY);
	ret = ioctl(fd, FIFO_GET_LEN, &arg);
	printf("Returned %m with argument %d\n", -ret, arg);
	return 0;
}

