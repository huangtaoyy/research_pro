/*
 * append_func.cpp
 *
 *  Created on: 2013-12-18
 *      Author: huangtao
 */
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <fcntl.h>
#include <sysexits.h>
#include <gelf.h>
#include <elf.h>
#include <libelf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "elf_util.h"

#include "append_func.h"

unsigned char* file_buffer;
int buffer_size;

//double the text segment size
int get_allo_region_size(int fd){
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
	allo_size = text_shdr->sh_size * 2;
	printf("allo_size = %x\n", allo_size);

	elf_end(e);

	return allo_size;
}

void copyFileToBuffer(int fd, int allo_size) {
	struct stat fileStat;
	fstat(fd, &fileStat);

	buffer_size = fileStat.st_size + allo_size + PAGESIZE;
	file_buffer = (unsigned char*) malloc(sizeof(unsigned char) * buffer_size);
	if (file_buffer == NULL) {
		fputs("allocate buffer error", stderr);
		exit(1);
	}
	memset(file_buffer, 0, buffer_size);

	lseek(fd, 0, SEEK_SET);
	read(fd, file_buffer, buffer_size);
}

Elf32_Off add_segment(int fd, Elf32_Phdr& n_phdr, int size) {
	int old_shoff;
	int old_phoff;

	Elf32_Ehdr ehdr;
	Elf32_Phdr phdr;
	Elf32_Shdr shdr;

	//open file and read elf header to ehdr
	lseek(fd, 0, SEEK_SET);
	read(fd, &ehdr, sizeof(Elf32_Ehdr));

	//original section header offset of elf file
	old_shoff = ehdr.e_shoff;
	old_phoff = ehdr.e_phoff;

	modify_elf_header(ehdr, fd);

	modify_elf_program_header(ehdr, phdr, fd);

	int add_offset = new_program_header(ehdr, n_phdr, shdr, old_shoff, size, fd);

	Elf32_Off last_offset = shdr.sh_offset + shdr.sh_size;
	Elf32_Off add_last_section_gap = add_offset - last_offset;

	modify_data_in_buffer(ehdr, n_phdr, old_phoff, add_last_section_gap, fd);

	modify_elf_section_header(ehdr, shdr, old_shoff, fd);

	return add_offset;
}


//modify the section header offset
void modify_elf_header(Elf32_Ehdr& ehdr, int fd) {
	printf("ehdr.e_shoff = %d,\n", ehdr.e_shoff);
	ehdr.e_shoff += PAGESIZE;
	printf("ehdr.e_shoff = %d,\n", ehdr.e_shoff);

//	lseek(fd, 0, SEEK_SET);
//	write(fd, &ehdr, sizeof(Elf32_Ehdr));
	memcpy(file_buffer, &ehdr, sizeof(Elf32_Ehdr));
}


//loop program header and modify the offset
void modify_elf_program_header(Elf32_Ehdr& ehdr, Elf32_Phdr& phdr, int fd) {
	//segment 0---phdr segment
	lseek(fd, ehdr.e_phoff, SEEK_SET);
	read(fd, &phdr, sizeof(Elf32_Phdr));
	phdr.p_vaddr -= PAGESIZE;
	lseek(fd, ehdr.e_phoff, SEEK_SET);
//	write(fd, &phdr, sizeof(Elf32_Phdr));
	memcpy(file_buffer + ehdr.e_phoff, &phdr, sizeof(Elf32_Phdr));

	//segment 1---interp segment
	lseek(fd, ehdr.e_phoff + ehdr.e_phentsize, SEEK_SET);
	read(fd, &phdr, sizeof(Elf32_Phdr));
	phdr.p_offset += PAGESIZE;
	lseek(fd, ehdr.e_phoff + ehdr.e_phentsize, SEEK_SET);
//	write(fd, &phdr, sizeof(Elf32_Phdr));
	memcpy(file_buffer + ehdr.e_phoff + ehdr.e_phentsize, &phdr, sizeof(Elf32_Phdr));

	int i = 2;
	//modify the program rest header
	for (; i < ehdr.e_phnum; i++) {
		lseek(fd, ehdr.e_phoff + i * ehdr.e_phentsize, SEEK_SET);
		read(fd, &phdr, sizeof(Elf32_Phdr));

		if (i == 2) { //the first LOAD Segment
			phdr.p_filesz += PAGESIZE;
			phdr.p_memsz += PAGESIZE;
			phdr.p_vaddr -= PAGESIZE;
		}

		if (phdr.p_offset != 0x0) {
			//increase p_offset by PAGESIZE
			phdr.p_offset += PAGESIZE;
		}

		//write back
//		lseek(fd, ehdr.e_phoff + i * ehdr.e_phentsize, SEEK_SET);
//		write(fd, &phdr, sizeof(Elf32_Phdr));
		memcpy(file_buffer + ehdr.e_phoff + i * ehdr.e_phentsize, &phdr, sizeof(Elf32_Phdr));
	}
}

int get_last_vaddr(Elf32_Ehdr& ehdr, int fd) {
	int max_addr = 0;

	int i = 0;
	for (; i < ehdr.e_phnum; i++) {
		Elf32_Phdr phdr;
		lseek(fd, ehdr.e_phoff + i * ehdr.e_phentsize, SEEK_SET);
		read(fd, &phdr, sizeof(Elf32_Phdr));

		int temp_addr = phdr.p_vaddr + phdr.p_filesz;
		if (max_addr < temp_addr) {
			max_addr = temp_addr;
		}
	}

	//TODO
	return (max_addr / PAGESIZE  + 1) * PAGESIZE;
}


//a new program header item
int new_program_header(Elf32_Ehdr& ehdr, Elf32_Phdr& n_phdr,
		Elf32_Shdr& shdr, int old_shoff, int size, int fd) {
	Elf32_Phdr load_phdr;
	lseek(fd, ehdr.e_phoff + 2 * ehdr.e_phentsize, SEEK_SET);
	read(fd, &load_phdr, sizeof(Elf32_Phdr));

	//last_offset and last_vaddr
	lseek(fd, old_shoff + (ehdr.e_shnum - 1) * ehdr.e_shentsize, SEEK_SET);
	printf("(ehdr.e_shnum - 1) = %d\n", (ehdr.e_shnum - 1));
	read(fd, &shdr, sizeof(Elf32_Phdr));
	Elf32_Off last_offset = shdr.sh_offset + shdr.sh_size + PAGESIZE;

	//TODO
	Elf32_Addr last_vaddr = get_last_vaddr(ehdr, fd); //load_phdr.p_vaddr + 4 * PAGESIZE; //0x804b000	/

	//set attributes
	n_phdr.p_type = load_phdr.p_type;
	printf("p_type = %d\n", n_phdr.p_type);

	Elf32_Off add_offset = ((last_offset + load_phdr.p_align)
			/ load_phdr.p_align) * load_phdr.p_align;
	n_phdr.p_offset = add_offset;
	printf("p_offset = %x\n", n_phdr.p_offset);

	n_phdr.p_vaddr = last_vaddr; //0x804b000
	printf("p_vaddr = %x\n", n_phdr.p_vaddr);
	n_phdr.p_paddr = last_vaddr;
	n_phdr.p_flags = load_phdr.p_flags;
	n_phdr.p_align = load_phdr.p_align;

	//TODO
//	n_phdr.p_filesz = PAGESIZE; //0x1000
//	n_phdr.p_memsz = PAGESIZE;
	n_phdr.p_filesz = size; //get_allo_region_size(fd);
	n_phdr.p_memsz = n_phdr.p_filesz;

	return add_offset;
}


//write segment to file
void modify_data_in_buffer(Elf32_Ehdr& ehdr, Elf32_Phdr& n_phdr, int old_phoff,
		int add_last_section_gap, int fd) {
	struct stat fileStat;
	fstat(fd, &fileStat);

	ehdr.e_phnum += 1; //*******add new segment
	lseek(fd, 0, SEEK_SET);
//	write(fd, &ehdr, sizeof(Elf32_Ehdr));
	memcpy(file_buffer, &ehdr, sizeof(Elf32_Ehdr));

	char* data = NULL;
	long int oldSize = old_phoff + (ehdr.e_phnum - 1) * ehdr.e_phentsize;
	data = (char*) malloc(fileStat.st_size - oldSize);
	lseek(fd, oldSize, SEEK_SET);
	read(fd, data, fileStat.st_size - oldSize);

	//add new entry to program header
	lseek(fd, oldSize, SEEK_SET);
//	write(fd, &n_phdr, sizeof(Elf32_Phdr));
	memcpy(file_buffer + oldSize, &n_phdr, sizeof(Elf32_Phdr));

	memset(file_buffer + oldSize + sizeof(Elf32_Phdr), 0, PAGESIZE - sizeof(Elf32_Phdr));
//	write(fd, tmp, PAGESIZE - sizeof(Elf32_Phdr));

//	write(fd, data, fileStat.st_size - oldSize);
	memcpy(file_buffer + oldSize + PAGESIZE, data, fileStat.st_size - oldSize);

//	//padding between last and add
//	data = (char*) malloc(add_last_section_gap);
//	memset(data, 0, add_last_section_gap);
//	write(fd, data, add_last_section_gap);
//
//	//set the new segment to 0
//	data = (char*) malloc(n_phdr.p_filesz);
//	memset(data, 0, n_phdr.p_filesz);
//	write(fd, data, n_phdr.p_filesz);

	free(data);
}


//loop section header and modify the offset
void modify_elf_section_header(Elf32_Ehdr& ehdr, Elf32_Shdr& shdr,
		int old_shoff, int fd) {
	int i = 1;
	for (; i < ehdr.e_shnum; i++) {
		//read section header to shdr
		lseek(fd, old_shoff + i * ehdr.e_shentsize, SEEK_SET);
		read(fd, &shdr, sizeof(Elf32_Shdr));

		//increase sh_offset by PAGESIZE
		shdr.sh_offset += PAGESIZE;

		//write back
//		lseek(fd, old_shoff + i * ehdr.e_shentsize, SEEK_SET);
//		write(fd, &shdr, sizeof(Elf32_Shdr));
		memcpy(file_buffer + old_shoff + PAGESIZE + i * ehdr.e_shentsize, &shdr, sizeof(Elf32_Shdr));
	}
}


void copyBufferToFile(char* file_name, Elf32_Phdr& n_phdr) {
	char* new_file = strcat(file_name, "_add");

	FILE* fd = fopen(new_file, "wb+");

	unsigned char* data = (unsigned char*) malloc(n_phdr.p_filesz + n_phdr.p_offset);
	memset(data, 0, n_phdr.p_filesz + n_phdr.p_offset);
	memcpy(data, file_buffer, buffer_size);

	fwrite(data, sizeof(unsigned char), n_phdr.p_filesz + n_phdr.p_offset, fd);
	chmod(new_file, S_IRWXU | S_IRWXG | S_IRWXO);

	printf("write to file %s, buffer_size = %x\n", new_file, buffer_size);

	fclose(fd);
	free(file_buffer);
	free(data);
}


