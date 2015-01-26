/*
 * main.cpp
 *
 *  Created on: 2013-12-19
 *      Author: huangtao
 */
#include "append_func.h"
#include <fcntl.h>


int main(int argc, char** argv) {
	if (argc != 2) {
		printf("usage : %s file\n", argv[0]);
		exit(1);
	}

	int fd;
	if((fd = open(argv[1], O_RDWR, 0)) < 0)
		err ( EXIT_FAILURE , " open %s failed " , argv [1]);

	int size = get_allo_region_size(fd);

	Elf32_Phdr n_phdr;
	add_segment(fd, n_phdr, size);
	AlloRegion allo_region(n_phdr.p_offset, n_phdr.p_vaddr, n_phdr.p_filesz );
	printf("offset = %x, vaddr = %x, size = %x \n", n_phdr.p_offset, n_phdr.p_vaddr, n_phdr.p_filesz );
	/*
	 * do something below
	 */

	/*
	 * do something above
	 */

	close(fd);

	return 0;
}
