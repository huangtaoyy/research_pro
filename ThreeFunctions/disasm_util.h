#include <libdis.h>
#include <stddef.h>
char *get_operand_type_str(enum x86_op_type type);

char *get_insn_type_str(enum x86_insn_type type);

char *get_operand_access_str(enum x86_op_access type);
char *get_operand_flag_str(enum x86_op_flags flag);
char *get_reg_type_str(enum x86_reg_type type);
