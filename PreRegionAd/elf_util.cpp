
#include <err.h>
#include <fcntl.h>
#include <elf.h>
#include <gelf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include <string.h>
#include "elf_util.h"

size_t get_scn_ndx(Elf *e, const char *scn_name) {
	size_t shdrstrndx = 0;
	if (elf_getshdrstrndx(e, &shdrstrndx) != 0)
		errx(EX_SOFTWARE, "elf_getshstrndx() failed:%s.", elf_errmsg(-1));

	Elf_Scn *scn;
	GElf_Shdr shdr;

	char *name;
	size_t shnum;
	if (elf_getshdrnum(e, &shnum) != 0)
		errx(EX_SOFTWARE, "elf_getshnum() failed:%s.", elf_errmsg(-1));
	size_t i;
	for (i = 0; i < shnum; i++) {
		scn = elf_getscn(e, i);

		if (gelf_getshdr(scn, &shdr) != &shdr)
			errx(EX_SOFTWARE, "getshdr() failed: %s.", elf_errmsg(-1));

		if ((name = elf_strptr(e, shdrstrndx, shdr.sh_name)) == NULL )
			errx(EX_SOFTWARE, "elf_strptr() failed: %s.", elf_errmsg(-1));

		if (strcmp(name, scn_name) == 0) {
			size_t elf_ndx = elf_ndxscn(scn);
			return elf_ndx;
		}
	}
	return 0;
}

void print_scn_content(Elf *e, const char *scn_name) {
	size_t scnNdx = get_scn_ndx(e, scn_name);
	Elf_Scn *scn = elf_getscn(e, scnNdx);
	Elf_Data *data;
	if ((data = elf_getdata(scn, NULL )) == NULL ) {
		errx(EX_SOFTWARE, "elf_getdata() failed:%s.", elf_errmsg(-1));
	}

	char *dbuff = (char *)data->d_buf;
	size_t i;
	printf("contents of section %s: \n", scn_name);

	for (i = 0; i < data->d_size; i++) {
		printf("%02hhx ", dbuff[i]);
		if ((i + 1) % 4 == 0) {
			printf("\n");
		}
	}

	for (i = 0; i < data->d_size; i++) {
		printf("%c ", dbuff[i]);
		if ((i + 1) % 4 == 0) {
			printf("\n");
		}
	}

}
