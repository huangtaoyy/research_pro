#include "disasm_util.h"
#include "stddef.h"

char *get_operand_type_str(enum x86_op_type type) {
	static char *types[] = { "op_unused", "op_register", "op_immediate",
			"op_relative_near", "op_relative_far", "op_absolute",
			"op_expression", "op_offset", "op_unknown" };
	int elementSize = sizeof(types) / sizeof(types[0]);
	if (type >= elementSize) {
		return types[elementSize - 1];
	} else {
		return types[type];
	}

}

char *get_reg_type_str(enum x86_reg_type type) {

	static struct {
		char *name;
		int value;
	} types[] = { { "reg_gen", 0x00001 }, { "reg_in", 0x00002 }, { "reg_out",
			0x00004 }, { "reg_local", 0x00008 }, { "reg_fpu", 0x00010 }, {
			"reg_seg", 0x00020 }, { "reg_simd", 0x00040 },
			{ "reg_sys", 0x00080 }, { "reg_sp", 0x00100 },
			{ "reg_fp", 0x00200 }, { "reg_pc", 0x00400 }, { "reg_retaddr",
					0x00800 }, { "reg_cond", 0x01000 }, { "reg_zero", 0x02000 },
			{ "reg_ret", 0x04000 }, { "reg_src", 0x10000 }, { "reg_dest",
					0x20000 }, { "reg_count", 0x40000 }, { NULL, 0 }, };

	unsigned int i;

	//go thru every type in the enum
	for (i = 0; types[i].name; i++) {
		if (types[i].value == type)
			return types[i].name;
	}

	return NULL;
}

char *get_operand_flag_str(enum x86_op_flags flag) {

	static struct {
		char *name;
		int value;
	} flags[] = { { "op_signed", 1 }, { "op_string", 2 }, { "op_constant", 4 },
			{ "op_pointer", 8 }, { "op_sysref", 0x010 },
			{ "op_implied", 0x020 }, { "op_hardcode", 0x40 }, { "op_es_seg",
					0x100 }, { "op_cs_seg", 0x200 }, { "op_ss_seg", 0x300 }, {
					"op_ds_seg", 0x400 }, { "op_fs_seg", 0x500 }, { "op_gs_seg",
					0x600 }, { NULL, 0 }, };

	unsigned int i;

	//go thru every type in the enum
	for (i = 0; flags[i].name; i++) {
		if (flags[i].value == flag)
			return flags[i].name;
	}

	return NULL;
}
char *get_operand_access_str(enum x86_op_access type) {
	static struct {
		char *name;
		int value;
	} accesses[] = { { "op_read", 1 }, { "op_write", 2 }, { "op_execute", 4 }, {
			NULL, 0 } };

	unsigned int i;

	//go thru every type in the enum
	for (i = 0; accesses[i].name; i++) {
		if (accesses[i].value == type)
			return accesses[i].name;
	}

	return NULL;
}

char *get_insn_type_str(enum x86_insn_type type) {

	static struct {
		char *name;
		int value;
	} types[] = {
	/* insn_controlflow */
	{ "jmp", 0x1001 }, { "jcc", 0x1002 }, { "call", 0x1003 },
			{ "callcc", 0x1004 }, { "return", 0x1005 }, { "loop", 0x1006 },
			/* insn_arithmetic */
			{ "add", 0x2001 }, { "sub", 0x2002 }, { "mul", 0x2003 }, { "div",
					0x2004 }, { "inc", 0x2005 }, { "dec", 0x2006 }, { "shl",
					0x2007 }, { "shr", 0x2008 }, { "rol", 0x2009 }, { "ror",
					0x200A },
			/* insn_logic */
			{ "and", 0x3001 }, { "or", 0x3002 }, { "xor", 0x3003 }, { "not",
					0x3004 }, { "neg", 0x3005 },
			/* insn_stack */
			{ "push", 0x4001 }, { "pop", 0x4002 }, { "pushregs", 0x4003 }, {
					"popregs", 0x4004 }, { "pushflags", 0x4005 }, { "popflags",
					0x4006 }, { "enter", 0x4007 }, { "leave", 0x4008 },
			/* insn_comparison */
			{ "test", 0x5001 }, { "cmp", 0x5002 },
			/* insn_move */
			{ "mov", 0x6001 }, /* move */
			{ "movcc", 0x6002 }, /* conditional move */
			{ "xchg", 0x6003 }, /* exchange */
			{ "xchgcc", 0x6004 }, /* conditional exchange */
			/* insn_string */
			{ "strcmp", 0x7001 }, { "strload", 0x7002 }, { "strmov", 0x7003 }, {
					"strstore", 0x7004 }, { "translate", 0x7005 }, /* xlat */
			/* insn_bit_manip */
			{ "bittest", 0x8001 }, { "bitset", 0x8002 }, { "bitclear", 0x8003 },
			/* insn_flag_manip */
			{ "clear_carry", 0x9001 }, { "clear_zero", 0x9002 }, {
					"clear_oflow", 0x9003 }, { "clear_dir", 0x9004 }, {
					"clear_sign", 0x9005 }, { "clear_parity", 0x9006 }, {
					"set_carry", 0x9007 }, { "set_zero", 0x9008 }, {
					"set_oflow", 0x9009 }, { "set_dir", 0x900A }, { "set_sign",
					0x900B }, { "set_parity", 0x900C }, { "tog_carry", 0x9010 },
			{ "tog_zero", 0x9020 }, { "tog_oflow", 0x9030 },
			{ "tog_dir", 0x9040 }, { "tog_sign", 0x9050 }, { "tog_parity",
					0x9060 },
			/* insn_fpu */
			{ "fmov", 0xA001 }, { "fmovcc", 0xA002 }, { "fneg", 0xA003 }, {
					"fabs", 0xA004 }, { "fadd", 0xA005 }, { "fsub", 0xA006 }, {
					"fmul", 0xA007 }, { "fdiv", 0xA008 }, { "fsqrt", 0xA009 }, {
					"fcmp", 0xA00A }, { "fcos", 0xA00C }, { "fldpi", 0xA00D }, {
					"fldz", 0xA00E }, { "ftan", 0xA00F }, { "fsine", 0xA010 }, {
					"fsys", 0xA020 },
			/* insn_interrupt */
			{ "int", 0xD001 }, { "intcc", 0xD002 }, /* not present in x86 ISA */
			{ "iret", 0xD003 }, { "bound", 0xD004 }, { "debug", 0xD005 }, {
					"trace", 0xD006 }, { "invalid_op", 0xD007 }, { "oflow",
					0xD008 },
			/* insn_system */
			{ "halt", 0xE001 }, { "in", 0xE002 }, /* input from port/bus */
			{ "out", 0xE003 }, /* output to port/bus */
			{ "cpuid", 0xE004 },
			/* insn_other */
			{ "nop", 0xF001 }, { "bcdconv", 0xF002 }, /* convert to or from BCD */
			{ "szconv", 0xF003 }, /* change size of operand */
			{ NULL, 0 } //end
	};

	unsigned int i;

//go thru every type in the enum
	for (i = 0; types[i].name; i++) {
		if (types[i].value == type)
			return types[i].name;
	}

	return NULL;
}
