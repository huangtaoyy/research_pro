/*
 * modify_fun.cpp
 *
 *  Created on: 2013-11-22
 *      Author: huangtao
 */
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define PAGESIZE 4096

char insert_code[] = {'f', 'f', '3', '5', '3', '0', '8', '3',
			'0', '4', '0', '8'};

int add_segment(char* elfFile) {
	int fd;
	int oldShoff;
	int oldPhoff;

	Elf32_Ehdr ehdr;
	Elf32_Shdr shdr;
	Elf32_Phdr phdr;

	//open elf file and read elf header to ehdr
	fd = open(elfFile, O_RDWR);
	read(fd, &ehdr, sizeof(ehdr));
//	if (strncmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
//		exit(0);
//	}

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
	for (; i < ehdr.e_phnum; i ++) {
		//read program header to phdr
		lseek(fd, ehdr.e_phoff + i * ehdr.e_phentsize, SEEK_SET);
		read(fd, &phdr, sizeof(phdr));

		if (i == 2) {
			phdr.p_filesz += PAGESIZE;
			phdr.p_memsz += PAGESIZE;
			phdr.p_vaddr -= PAGESIZE;
		}
		if(phdr.p_offset != 0x0) {
			//increase p_offset by PAGESIZE
			phdr.p_offset += PAGESIZE;
		}

		//write back
		lseek(fd, ehdr.e_phoff + i * ehdr.e_phentsize, SEEK_SET);
		write(fd, &phdr, sizeof(phdr));
	}

	//loop section header and modify the offset
	i = 1;
	for (; i < ehdr.e_shnum; i ++) {
		//read section header to shdr
		lseek(fd, oldShoff + i * ehdr.e_shentsize, SEEK_SET);
		read(fd, &shdr, sizeof(shdr));

		//increase sh_offset by PAGESIZE
		shdr.sh_offset += PAGESIZE;

		//write back
		lseek(fd, oldShoff + i * ehdr.e_shentsize, SEEK_SET);
		write(fd, &shdr, sizeof(shdr));
	}

	//a new program header
	Elf32_Phdr n_phdr;														//*******add new segment
	Elf32_Phdr load_phdr;													//*******add new segment
	lseek(fd, ehdr.e_phoff + 2 * ehdr.e_phentsize, SEEK_SET);				//*******add new segment
	read(fd, &load_phdr, sizeof(phdr));										//*******add new segment

	//last_offset and last_vaddr
	lseek(fd, oldShoff + (ehdr.e_shnum - 1) * ehdr.e_shentsize, SEEK_SET);	//*******add new segment
	printf("(ehdr.e_shnum - 1) = %d\n", (ehdr.e_shnum - 1));
	read(fd, &shdr, sizeof(shdr));											//*******add new segment
	Elf32_Off last_offset = shdr.sh_offset + shdr.sh_size;					//*******add new segment
	Elf32_Addr last_vaddr = load_phdr.p_vaddr + 4 * PAGESIZE; //0x804b000	//*******add new segment

	//set attributes
	n_phdr.p_type = load_phdr.p_type;										//*******add new segment
	printf("p_type = %d\n", n_phdr.p_type);

	Elf32_Off add_offset = ((last_offset + load_phdr.p_align) / load_phdr.p_align) * load_phdr.p_align;
	Elf32_Off add_last_gap = add_offset - last_offset;
	n_phdr.p_offset = add_offset;
	printf("p_offset = %x\n", n_phdr.p_offset);								//*******add new segment

	n_phdr.p_vaddr = last_vaddr; //0x804b000								//*******add new segment
	printf("p_vaddr = %x\n", n_phdr.p_vaddr);
	n_phdr.p_paddr = last_vaddr;											//*******add new segment
	n_phdr.p_filesz = PAGESIZE;      //0x1000								//*******add new segment
	n_phdr.p_memsz = PAGESIZE;												//*******add new segment
	n_phdr.p_flags = load_phdr.p_flags;										//*******add new segment
	n_phdr.p_align = load_phdr.p_align;										//*******add new segment


	//get file size with fileStat.st_size
	struct stat fileStat;
	fstat(fd, &fileStat);

	ehdr.e_phnum += 1;														//*******add new segment
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

	char tmp[PAGESIZE] = {0};
	memset(tmp, 0, PAGESIZE - sizeof(n_phdr));
	write(fd, tmp, PAGESIZE - sizeof(n_phdr));

	write(fd, data, fileStat.st_size - oldSize);

	//padding between last and add
	data = (char*) malloc(add_last_gap);
	memset(data, 0, add_last_gap);
	write(fd, data, add_last_gap);

	//add data to the end of the file
	data = (char*) malloc(PAGESIZE);
	memset(data, 0, PAGESIZE);
	write(fd, data, PAGESIZE);

	free(data);

	return 0;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("usage : %s file\n", argv[0]);
		exit(0);
	}

	add_segment(argv[1]);

	return 0;
}




