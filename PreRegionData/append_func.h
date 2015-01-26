/*
 * appdend_func.h
 *
 *  Created on: 2013-12-19
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
//#include <libdis.h>
//#include "disasm_util.h"
#include "elf_util.h"

struct Function {
	char* name;
	Elf32_Addr start_addr;
	Elf32_Addr end_addr;
	size_t size;
	unsigned char* binary;
};

struct FuncList {
	Function *func;
	struct FuncList *next;
	FuncList () {
		func = NULL;
		next = NULL;
	}
};

struct AlloRegion {
	Elf32_Off allo_offset;
	Elf32_Addr allo_addr;
	Elf32_Word allo_size;

	AlloRegion (Elf32_Off offset, Elf32_Addr addr, Elf32_Word size) {
		this->allo_offset = offset;
		this->allo_addr = addr;
		this->allo_size = size;
	}
};

#define PAGESIZE 4096
#define LINE_SIZE 50
#define TARGET_ADDR_LENGTH 4

void copyFileToBuffer(int fd, int allo_size);

void copyBufferToFile(char* file_name, Elf32_Phdr& n_data_phdr);

//write segment to file
void modify_data_in_buffer(Elf32_Ehdr& ehdr, Elf32_Phdr& n_phdr, Elf32_Phdr& n_data_phdr, int old_phoff, int fd);

int get_last_offset(Elf32_Ehdr& ehdr, int old_shoff, int fd);

//get the max vaddr for the new segment to occupy
int get_last_vaddr(Elf32_Ehdr& ehdr, int fd);

//get the filesize of the new segment
int get_allo_region_size(int fd);

//a new program header item
int new_program_header(Elf32_Ehdr& ehdr, Elf32_Phdr& n_phdr, int old_shoff, int size, int fd);

//a data segment header item
void new_data_program_header(Elf32_Ehdr& ehdr, Elf32_Phdr&  n_phdr, Elf32_Phdr& n_data_phdr, int fd);

//loop section header and modify the offset
void modify_elf_section_header(Elf32_Ehdr& ehdr, int old_shoff, int fd);

void modify_data_segment_size(Elf32_Ehdr& ehdr, Elf32_Phdr& n_phdr, int fd);

//loop program header and modify the offset
void modify_elf_program_header(Elf32_Ehdr& ehdr, int fd);

//modify the section header offset
void modify_elf_header(Elf32_Ehdr& ehdr, int fd);

void add_segment(int fd, Elf32_Phdr& n_phdr, Elf32_Phdr& n_data_phdr, int size);
