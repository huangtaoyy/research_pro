/*
 * main.cpp
 *
 *  Created on: 2014-3-11
 *      Author: huangtao
 */
#include <fcntl.h>
#include <stdio.h>
#include "pad_data.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("usage : %s file", argv[0]);
	}

	int fd;
	if((fd = open(argv[1], O_RDWR, 0)) < 0)
		printf(" open %s failed " , argv [1]);

	PadRegion region;
	int size;

	size = get_allo_region_size(fd);
	copyFileToBuffer(fd, size);
	pad_data(fd, region, size);
	char* file = copyBufferToFile(argv[1]);
	printf("offset = %x, vaddr = %x, size = %x\n", region.pad_offset, region.pad_addr, region.pad_size);

	return 0;
}

