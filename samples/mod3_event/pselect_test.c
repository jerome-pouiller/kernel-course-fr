/**
 * Licence: GPL
 * Created: 2012-8-1 17:09:18
 * Main authors:
 *     - Jérôme Pouiller <j.pouiller@sysmic.fr>
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

int main(int argc, char **argv) {
	fd_set Rfds;
	int    ret;
	int    fd;
	char buf[255];

	if (argc != 2)
		return 1;
	printf("Try to open %s\n", argv[1]);
	fd = open(argv[1], O_RDWR);
	if (fd < 0)
		return 2;
	FD_ZERO(&Rfds);
	FD_SET(fd, &Rfds);
	printf("Waiting for data on %s (fd = %d)\n", argv[1], fd);
	ret = pselect(fd + 1, &Rfds, NULL, NULL, NULL, NULL);
	if (ret != 1 || !FD_ISSET(fd, &Rfds))
		return 3;
	printf("Read data\n");
	ret = read(fd, buf, sizeof(buf));
	printf("Read %d bytes\n", ret);

	return 0;
}