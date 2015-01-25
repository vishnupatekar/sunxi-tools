/*
 * Copyright (C) 2012  Floris Bos <bos@je-eigen-domein.nl>
 * Copyright (c) 2014  Luc Verhaegen <libv@skynet.be>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <errno.h>
#include <sys/io.h>
#include <stdbool.h>

typedef uint32_t u32;

#define DEVMEM_FILE "/dev/mem"
static int devmem_fd;

/*
 * Find out exactly which SoC we are dealing with.
 */
#define SUNXI_SCRIPT_BIN_BASE	0x43000000 
#define SUNXI_SCRIPT_BIN_SIZE	0x00020000


void read_script_bin(void)
{
	void *src;
	void *dst;
	int fdout;

	printf (" read_script_bin @\n");
	
	 if ((fdout = open ("script.bin", O_RDWR | O_CREAT)) < 0)
		  printf ("can't create script.bin for writing");

	src = mmap(NULL, SUNXI_SCRIPT_BIN_SIZE, PROT_READ|PROT_WRITE,
		    MAP_SHARED, devmem_fd, SUNXI_SCRIPT_BIN_BASE);
	if (src == MAP_FAILED) {
		fprintf(stderr, "Failed to map SCRIPT.BIN registers: %s\n",
			strerror(errno));
	}

	 if ((dst = mmap (NULL, SUNXI_SCRIPT_BIN_SIZE, PROT_READ | PROT_WRITE,MAP_SHARED, fdout, 0)))
		printf ("mmap error for output");

	/* this copies the input file to the output file */
 	memcpy(dst, src, SUNXI_SCRIPT_BIN_SIZE);
	
	munmap(src, SUNXI_SCRIPT_BIN_SIZE);
	munmap(dst, SUNXI_SCRIPT_BIN_SIZE);
}

int
main(int argc, char *argv[])
{
	devmem_fd = open(DEVMEM_FILE, O_RDWR);
	if (devmem_fd == -1) {
		fprintf(stderr, "Error: failed to open %s: %s\n", DEVMEM_FILE,
			strerror(errno));
		return errno;
	}

	read_script_bin(); 
	return 0;
}
