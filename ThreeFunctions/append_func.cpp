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
#include <libdis.h>
#include "disasm_util.h"
#include "elf_util.h"

#include "append_func.h"

unsigned char insert_code[] = { 0x55, 0x89, 0xe5, 0x8b, 0x45, 0x08, 0x0f, 0xaf,
		0x45, 0x0c, 0x5d, 0xc3 };

//write segment to file
void modify_data_in_buffer(Elf32_Ehdr& ehdr, Elf32_Phdr& n_phdr, int old_phoff,
		int add_last_section_gap, int fd) {
	struct stat fileStat;
	fstat(fd, &fileStat);

	ehdr.e_phnum += 1; //*******add new segment
	lseek(fd, 0, SEEK_SET);
	write(fd, &ehdr, sizeof(Elf32_Ehdr));

	char* data = NULL;
	long int oldSize = old_phoff + (ehdr.e_phnum - 1) * ehdr.e_phentsize;
	data = (char*) malloc(fileStat.st_size - oldSize);
	lseek(fd, oldSize, SEEK_SET);
	read(fd, data, fileStat.st_size - oldSize);

	//add entry to program header
	lseek(fd, oldSize, SEEK_SET);
	write(fd, &n_phdr, sizeof(Elf32_Phdr));

	char tmp[PAGESIZE] = { 0 };
	memset(tmp, 0, PAGESIZE - sizeof(Elf32_Phdr));
	write(fd, tmp, PAGESIZE - sizeof(Elf32_Phdr));

	write(fd, data, fileStat.st_size - oldSize);

	//padding between last and add
	data = (char*) malloc(add_last_section_gap);
	memset(data, 0, add_last_section_gap);
	write(fd, data, add_last_section_gap);

	//add data to the end of the file
//	write(fd, insert_code, sizeof(insert_code));
//	data = (char*) malloc(PAGESIZE - sizeof(insert_code));
//	memset(data, 0, PAGESIZE - sizeof(insert_code));
//	write(fd, data, PAGESIZE - sizeof(insert_code));
	//set the new segment to 0
	data = (char*) malloc(PAGESIZE);
	memset(data, 0, PAGESIZE);
	write(fd, data, PAGESIZE);

	free(data);
}

//a new program header item
int new_program_header_item(Elf32_Ehdr& ehdr, Elf32_Phdr& n_phdr,
		Elf32_Shdr& shdr, int old_shoff, int fd) {
	Elf32_Phdr load_phdr;
	lseek(fd, ehdr.e_phoff + 2 * ehdr.e_phentsize, SEEK_SET);
	read(fd, &load_phdr, sizeof(Elf32_Phdr));

	//last_offset and last_vaddr
	lseek(fd, old_shoff + (ehdr.e_shnum - 1) * ehdr.e_shentsize, SEEK_SET);
	printf("(ehdr.e_shnum - 1) = %d\n", (ehdr.e_shnum - 1));
	read(fd, &shdr, sizeof(Elf32_Phdr));
	Elf32_Off last_offset = shdr.sh_offset + shdr.sh_size;

	//TODO
	Elf32_Addr last_vaddr = load_phdr.p_vaddr + 4 * PAGESIZE; //0x804b000	/

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
	n_phdr.p_filesz = PAGESIZE; //0x1000
	n_phdr.p_memsz = PAGESIZE;
	n_phdr.p_flags = load_phdr.p_flags;
	n_phdr.p_align = load_phdr.p_align;

	return add_offset;
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
		lseek(fd, old_shoff + i * ehdr.e_shentsize, SEEK_SET);
		write(fd, &shdr, sizeof(Elf32_Shdr));
	}
}

//loop program header and modify the offset
void modify_elf_program_header(Elf32_Ehdr& ehdr, Elf32_Phdr& phdr, int fd) {
	//segment 0---phdr segment
	lseek(fd, ehdr.e_phoff, SEEK_SET);
	read(fd, &phdr, sizeof(Elf32_Phdr));
	phdr.p_vaddr -= PAGESIZE;
	lseek(fd, ehdr.e_phoff, SEEK_SET);
	write(fd, &phdr, sizeof(Elf32_Phdr));

	//segment 1---interp segment
	lseek(fd, ehdr.e_phoff + ehdr.e_phentsize, SEEK_SET);
	read(fd, &phdr, sizeof(Elf32_Phdr));
	phdr.p_offset += PAGESIZE;
	lseek(fd, ehdr.e_phoff + ehdr.e_phentsize, SEEK_SET);
	write(fd, &phdr, sizeof(Elf32_Phdr));

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
		lseek(fd, ehdr.e_phoff + i * ehdr.e_phentsize, SEEK_SET);
		write(fd, &phdr, sizeof(Elf32_Phdr));
	}
}

//modify the section header offset
void modify_elf_header(Elf32_Ehdr& ehdr, int fd) {
	ehdr.e_shoff += PAGESIZE;

	lseek(fd, 0, SEEK_SET);
	write(fd, &ehdr, sizeof(Elf32_Ehdr));
}

Elf32_Off add_segment(char* elf_file, Elf32_Phdr& n_phdr) {
	int fd;
	int old_shoff;
	int old_phoff;

	Elf32_Ehdr ehdr;
	Elf32_Phdr phdr;
	Elf32_Shdr shdr;

	//open file and read elf header to ehdr
	fd = open(elf_file, O_RDWR);
	read(fd, &ehdr, sizeof(Elf32_Ehdr));
	/*	if (strncmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
	 exit(0);
	 }
	 */
	//original section header offset of elf file
	old_shoff = ehdr.e_shoff;
	old_phoff = ehdr.e_phoff;

	modify_elf_header(ehdr, fd);

	modify_elf_program_header(ehdr, phdr, fd);

	modify_elf_section_header(ehdr, shdr, old_shoff, fd);

	int add_offset = new_program_header_item(ehdr, n_phdr, shdr, old_shoff, fd);

	Elf32_Off last_offset = shdr.sh_offset + shdr.sh_size;
	Elf32_Off add_last_section_gap = add_offset - last_offset;

	modify_data_in_buffer(ehdr, n_phdr, old_phoff, add_last_section_gap, fd);
	close(fd);

	return add_offset;
}

void insert_into_func_list(FuncList* header, Function* func) {
	if (header == NULL) {
		return;
	}

	if (header->func == NULL) {
		header->func = func;
		return;
	}

	FuncList *toInsert = (FuncList *) malloc(sizeof(FuncList));
	toInsert->func = func;
	toInsert->next = NULL;
	/** 插到第一个元素前 */
	if (header->func->start_addr > func->start_addr) {
		toInsert->func = header->func;
		header->func = func;
		toInsert->next = header->next;
		header->next = toInsert;
		return;
	}

	if (header->func->start_addr == func->start_addr) {
		free(toInsert);
		return;
	}

	FuncList *previous = header;
	FuncList *current = NULL;
	while ((current = previous->next) != NULL) {
		if (current->func->start_addr > func->start_addr) {
			toInsert->next = previous->next;
			previous->next = toInsert;
			return;
		}
		if (current->func->start_addr == func->start_addr) {
			free(toInsert);
			return;
		}
		previous = current;
	}

	previous->next = toInsert;
}

Elf32_Sym* find_func_sym(Elf* e, const char* func_name, FuncList* func_list) {
	Elf32_Sym *sym = NULL;
	Function* func = (Function*) malloc(sizeof(Function));
	sym = find_func_sym(e, func_name, func);
	insert_into_func_list(func_list, func);

	return sym;
}

Elf32_Sym* find_func_sym(Elf* e, const char* func_name, Function* func) {
	Elf32_Sym *sym = NULL;

	size_t strtabScnNdxArray[2];
	strtabScnNdxArray[0] = get_scn_ndx(e, ".strtab");
	strtabScnNdxArray[1] = get_scn_ndx(e, ".dynstr");

	size_t symScnNdxArray[2];
	symScnNdxArray[0] = get_scn_ndx(e, ".symtab");
	symScnNdxArray[1] = get_scn_ndx(e, ".dynsym");

	size_t textScnNdx = get_scn_ndx(e, ".text");
	Elf32_Addr func_addr = 0;
	int i;
	for (i = 0; i < 2; i++) {
		size_t symScnNdx = symScnNdxArray[i];

		Elf_Scn *symScn = elf_getscn(e, symScnNdx);

		Elf_Data *symData;
		if ((symData = elf_getdata(symScn, NULL)) == NULL) {
			return 0;
		}
		sym = (Elf32_Sym *) (symData->d_buf);
		int symNum = symData->d_size / sizeof(Elf32_Sym);

		char *name;
		int j;
		for (j = 1; j < symNum; j++) {
			sym++;
			int st_type = ELF32_ST_TYPE(sym->st_info);

			if (st_type == STT_FUNC && sym->st_shndx == textScnNdx) {
				name = elf_strptr(e, strtabScnNdxArray[i], sym->st_name);
				if (strcmp(name, func_name) == 0) {
					func_addr = sym->st_value;

					printf("func_name: %s, func_addr = %x\n", func_name,
							func_addr);

					func->name = name;
					func->start_addr = sym->st_value;
					func->size = sym->st_size;

					return sym;
				}
			}
		}
	}

	printf("func_name: %s, func_addr = %x\n", func_name, func_addr);

	return NULL;
}

void set_func_end_addr(FuncList *header) {
	if (header->func == NULL) {
		return;
	}

	FuncList *current = header;
	FuncList *next = NULL;
	Function *func;
	while (current != NULL && (func = current->func) != NULL) {
		if ((next = current->next) != NULL) {
			func->end_addr = next->func->start_addr;
		} else {
			func->end_addr = func->start_addr + func->size;
		}
		current = next;
	}
}

void get_func_body(Elf* e, FuncList* func_list) {
	size_t text_scn_ndx = get_scn_ndx(e, ".text");
	Elf_Scn* text_scn = elf_getscn(e, text_scn_ndx);
	Elf_Data* text_data = NULL;

	if ((text_data = elf_getdata(text_scn, NULL)) == NULL) {
		errx(EX_SOFTWARE, "elf_getdata() failed:%s.", elf_errmsg(-1));
	}

	Elf32_Shdr* text_shdr = elf32_getshdr(text_scn);
	unsigned char* scn_binary = (unsigned char*) malloc(
			sizeof(unsigned char) * text_shdr->sh_size);
	memcpy(scn_binary, text_data->d_buf, text_shdr->sh_size);

	set_func_binary(func_list, scn_binary, text_shdr->sh_addr);
}

void set_func_binary(FuncList *header, unsigned char *scn_binary,
		Elf32_Addr scn_start_addr) {
	FuncList *current = header;
	Function *func;

	while (current != NULL) {
		if ((func = current->func) != NULL && func->size > 0) {
			func->binary = scn_binary + (func->start_addr - scn_start_addr);
		}

		current = current->next;
	}
}

void add_func_to_segment(int fd, Elf32_Off add_offset, FuncList* func_list) {
	FuncList *current = func_list;
	Function *func;

	Elf32_Off offset_incr = add_offset;
	while (current != NULL) {
		if ((func = current->func) != NULL && func->size > 0) {
			//identify the start of the new segment
			lseek(fd, offset_incr, SEEK_SET);

			write(fd, func->binary, func->size);
			offset_incr += func->end_addr - func->start_addr;
		}

		current = current->next;
	}
}

//exchange the whole order
FuncList* exchange_func_order(FuncList* func_list) {
	if (func_list == NULL)	return NULL;
	if (func_list->next == NULL) return NULL;
	FuncList* p_begin = func_list->next;
	FuncList* p_end = func_list;
	p_end->next = NULL;

	do {
		FuncList* p_temp = p_begin->next;
		p_begin->next = p_end;
		p_end = p_begin;
		p_begin = p_temp;
	} while (p_begin != NULL);

	return p_end;
}

void print_func_list(FuncList* func_list) {
	FuncList* current = func_list;
	Function* func;

	while(current != NULL) {
		if ((func = current->func) != NULL) {
			printf("func->name : %s\t, func->start : %x\t", func->name, func->start_addr);
		}
		current = current->next;
	}

	printf("\n");
}

void modify_call_target_func(Function* main_func, FuncList* func_list, Elf32_Phdr& n_phdr) {
	FuncList* current = func_list;
	Function* func;

	Elf32_Addr new_target_addr = n_phdr.p_vaddr;
	while (current != NULL) {
		if ((func = current->func) != NULL) {
			printf("new_target_addr = %x, func->end_addr = %x, func->start_addr= %x\n", new_target_addr, func->end_addr ,func->start_addr);
			modify_call_target_func(main_func, func->start_addr, new_target_addr);

			new_target_addr = new_target_addr + func->end_addr - func->start_addr;
		}
		current = current->next;
	}
}

void int_to_char(unsigned int addr, unsigned char* addr_arr, int size) {
	for (int i = 0; i < size; i ++) {
		addr_arr[i] = addr >> (i * 8);
	}
}

void print_array(unsigned char* addr_arr, int size) {
	printf("\n");
	for (int i = 0; i < size; i  ++) {
		printf("%x ", addr_arr[i]);
	}

	printf("\n");
}

void modify_call_target_func(Function* main_func, Elf32_Addr old_target_addr, Elf32_Addr new_target_addr) {
	char line[LINE_SIZE];

	size_t pos = 0;
	size_t size = 0;
	x86_insn_t insn;

	unsigned char *buf = main_func->binary;
	size_t buf_len = main_func->size;
	Elf32_Addr buf_start = main_func->start_addr;

	x86_init(opt_none, NULL, NULL);
	printf("func->size = %d\n", main_func->size);
	while (pos < main_func->size) {
		size = x86_disasm(buf, buf_len, buf_start, pos, &insn);
		if (size) {
			x86_format_insn(&insn, line, LINE_SIZE, intel_syntax);
//			printf("group:%d\ttype:%x\taddr:%x\t", insn.group, insn.type,
//					insn.addr);

			int i;
			for (i = 0; i < insn.size; i++) {
				printf("%02hhx ", insn.bytes[i]);
			}
			for (i = 0; i < 8 - insn.size; i++) {
				printf("    ");
			}

			printf("\t%s\t", line);
			//printf("ins_type:%s\t", get_insn_type_str(insn.type));

			x86_op_t* op;
			//if instruction is a call
			if (insn.type == insn_call || insn.type == insn_callcc) {
				op = x86_operand_1st(&insn);
				Elf32_Addr target_addr;
				if (op->datatype == op_byte) {
					printf("--b--");
					target_addr = (signed int) (op->data.relative_near)
							+ insn.addr + insn.size;
				} else if (op->datatype == op_dword) {
					printf("--d--");
//					printf("--d--op->data.dword=%x, ", op->data.dword);
					target_addr = op->data.dword + insn.addr + insn.size;
				} else {
					printf("\top_type unsupported:%d,", op->datatype);
					continue;
				}
				printf("\t target_addr:%x", target_addr);

//				printf("\t op->datatype:%d", op->datatype);

				//find the corresponding target address of add, sub and mul
				if (target_addr == old_target_addr) {
//					target_addr = op->data.dword + insn.addr + insn.size;
					op->data.dword = new_target_addr - (insn.addr + insn.size);
					printf("\t new_target_addr:%x, -- %x", new_target_addr, op->data.dword);

					unsigned char addr_arr[TARGET_ADDR_LENGTH];
					int_to_char(op->data.dword, addr_arr, TARGET_ADDR_LENGTH);

					print_array(addr_arr, 4);

					memcpy(main_func->binary + pos + 1, addr_arr, insn.size - 1);
				}
			}

//			size_t operand_count = x86_operand_count(&insn, op_any);
//			x86_oplist_t* oplist = insn.operands;
//
//			printf("\t operands:%d\t", operand_count);
//			while (oplist != NULL) {
//				op = &oplist->op;
//				x86_reg_t reg;
//				x86_ea_t expr;
//				switch (op->type) {
//				case op_immediate:
//				case op_relative_near:
//				case op_relative_far:
//				case op_absolute:
//				case op_offset:
//					switch (op->datatype) {
//					case op_byte:
//						printf("%x", op->data.byte);
//						break;
//					case op_word:
//						printf("%x", op->data.word);
//						break;
//					case op_dword:
//						printf("%x", op->data.dword);
//						break;
//					default:
//						break;
//					}
//					break;
//				case op_register:
//					reg = op->data.reg;
//
//					printf("%s", reg.name);
//					break;
//				case op_expression:
//					expr = op->data.expression;
//
//					printf("%s", expr.base.name);
//					if (expr.index.id > 0) {
//						printf("+%s*%d", expr.index.name, expr.scale);
//					}
//					if (expr.disp != 0) {
//						printf("+0x%x", expr.disp);
//					}
//
//					break;
//				case op_unused:
//				case op_unknown:
//					break;
//				default:
//					break;
//				}
//
//				printf("(%s ", get_operand_type_str(op->type));
//				printf("%s", (op->access & op_read) ? "r" : "");
//				printf("%s", (op->access & op_write) ? "w" : "");
//				printf("%s", (op->access & op_execute) ? "x" : "");
//				printf("), ");
//
//				oplist = oplist->next;
//			}

			printf("\n");
			pos += size;
		} else {
			//invalid/unrecognized instruction
			printf("nop\n");
			pos++;
		}
	}

	x86_cleanup();
}

void update_main_func_binary(int fd, Elf* e, Function* main_func) {
	size_t text_scn_ndx = get_scn_ndx(e, ".text");
	Elf_Scn* text_scn = elf_getscn(e, text_scn_ndx);
//	Elf_Data* text_data = NULL;

//	if ((text_data = elf_getdata(text_scn, NULL)) == NULL) {
//		errx(EX_SOFTWARE, "elf_getdata() failed:%s.", elf_errmsg(-1));
//	}

	Elf32_Shdr* text_shdr = elf32_getshdr(text_scn);
//	memcpy(text_data->d_buf + (main_func->start_addr - text_shdr->sh_addr), main_func->binary, main_func->size);
//	elf_flagdata(text_data, ELF_C_SET, ELF_F_DIRTY);

	Elf32_Off offset = text_shdr->sh_offset + (main_func->start_addr - text_shdr->sh_addr);
	lseek(fd, offset, SEEK_SET);
	write(fd, main_func->binary, main_func->size);
}


