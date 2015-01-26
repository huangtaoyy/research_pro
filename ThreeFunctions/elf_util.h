#include <libelf.h>

size_t get_scn_ndx(Elf *e, const char *scn_name);
void print_scn_content(Elf *e, const char *scn_name);
