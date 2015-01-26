/*
 * addsegment.c
 *
 *  Created on: 2013-11-20
 *      Author: huangtao
 */
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <libelf.h>
#include "elf_util.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define PAGESIZE 4096

unsigned char insert_code[] = { 0x55, 0x89, 0xe5, 0x8b, 0x45, 0x08, 0x0f, 0xaf,
		0x45, 0x0c, 0x5d, 0xc3 };

Elf32_Off add_segment(char* elfFile) {
	int fd;
	int oldShoff;
	int oldPhoff;

	Elf32_Ehdr ehdr;
	Elf32_Shdr shdr;
	Elf32_Phdr phdr;

	//open elf file and read elf header to ehdr
	fd = open(elfFile, O_RDWR);
	read(fd, &ehdr, sizeof(ehdr));
	if (strncmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
		exit(0);
	}

	//original section header offset of elf file
	oldShoff = ehdr.e_shoff;

	oldPhoff = ehdr.e_phoff;

	//modify the elf header
	ehdr.e_shoff += PAGESIZE;
	//ehdr.e_phnum += 1;										//*******add new segment
	lseek(fd, 0, SEEK_SET);
	write(fd, &ehdr, sizeof(ehdr));

	//loop program header and modify the offset
	//segment 0---phdr segment
	lseek(fd, ehdr.e_phoff, SEEK_SET);
	read(fd, &phdr, sizeof(phdr));
	phdr.p_vaddr -= PAGESIZE;
	lseek(fd, ehdr.e_phoff, SEEK_SET);
	write(fd, &phdr, sizeof(phdr));

	//segment 1---interp segment
	lseek(fd, ehdr.e_phoff + ehdr.e_phentsize, SEEK_SET);
	read(fd, &phdr, sizeof(phdr));
	phdr.p_offset += PAGESIZE;
	lseek(fd, ehdr.e_phoff + ehdr.e_phentsize, SEEK_SET);
	write(fd, &phdr, sizeof(phdr));

	int i = 2;
	for (; i < ehdr.e_phnum; i++) {
		//read program header to phdr
		lseek(fd, ehdr.e_phoff + i * ehdr.e_phentsize, SEEK_SET);
		read(fd, &phdr, sizeof(phdr));

		if (i == 2) {
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
		write(fd, &phdr, sizeof(phdr));
	}

	//loop section header and modify the offset
	i = 1;
	for (; i < ehdr.e_shnum; i++) {
		//read section header to shdr
		lseek(fd, oldShoff + i * ehdr.e_shentsize, SEEK_SET);
		read(fd, &shdr, sizeof(shdr));

		//increase sh_offset by PAGESIZE
		shdr.sh_offset += PAGESIZE;

		//write back
		lseek(fd, oldShoff + i * ehdr.e_shentsize, SEEK_SET);
		write(fd, &shdr, sizeof(shdr));
	}

//******************* star a new program header
	Elf32_Phdr n_phdr; //*******add new segment
	Elf32_Phdr load_phdr; //*******add new segment
	lseek(fd, ehdr.e_phoff + 2 * ehdr.e_phentsize, SEEK_SET); //*******add new segment
	read(fd, &load_phdr, sizeof(phdr)); //*******add new segment

	//last_offset and last_vaddr
	lseek(fd, oldShoff + (ehdr.e_shnum - 1) * ehdr.e_shentsize, SEEK_SET); //*******add new segment
	printf("(ehdr.e_shnum - 1) = %d\n", (ehdr.e_shnum - 1));
	read(fd, &shdr, sizeof(shdr)); //*******add new segment
	Elf32_Off last_offset = shdr.sh_offset + shdr.sh_size; //*******add new segment
	Elf32_Addr last_vaddr = load_phdr.p_vaddr + 4 * PAGESIZE; //0x804b000	//*******add new segment

	//set attributes
	n_phdr.p_type = load_phdr.p_type; //*******add new segment
	printf("p_type = %d\n", n_phdr.p_type);

	Elf32_Off add_offset = ((last_offset + load_phdr.p_align)
			/ load_phdr.p_align) * load_phdr.p_align;
	Elf32_Off add_last_section_gap = add_offset - last_offset;
	n_phdr.p_offset = add_offset;
	printf("p_offset = %x\n", n_phdr.p_offset); //*******add new segment

	n_phdr.p_vaddr = last_vaddr; //0x804b000								//*******add new segment
	printf("p_vaddr = %x\n", n_phdr.p_vaddr);
	n_phdr.p_paddr = last_vaddr; //*******add new segment
	n_phdr.p_filesz = PAGESIZE; //0x1000								//*******add new segment
	n_phdr.p_memsz = PAGESIZE; //*******add new segment
	n_phdr.p_flags = load_phdr.p_flags; //*******add new segment
	n_phdr.p_align = load_phdr.p_align; //*******add new segment
//******************* end a new program header

	//get file size with fileStat.st_size
	struct stat fileStat;
	fstat(fd, &fileStat);

	ehdr.e_phnum += 1; //*******add new segment
	lseek(fd, 0, SEEK_SET);
	write(fd, &ehdr, sizeof(ehdr));

	char* data = NULL;
	long int oldSize = oldPhoff + (ehdr.e_phnum - 1) * ehdr.e_phentsize;
	data = (char*) malloc(fileStat.st_size - oldSize);
	lseek(fd, oldSize, SEEK_SET);
	read(fd, data, fileStat.st_size - oldSize);

	//add entry to program header
	lseek(fd, oldSize, SEEK_SET);
	write(fd, &n_phdr, sizeof(n_phdr));

	char tmp[PAGESIZE] = { 0 };
	memset(tmp, 0, PAGESIZE - sizeof(n_phdr));
	write(fd, tmp, PAGESIZE - sizeof(n_phdr));

	write(fd, data, fileStat.st_size - oldSize);

	//padding between last and add
	data = (char*) malloc(add_last_section_gap);
	memset(data, 0, add_last_section_gap);
	write(fd, data, add_last_section_gap);

	//add data to the end of the file
	write(fd, insert_code, sizeof(insert_code));
	data = (char*) malloc(PAGESIZE - sizeof(insert_code));
	memset(data, 0, PAGESIZE - sizeof(insert_code));
	write(fd, data, PAGESIZE - sizeof(insert_code));

	free(data);
	close(fd);

	return add_offset;
}

Elf32_Addr findMainFunc(char* elfFile) {
	int fd;
	Elf *e;

	fd = open(elfFile, O_RDWR);
	elf_version(EV_CURRENT);
	e = elf_begin(fd, ELF_C_RDWR, NULL);

	size_t strtabScnNdxArray[2];
	strtabScnNdxArray[0] = getScnNdx(e, ".strtab");
	strtabScnNdxArray[1] = getScnNdx(e, ".dynstr");

	size_t symScnNdxArray[2];
	symScnNdxArray[0] = getScnNdx(e, ".symtab");
	symScnNdxArray[1] = getScnNdx(e, ".dynsym");

	size_t textScnNdx = getScnNdx(e, ".text");
	Elf32_Addr main_addr = 0;
	int i;
	for (i = 0; i < 2; i++) {
		size_t symScnNdx = symScnNdxArray[i];

		Elf_Scn *symScn = elf_getscn(e, symScnNdx);

		Elf_Data *symData;
		if ((symData = elf_getdata(symScn, NULL)) == NULL) {
			return 0;
		}
		Elf32_Sym *sym = (Elf32_Sym *) (symData->d_buf);
		int symNum = symData->d_size / sizeof(Elf32_Sym);

		char *name;
		int j;
		for (j = 1; j < symNum; j++) {
			sym++;
			int st_type = ELF32_ST_TYPE(sym->st_info);

			if (st_type == STT_FUNC && sym->st_shndx == textScnNdx) {
				name = elf_strptr(e, strtabScnNdxArray[i], sym->st_name);
				if (strcmp(name, "main") == 0) {
					main_addr = sym->st_value;
				}
			}
		}
	}

	printf("main_addr = %x\n", main_addr);
	elf_end(e);
	close(fd);

	return main_addr;
}

int modify_call_addr(Elf32_Off add_offset, Elf32_Addr main_addr, char* elfFile) {
	Elf32_Off main_file_offset = (main_addr % PAGESIZE ) + PAGESIZE;
	Elf32_Off call_ins_file_offset = main_file_offset + 24;

	Elf32_Off call_add_gap = add_offset - (call_ins_file_offset + 5);

	int fd = open(elfFile, O_RDWR);

	unsigned char call_offset[4] = {0xf2, 0x2b, 0x00, 0x00};//changeGapToOffset(call_add_gap);
	lseek(fd, call_ins_file_offset + 1, SEEK_SET);
	printf("call_ins_file_offset = %x\n", call_ins_file_offset);
	printf("call_add_gap = %x\n", call_add_gap);
	write(fd, call_offset, 4);

	close(fd);
	return 0;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("usage : %s file\n", argv[0]);
		exit(0);
	}

	Elf32_Off add_offset = add_segment(argv[1]);
	Elf32_Addr main_addr = findMainFunc(argv[1]);
	modify_call_addr(add_offset, main_addr, argv[1]);

	return 0;
}

