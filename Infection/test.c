/*
 * test.c
 *
 *  Created on: 2013-11-16
 *      Author: huangtao
 */
#include <dlfcn.h>
#include <stdio.h>

int mul(int a, int b) {
	return a * b;
}

int main() {
//	void *libc;
//	void (*printf_call)();
//	char* error_text;
//	if ((libc = dlopen("/lib/i386-linux-gnu/libc.so.6", RTLD_LAZY))) {
//		printf_call = dlsym(libc, "printf");
//		(*printf_call)("hello, world\n");
//		dlclose(libc);
//		return 0;
//	}
//	error_text = dlerror();
//	printf(error_text);
	printf("%d\n", mul(3, 5));
	return -2;
}

/*
 *
 * for (ph = phdr; ph < &phdr[l->l_phnum]; ++ph)
  1449        switch (ph->p_type)
  1450    {
………..
  1454    case PT_DYNAMIC:
  1455      l->l_ld = (void *) ph->p_vaddr;
  1456      l->l_ldnum = ph->p_memsz / sizeof (ElfW(Dyn));
  1457      break;
  1458
  1459    case PT_PHDR:
  1460      l->l_phdr = (void *) ph->p_vaddr;
  1461      break;
  1462
  1463    case PT_LOAD:
 …………..
  1467      c = &loadcmds[nloadcmds++];
  1468      c->mapstart = ph->p_vaddr & ~(ph->p_align - 1);
  1469      c->mapend = ((ph->p_vaddr + ph->p_filesz + GL(dl_pagesize) - 1)
  1470             & ~(GL(dl_pagesize) - 1));
  1471      c->dataend = ph->p_vaddr + ph->p_filesz;
  1472      c->allocend = ph->p_vaddr + ph->p_memsz;
  1473      c->mapoff = ph->p_offset & ~(ph->p_align - 1);
…………..
  1480      c->prot = 0;
  1481      if (ph->p_flags & PF_R)
  1482        c->prot |= PROT_READ;
  1483      if (ph->p_flags & PF_W)
  1484        c->prot |= PROT_WRITE;
  1485      if (ph->p_flags & PF_X)
  1486        c->prot |= PROT_EXEC;
    1488      break;
 */

