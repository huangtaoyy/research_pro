/*
 * elf_util.c
 *
 *  Created on: 2012-8-23
 *      Author: michael
 */

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

size_t getScnNdx(Elf *e, char *scnName) {
	size_t shstrndx;
	if (elf_getshstrndx(e, &shstrndx) == 0)
		errx(EX_SOFTWARE, "elf_getshstrndx() failed:%s.", elf_errmsg(-1));

	Elf_Scn *scn;
	GElf_Shdr shdr;

	char *name;
	int shnum;
	if (elf_getshnum(e, &shnum) == 0)
		errx(EX_SOFTWARE, "elf_getshnum() failed:%s.", elf_errmsg(-1));
	int i;
	for (i = 0; i < shnum; i++) {
		scn = elf_getscn(e, i);

		if (gelf_getshdr(scn, &shdr) != &shdr)
			errx(EX_SOFTWARE, "getshdr() failed: %s.", elf_errmsg(-1));

		if ((name = elf_strptr(e, shstrndx, shdr.sh_name)) == NULL )
			errx(EX_SOFTWARE, "elf_strptr() failed: %s.", elf_errmsg(-1));

		if (strcmp(name, scnName) == 0) {
			size_t elf_ndx = elf_ndxscn(scn);
			return elf_ndx;
		}
	}
	return 0;
}

void printScnContent(Elf *e, char *scnName) {
	size_t scnNdx = getScnNdx(e, scnName);
	Elf_Scn *scn = elf_getscn(e, scnNdx);
	Elf_Data *data;
	if ((data = elf_getdata(scn, NULL )) == NULL ) {
		errx(EX_SOFTWARE, "elf_getdata() failed:%s.", elf_errmsg(-1));
	}

	char *dbuff = data->d_buf;
	int i;
	printf("contents of section %s: \n", scnName);

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

