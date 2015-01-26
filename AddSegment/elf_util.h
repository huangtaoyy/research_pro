/*
 * elf_util.h
 *
 *  Created on: 2012-8-23
 *      Author: michael
 */

#ifndef ELF_UTIL_H_
#define ELF_UTIL_H_

#include <libelf.h>

size_t getScnNdx(Elf *e, char *scnName);
void printScnContent(Elf *e, char *scnName);

#endif /* ELF_UTIL_H_ */
