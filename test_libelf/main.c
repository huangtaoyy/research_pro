#include <err.h>
#include <fcntl.h>

#include <gelf.h>
//#include <libelf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv) {

	printf("%d = \n", PF_R | PF_W);
	printf("%x, %x\n", 0xf14 + 0x104 + 0x23c, 0x49f14 + 0x10c + 0x23c);
	int i, fd;

	Elf *e;
	size_t n;
	GElf_Ehdr ehdr;

	if ( argc != 2)
		errx ( EXIT_FAILURE , " usage : %s file - name " , argv [0]);

	if (elf_version(EV_CURRENT) == EV_NONE)
		errx ( EXIT_FAILURE , " ELF library initialization "
		" failed : %s " , elf_errmsg ( -1));

	if((fd = open(argv[1], O_RDONLY, 0)) < 0)
		err ( EXIT_FAILURE , " open %s failed " , argv [1]);

	if ((e = elf_begin(fd, ELF_C_READ, NULL)) == NULL)
		errx ( EXIT_FAILURE , " elf_begin () failed : %s . " ,
		elf_errmsg ( -1));

	if(elf_kind(e) != ELF_K_ELF)
		errx ( EXIT_FAILURE , " %s is not an elf object. " ,
				argv[1]);

	if(gelf_getehdr(e, &ehdr) == NULL)
		errx ( EXIT_FAILURE , " get_ehdr () failed : %s . " ,
				elf_errmsg ( -1));

	if((i = gelf_getclass(e)) == ELFCLASSNONE)
		errx ( EXIT_FAILURE , " elf_getclass () failed : %s . " ,
				elf_errmsg ( -1));

	if(elf_getshdrnum(e, &n) != 0)
		errx ( EXIT_FAILURE , " elf_getshdrnum () failed : %s . " ,
				elf_errmsg ( -1));
	printf("shdrnum = %d, ehdr.e_phnum = %d\n", n, ehdr.e_phnum);

	if(elf_getphdrnum(e, &n) != 0)
		errx ( EXIT_FAILURE , " elf_getphdrnum () failed : %s . " ,
					elf_errmsg ( -1));
	printf("phdrnum = %d\n", n);

	if(elf_getshdrstrndx(e, &n) != 0)
		errx ( EXIT_FAILURE , " elf_getshdrstrndx () failed : %s . " ,
							elf_errmsg ( -1));
	printf("shdrstrndx = %d\n", n);

	(void) elf_end(e);
	(void) close(fd);

	exit(EXIT_SUCCESS);

}
