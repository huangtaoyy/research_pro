/*
 * pad_data.h
 *
 *  Created on: 2014-3-11
 *      Author: huangtao
 */

#include <elf.h>

struct PadRegion {
	Elf32_Addr pad_addr;
	Elf32_Off pad_offset;
	Elf32_Word pad_size;

	PadRegion(){
	}

	PadRegion(Elf32_Addr pad_addr, Elf32_Off pad_offset, Elf32_Word pad_size) {
		this->pad_offset = pad_addr;
		this->pad_offset = pad_offset;
		this->pad_size = pad_size;
	}
};

void pad_data(int fd, PadRegion& region, int size);
int get_allo_region_size(int fd);
void modify_program_header(int fd, Elf32_Ehdr& ehdr, PadRegion& region, int size);
void copyFileToBuffer(int fd, int allo_size);
char* copyBufferToFile(char* file_name);
