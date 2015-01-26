/*
 * pad_data.cpp
 *
 *  Created on: 2014-3-11
 *      Author: huangtao
 */
#include "pad_data.h"
#include "elf_util.h"

#include <libelf.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

unsigned char* file_buffer;
int buffer_size;

int get_allo_region_size(int fd) {
	Elf *e;
	if (elf_version(EV_CURRENT) == EV_NONE)
			errx ( EXIT_FAILURE , " ELF library initialization "
			" failed : %s " , elf_errmsg ( -1));
	lseek(fd, 0, SEEK_SET);
	if ((e = elf_begin(fd, ELF_C_RDWR, NULL)) == NULL)
		errx ( EXIT_FAILURE , " elf_begin () failed : %s . " ,	elf_errmsg ( -1));

	int allo_size = 0;
	size_t text_scn_ndx = get_scn_ndx(e, ".text");
	Elf_Scn* text_scn = elf_getscn(e, text_scn_ndx);
	Elf32_Shdr* text_shdr = elf32_getshdr(text_scn);

	printf("ndx = %d\n", text_scn_ndx);
	//TODO
	allo_size = text_shdr->sh_size;
	printf("allo_size = %x\n", allo_size);

	elf_end(e);

	return allo_size;
}

void copyFileToBuffer(int fd, int allo_size) {
	struct stat fileStat;
	fstat(fd, &fileStat);

	buffer_size = fileStat.st_size + allo_size;
	file_buffer = (unsigned char*) malloc(sizeof(unsigned char) * buffer_size);
	if (file_buffer == NULL) {
		fputs("allocate buffer error", stderr);
		exit(1);
	}
	memset(file_buffer, 0, buffer_size);

	lseek(fd, 0, SEEK_SET);
	read(fd, file_buffer, buffer_size);
}

void pad_data(int fd, PadRegion& region, int size) {
	Elf32_Ehdr ehdr;

	lseek(fd, 0, SEEK_SET);
	read(fd, &ehdr, sizeof(Elf32_Ehdr));

	modify_program_header(fd, ehdr, region, size);
}

void modify_program_header(int fd, Elf32_Ehdr& ehdr, PadRegion& region, int size) {
	int phoff = ehdr.e_phoff;
	int phnum = ehdr.e_phnum;
	int ensize = ehdr.e_phentsize;

	for (int i = 0; i < phnum; i ++) {
		Elf32_Phdr phdr;
		//TODO identify the data segment
		lseek(fd, phoff + i * ensize, SEEK_SET);
		read(fd, &phdr, sizeof(Elf32_Phdr));

		Elf32_Word type = phdr.p_type;
		Elf32_Word flag = phdr.p_flags;
		if (type == PT_LOAD && flag == (PF_R | PF_W)) {

			region.pad_addr = phdr.p_vaddr + phdr.p_memsz;
			region.pad_offset = phdr.p_offset + phdr.p_filesz;
			region.pad_size = size;

			printf("segment offset = %x, filesize = %x, memsz = %x\n", phdr.p_offset, phdr.p_filesz, phdr.p_memsz);
			phdr.p_filesz += size;
			phdr.p_memsz += size;
			printf("segment offset = %x, filesize = %x, memsz = %x\n", phdr.p_offset, phdr.p_filesz, phdr.p_memsz);

			memcpy(file_buffer + phoff + i * ensize, &phdr, sizeof(Elf32_Phdr));
			break;
		}
	}

}

char* copyBufferToFile(char* file_name) {
	char* new_file = strcat(file_name, "_add");

	FILE* fd = fopen(new_file, "wb+");

	fwrite(file_buffer, sizeof(unsigned char), buffer_size, fd);
	chmod(new_file, S_IRWXU | S_IRWXG | S_IRWXO);

	printf("write to file %s, buffer_size = %x\n", new_file, buffer_size);

	fclose(fd);
	free(file_buffer);

	return new_file;
}



