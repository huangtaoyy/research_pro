/*
 * main.cpp
 *
 *  Created on: 2013-12-19
 *      Author: huangtao
 */
#include "append_func.h"
#include <libelf.h>
#include <elf.h>
#include <fcntl.h>


int main(int argc, char** argv) {
	if (argc != 2) {
		printf("usage : %s file\n", argv[0]);
		exit(0);
	}
	Elf32_Phdr n_phdr;
	Elf32_Off add_offset = add_segment(argv[1], n_phdr);

	int fd;
	Elf *e;

	fd = open(argv[1], O_RDWR);
	elf_version(EV_CURRENT);
	e = elf_begin(fd, ELF_C_RDWR, NULL);

	//get data for add, sub, mul
	FuncList* func_list = new FuncList;
	Elf32_Sym *sym_add = find_func_sym(e, "add", func_list);
	printf("sym_add = %x\n", sym_add->st_value);
	Elf32_Sym *sym_sub = find_func_sym(e, "sub", func_list);
	printf("sym_sub = %x\n", sym_sub->st_value);
	Elf32_Sym *sym_mul = find_func_sym(e, "mul", func_list);
	printf("sym_mul = %x\n", sym_mul->st_value);

	set_func_end_addr(func_list);
	get_func_body(e, func_list);

	//exchange the whole order
	print_func_list(func_list);
//	func_list = exchange_func_order(func_list);
	print_func_list(func_list);

	add_func_to_segment(fd, add_offset, func_list);

	//get data for main
	Function* main_func = (Function*) malloc(sizeof(Function));
	Elf32_Sym* main_sym = find_func_sym(e, "main", main_func);
	printf("main_sym = %x\n", main_sym->st_value);

	FuncList* main_func_list = new FuncList;
	main_func_list->func = main_func;
	main_func_list->next = NULL;

	get_func_body(e, main_func_list);

	modify_call_target_func(main_func, func_list, n_phdr);

	printf("updating call target address in main\n");
	update_main_func_binary(fd, e, main_func);

	elf_end(e);
	close(fd);

	return 0;
}
