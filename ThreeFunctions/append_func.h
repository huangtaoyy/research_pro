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
#include <libdis.h>
#include "disasm_util.h"
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

#define PAGESIZE 4096
#define LINE_SIZE 50
#define TARGET_ADDR_LENGTH 4

//write segment to file
void modify_data_in_buffer(Elf32_Ehdr& ehdr, Elf32_Phdr& n_phdr, int old_phoff,
		int add_last_section_gap, int fd);

//a new program header item
int new_program_header_item(Elf32_Ehdr& ehdr, Elf32_Phdr& n_phdr,
		Elf32_Shdr& shdr, int old_shoff, int fd);

//loop section header and modify the offset
void modify_elf_section_header(Elf32_Ehdr& ehdr, Elf32_Shdr& shdr,
		int old_shoff, int fd);

//loop program header and modify the offset
void modify_elf_program_header(Elf32_Ehdr& ehdr, Elf32_Phdr& phdr, int fd);

//modify the section header offset
void modify_elf_header(Elf32_Ehdr& ehdr, int fd);

Elf32_Off add_segment(char* elf_file, Elf32_Phdr& n_phdr);

void insert_into_func_list(FuncList* header, Function* func);

Elf32_Sym* find_func_sym(Elf* e, const char* func_name, FuncList* func_list);

Elf32_Sym* find_func_sym(Elf* e, const char* func_name, Function* func);

void set_func_end_addr(FuncList *header);

void get_func_body(Elf* e, FuncList* func_list);

void set_func_binary(FuncList *header, unsigned char *scn_binary,
		Elf32_Addr scn_start_addr);

void add_func_to_segment(int fd, Elf32_Off add_offset, FuncList* func_list);

void int_to_char(unsigned int addr, unsigned char* addr_arr, int size);

FuncList* exchange_func_order(FuncList* func_list);

void print_func_list(FuncList* func_list);

void modify_call_target_func(Function* main_func, FuncList* func_list, Elf32_Phdr& n_phdr);

void modify_call_target_func(Function* main_func, Elf32_Addr old_target_addr, Elf32_Addr new_target_addr);

void update_main_func_binary(int fd, Elf* e, Function* main_func);
